from typing import List, Dict
from rules_parser import Rule, Response, ConditionBlock
from log_parser import LogEvent

class EvaluationResult:
    def __init__(self, trigger: str):
        self.trigger = trigger
        self.passed = True
        self.missing_responses: List[str] = []
        self.unexpected_responses: List[str] = []
        self.failed_assertions: List[str] = []
        self.execution_errors: List[str] = []
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

        # Prepare working variable map
        variable_map = event.start_state.copy()

        # Execute assignments
        for step in rule.execute:
            try:
                exec(step.formula, {}, variable_map)
            except Exception as e:
                result.fail(f"Execution error ({step.description}): {e}")
                result.execution_errors.append(step.description)

        # Evaluate condition block
        selected_block = None
        for block in rule.conditions:
            if block.condition is None:
                continue
            try:
                if eval(block.condition, {}, variable_map):
                    selected_block = block
                    break
            except Exception as e:
                result.fail(f"Condition evaluation error: {e}")
                return result

        if not selected_block:
            for block in rule.conditions:
                if block.condition is None:
                    selected_block = block
                    break

        # Check expected responses from the selected block
        expected = (selected_block.responses if selected_block else []) + rule.always
        for response in expected:
            expected_line = f"{response.type}: {response.value}"
            if not any(expected_line in line for line in event.buffer):
                result.fail(f"Missing response: {expected_line}")
                result.missing_responses.append(expected_line)

        # Check forbidden responses from unmatched blocks and 'never'
        all_possible = [r for cond in rule.conditions if cond != selected_block for r in cond.responses] + rule.never
        for response in all_possible:
            unexpected_line = f"{response.type}: {response.value}"
            if any(unexpected_line in line for line in event.buffer):
                result.fail(f"Unexpected response: {unexpected_line}")
                result.unexpected_responses.append(unexpected_line)

        # Check followed_by from selected block
        if selected_block and selected_block.followed_by:
            next_triggers = [e.trigger for e in self.rules.values()]
            for fb in selected_block.followed_by:
                if fb not in next_triggers:
                    result.fail(f"Missing followed_by trigger: {fb}")
                    result.missing_followed_by.append(fb)

        # Evaluate assertions
        for assertion in rule.assertions:
            try:
                if not eval(assertion.formula, {}, variable_map):
                    result.fail(f"Assertion failed: {assertion.description}")
                    result.failed_assertions.append(assertion.description)
            except Exception as e:
                result.fail(f"Assertion error ({assertion.description}): {e}")
                result.failed_assertions.append(assertion.description)

        return result

    def evaluate_all(self, events: List[LogEvent]) -> List[EvaluationResult]:
        return [self.evaluate_event(event) for event in events]
