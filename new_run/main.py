import sys
from rules_parser import parse_rules
from log_parser import LogParser
from evaluator import Evaluator


def main():
    if len(sys.argv) != 4:
        print("Usage: python main.py rules.yaml log.txt test_result.txt")
        sys.exit(1)

    rules_path = sys.argv[1]
    log_path = sys.argv[2]
    result_path = sys.argv[3]

    # Load rules
    rules = parse_rules(rules_path)
    known_triggers = [rule.trigger for rule in rules]
    known_vars = list({var for rule in rules for cond in rule.conditions if cond.condition for var in cond.condition.replace('==', ' ').replace('!=', ' ').replace('>', ' ').replace('<', ' ').split() if var.isidentifier()})

    # Parse log
    parser = LogParser(known_triggers, known_vars)
    events = parser.parse_log_file(log_path)

    # Evaluate
    evaluator = Evaluator(rules)
    results = evaluator.evaluate_all(events)

    # Write results
    with open(result_path, 'w') as f:
        for result in results:
            status = "PASS" if result.passed else "FAIL"
            f.write(f"Trigger: {result.trigger}\n")
            f.write(f"Status: {status}\n")
            if result.missing_responses:
                f.write("Missing Responses:\n")
                for line in result.missing_responses:
                    f.write(f"  - {line}\n")
            if result.failed_assertions:
                f.write("Failed Assertions:\n")
                for line in result.failed_assertions:
                    f.write(f"  - {line}\n")
            if result.missing_followed_by:
                f.write("Missing Followed By:\n")
                for line in result.missing_followed_by:
                    f.write(f"  - {line}\n")
            f.write("\n")

if __name__ == "__main__":
    main()

