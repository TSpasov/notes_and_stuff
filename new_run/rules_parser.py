import yaml
from dataclasses import dataclass, field
from typing import List, Optional

# === Data classes to represent rules ===

@dataclass
class Response:
    type: str  # e.g., 'sound', 'animation', 'state'
    value: str

@dataclass
class ConditionBlock:
    condition: Optional[str]  # the 'if' or 'elif' expression; None if it's 'else'
    responses: List[Response]
    followed_by: List[str] = field(default_factory=list)

@dataclass
class Rule:
    trigger: str
    conditions: List[ConditionBlock] = field(default_factory=list)
    unconditional: List[Response] = field(default_factory=list)
    assertions: List[str] = field(default_factory=list)

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
    if isinstance(raw_rules, list):
        rule_blocks = raw_rules
    else:
        rule_blocks = [raw_rules]

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

        rule.unconditional = [parse_response_line(r) for r in block.get('unconditional', [])]

        if 'assert' in block:
            if isinstance(block['assert'], list):
                rule.assertions = [a['formula'] if isinstance(a, dict) else a for a in block['assert']]
            else:
                rule.assertions = [block['assert']]

        rules.append(rule)

    return rules


