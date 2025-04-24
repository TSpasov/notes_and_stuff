from typing import List, Dict
from rules_parser import Rule, Response, ConditionBlock
from log_parser import LogEvent

class EvaluationResult:
    def __init__(self, trigger: str):
        self.trigger = trigger
        self.passed = True
        self.missing_responses: List[str] = []
        self.failed_assertions: List[str] = []
        self.missing_followed_by: List[str] = []

    def fail(self, reason: str):
        self.passed = False
        print(f"[FAIL] {self.trigger}: {reason}")

class Evaluator:
    def __init__(self, rules: List[Rule]):
        self.rules = {rule.trigger: rule for rule in rules}

    def evaluate_event(self, event: LogEvent) -> EvaluationResult:
        result = EvaluationResult(event.trigger)
        rule = self.rules.get(event.trigger)

        if not rule:
            result.fail("No rule defined for trigger")
            return result

        # Evaluate condition block
        selected_block = None
        for block in rule.conditions:
            if block.condition is None:
                continue  # handle else later
            try:
                if eval(block.condition, {}, event.start_state):
                    selected_block = block
                    break
            except Exception as e:
                result.fail(f"Condition error: {e}")
                return result

        if not selected_block:
            for block in rule.conditions:
                if block.condition is None:  # else block
                    selected_block = block
                    break

        # Check responses from selected block
        expected = (selected_block.responses if selected_block else []) + rule.unconditional
        for response in expected:
            expected_line = f"{response.type}: {response.value}"
            if not any(expected_line in line for line in event.buffer):
                result.fail(f"Missing response: {expected_line}")
                result.missing_responses.append(expected_line)

        # Check followed_by
        if selected_block and selected_block.followed_by:
            if not any(fb in [e.trigger for e in self.rules.values()] for fb in selected_block.followed_by):
                result.fail(f"Missing followed_by trigger(s): {selected_block.followed_by}")
                result.missing_followed_by.extend(selected_block.followed_by)

        # Evaluate assertions
        current_state = event.buffer[-1] if event.buffer else {}
        for formula in rule.assertions:
            try:
                if not eval(formula, {}, {**event.start_state, **current_state}):
                    result.fail(f"Assertion failed: {formula}")
                    result.failed_assertions.append(formula)
            except Exception as e:
                result.fail(f"Assertion error: {e}")
                result.failed_assertions.append(formula)

        return result

    def evaluate_all(self, events: List[LogEvent]) -> List[EvaluationResult]:
        return [self.evaluate_event(event) for event in events]

