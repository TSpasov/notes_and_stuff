import yaml
from dataclasses import dataclass, field
from typing import List, Optional

# === Data classes to represent rules ===

@dataclass
class Response:
    type: str  # e.g., 'sound', 'animation', 'state'
    value: str

@dataclass
class Execute:
    description: str
    formula: str

@dataclass
class Assertion:
    description: str
    formula: str

@dataclass
class ConditionBlock:
    condition: Optional[str]  # the 'if' or 'elif' expression; None if it's 'else'
    responses: List[Response]
    followed_by: List[str] = field(default_factory=list)

@dataclass
class Rule:
    trigger: str
    conditions: List[ConditionBlock] = field(default_factory=list)
    always: List[Response] = field(default_factory=list)
    never: List[Response] = field(default_factory=list)
    execute: List[Execute] = field(default_factory=list)
    assertions: List[Assertion] = field(default_factory=list)

# === Parsing functions ===

def parse_response_line(line: str) -> Response:
    if ':' in line:
        key, val = line.split(':', 1)
        return Response(type=key.strip(), value=val.strip())
    else:
        raise ValueError(f"Invalid response line: {line}")

def parse_rules(yaml_file_path: str) -> List[Rule]:
    with open(yaml_file_path, 'r') as f:
        raw_rules = yaml.safe_load(f)

    rules = []

    # If rules are provided as a list
    rule_blocks = raw_rules if isinstance(raw_rules, list) else [raw_rules]

    for block in rule_blocks:
        trigger = block.get('trigger')
        rule = Rule(trigger=trigger)

        for cond in block.get('conditions', []):
            cond_expr = cond.get('if') or cond.get('elif')
            responses = [parse_response_line(r) for r in cond.get('then', [])]
            followed_by = [f["trigger"] for f in cond.get('followed_by', [])]
            rule.conditions.append(ConditionBlock(condition=cond_expr, responses=responses, followed_by=followed_by))

            if 'else' in cond:
                responses = [parse_response_line(r) for r in cond['else']]
                followed_by = [f["trigger"] for f in cond.get('followed_by', [])]
                rule.conditions.append(ConditionBlock(condition=None, responses=responses, followed_by=followed_by))

        rule.always = [parse_response_line(r) for r in block.get('always', [])]
        rule.never = [parse_response_line(r) for r in block.get('never', [])]

        for exe in block.get('execute', []):
            if isinstance(exe, dict):
                rule.execute.append(Execute(description=exe['description'], formula=exe['formula']))

        for assertion in block.get('assert', []):
            if isinstance(assertion, dict):
                rule.assertions.append(Assertion(description=assertion['description'], formula=assertion['formula']))

        rules.append(rule)

    return rules
