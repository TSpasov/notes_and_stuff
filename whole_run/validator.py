

# import ast
# from lxml import etree
# import re  # For extracting values using regex

# # Function to evaluate custom function-like expressions
# def evaluate_custom_expression(expression, context):
#     try:
#         # Replace custom function calls with equivalent Python logic
#         expression = expression.replace("AND(", "and(")
#         expression = expression.replace("OR(", "or(")
#         expression = expression.replace("NOT(", "not(")
#         expression = expression.replace("LESS_THAN_EQUAL(", "(")  # a <= b
#         expression = expression.replace("GREATER_THAN_EQUAL(", "(")  # a >= b
#         expression = expression.replace("LESS_THAN(", "(")  # a < b
#         expression = expression.replace("GREATER_THAN(", "(")  # a > b
#         expression = expression.replace("EQUAL(", "(")  # a == b
#         expression = expression.replace("NOT_EQUAL(", "(")  # a != b

#         # Safely evaluate the modified expression
#         return eval(expression, {}, context)
#     except Exception as e:
#         return str(e)

import re
import xml.etree.ElementTree as ET


class LogValidator:
    def __init__(self, log_file):
        self.logs = self.load_logs(log_file)
        self.log_history = []  # Store recent log entries
        self.errors_reported = set()  # Avoid reporting the same error multiple times

    def load_logs(self, log_file):
        with open(log_file, 'r') as f:
            return [line.strip() for line in f.readlines()]

    def process_logs(self, rule_set):
        context = {}  # Empty context to store extracted values

        for log in self.logs:
            self.log_history.append(log)
            self.extract_context(log, context)  # Extract variables like bet_value

            # Process each event from rules against the log entry
            rule_set.match_and_validate_event(log, context, self.log_history)

    def extract_context(self, log, context):
        # Extract bet_value from log entry like "bet_value = 20"
        bet_match = re.search(r'bet_value\s*=\s*(\d+)', log)
        if bet_match:
            context['bet_value'] = int(bet_match.group(1))
            print(f"Extracted bet_value: {context['bet_value']} from log")

        # Extract other values as needed, add to context

    def report_failure(self, unmatched, log_entry):
        unmatched_tuple = tuple(sorted((key, value) for outcome in unmatched for key, value in outcome.items()))
        if unmatched_tuple not in self.errors_reported:  # Avoid duplicate error reporting
            self.errors_reported.add(unmatched_tuple)
            print("Test failed!")
            print(f"Missing expected outcomes: {unmatched}")
            print(f"Context (log excerpt): \n{' '.join(self.log_history[-3:])}")


class RuleSet:
    def __init__(self, xml_file):
        self.events = self.load_rules_from_xml(xml_file)

    def load_rules_from_xml(self, xml_file):
        tree = ET.parse(xml_file)
        root = tree.getroot()

        events = []
        for event in root.findall('event'):
            event_name = event.get('name')
            condition = event.find('condition')
            expected_outcomes = []

            for expected in event.findall('expected'):
                expected_outcomes.append({expected.get('name'): expected.text.strip()})

            events.append({
                'name': event_name,
                'condition': condition.text.strip() if condition is not None else None,
                'expected_outcomes': expected_outcomes
            })

        return events

    def match_and_validate_event(self, log_entry, context, log_history):
        normalized_log_entry = log_entry.strip().lower()

        for event in self.events:
            if event['name'].replace("_", " ").lower() in normalized_log_entry:
                print(f"Processing event: {event['name']}")

                if event['condition']:
                    if not self.evaluate_condition(event['condition'], context):
                        print(f"Condition failed for event: {event['name']}.")
                        return  # Skip event if the condition isn't met
                    print(f"Condition passed for event: {event['name']}")

                self.validate_log(log_entry, event['expected_outcomes'], log_history)

    def evaluate_condition(self, condition, context):
        try:
            # Extract variables from context and replace in condition string
            condition_eval = condition
            for var in context:
                condition_eval = condition_eval.replace(var, str(context[var]))

            print(f"Evaluating condition: {condition_eval}")
            return eval(condition_eval)
        except KeyError as e:
            print(f"Condition failed due to missing variable: {e}")
            return False
        except Exception as e:
            print(f"Error evaluating condition: {e}")
            return False

    def validate_log(self, log_entry, expected_outcomes, log_history):
        matched_outcomes = []
        unmatched = expected_outcomes[:]  # Start with all outcomes as unmatched

        # Try to match log entry with expected outcomes
        for expected in expected_outcomes:
            for key, value in expected.items():
                if key.lower() in log_entry.lower() and value.lower() in log_entry.lower():
                    matched_outcomes.append(expected)
                    unmatched.remove(expected)
                    print(f"Matched expected outcome: {expected}")

        # If any unmatched expected outcomes remain, report them
        if unmatched:
            print("Test failed!")
            print(f"Missing expected outcomes: {unmatched}")
            print(f"Context (log excerpt): \n{' '.join(log_history[-3:])}")


def main():
    rule_set = RuleSet("rules.xml")
    log_validator = LogValidator("log.txt")
    log_validator.process_logs(rule_set)


if __name__ == "__main__":
    main()













