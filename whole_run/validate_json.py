import json
import re

class RuleSet:
    def __init__(self, rules_file):
        self.rules_file = rules_file
        self.rules = self.load_rules()

    def load_rules(self):
        with open(self.rules_file, 'r') as file:
            rules = json.load(file)
        print(f"Loaded {len(rules['events'])} events.")
        for event in rules['events']:
            print(f"Event data: {event}")
        return rules

    def extract_events(self):
        return self.rules['events']

class LogParser:
    def __init__(self, log_file, rules):
        self.log_file = log_file
        self.log_lines = self.load_log_file()
        self.rules = rules
        self.event_patterns = self.generate_event_patterns()

    def load_log_file(self):
        # Load the log file into a list of lines
        with open(self.log_file, 'r') as file:
            log_lines = file.readlines()
        
        # Debugging: print the loaded log lines
        print(f"Loaded {len(log_lines)} lines from log file.")
        for line in log_lines:
            print(line.strip())  # Strip extra spaces for clean output
        return log_lines

    def generate_event_patterns(self):
        # Generate event patterns dynamically from rules
        event_patterns = {}
        for event in self.rules.extract_events():
            event_name = event['name']
            # For event name mapping: convert underscores to spaces (if necessary)
            event_patterns[event_name] = re.escape(event_name).replace("\\_", " ")  
        return event_patterns

    def extract_event_scopes(self):
        event_scopes = []
        events = self.rules.extract_events()
        if not events:
            print("No events to process.")
            return None
        
        # Current scope
        current_scope = []
        current_event = None
        
        for log_line in self.log_lines:
            # Check if the log line marks a new event
            for event_name, pattern in self.event_patterns.items():
                if event_name.lower() in log_line.lower():
                    print(f"Matched event '{event_name}' in log line: {log_line.strip()}")
                    # If there was a previous event, save its scope
                    if current_event:
                        event_scopes.append((current_event, current_scope))
                    # Start a new scope for the new event
                    current_event = event_name
                    current_scope = [log_line]
                    break
            else:
                # Add log lines to the current event's scope
                if current_event:
                    current_scope.append(log_line)
        
        # Capture the final event's scope
        if current_event:
            event_scopes.append((current_event, current_scope))
        
        # Print the extracted event scopes for debugging
        print(f"Extracted {len(event_scopes)} event scopes:")
        for event, scope in event_scopes:
            print(f"Event: {event}, Scope: {scope}")
        
        return event_scopes if event_scopes else None

class EventValidator:
    def __init__(self, event_scopes, context, rules):
        self.event_scopes = event_scopes
        self.context = context
        self.rules = rules

    def search_variables(self, scope):
        # Extract variables like bet_value, credit, etc. from the scope
        variables_to_extract = self.extract_variable_names()

        for log in scope:
            for variable in variables_to_extract:
                pattern = rf'{variable}\s*=\s*(\d+)'
                match = re.search(pattern, log)
                if match:
                    self.context[variable] = int(match.group(1))
                    print(f"Extracted {variable}: {self.context[variable]}")

        return self.context

    def extract_variable_names(self):
        # Extract all variable names used in conditions from the rules.json
        variable_names = set()
        for event in self.rules.extract_events():
            if 'condition' in event:
                condition = event['condition']
                variables_in_condition = re.findall(r'\b\w+\b', condition)
                variable_names.update(variables_in_condition)
        print(f"Variables to extract: {variable_names}")
        return variable_names

    def evaluate_condition(self, condition):
        # Replace variable names in condition with actual values
        try:
            condition_evaluated = condition
            for var in self.context.keys():
                condition_evaluated = condition_evaluated.replace(var, str(self.context[var]))
            print(f"Evaluating condition: {condition_evaluated}")
            return eval(condition_evaluated)
        except Exception as e:
            print(f"Error evaluating condition: {str(e)}")
            return False

    def process_events(self):
        if not self.event_scopes:
            print("No events found in rules.")
            return
        for event, scope in self.event_scopes:
            print(f"Processing event: {event}")
            # Extract variables within the scope
            self.search_variables(scope)
            # Check if the event has a condition to evaluate
            condition = self.get_event_condition(event)
            if condition:
                if self.evaluate_condition(condition):
                    print(f"Condition passed for event: {event}")
                    self.match_expected_outcomes(event, scope)
                else:
                    print(f"Condition failed for event: {event}")
                    self.match_else_outcomes(event, scope)
            else:
                # No condition, just match the expected outcomes
                self.match_expected_outcomes(event, scope)

    def get_event_condition(self, event_name):
        for event in self.rules.extract_events():
            if event['name'] == event_name and 'condition' in event:
                return event['condition']
        return None

    def match_expected_outcomes(self, event_name, scope):
        for event in self.rules.extract_events():
            if event['name'] == event_name:
                expected = event.get('expected', [])
                for exp in expected:
                    if not any(exp in log for log in scope):
                        print(f"Test failed! Missing expected outcomes: {expected}")
                        print(f"Context (log excerpt): {' '.join(scope)}")
                    else:
                        print(f"Matched expected outcome in log scope: {exp}")

    def match_else_outcomes(self, event_name, scope):
        for event in self.rules.extract_events():
            if event['name'] == event_name and 'else' in event:
                else_outcomes = event['else'].get('expected', [])
                for outcome in else_outcomes:
                    if not any(outcome in log for log in scope):
                        print(f"Test failed! Missing else outcomes: {else_outcomes}")
                        print(f"Context (log excerpt): {' '.join(scope)}")
                    else:
                        print(f"Matched else outcome in log scope: {outcome}")

def main():
    rule_set = RuleSet("rules.json")  # Replace with your rules.json file
    log_parser = LogParser("log_json.txt", rule_set)  # Replace with your log.txt file
    
    # Extract events and log scopes
    event_scopes = log_parser.extract_event_scopes()
    
    # Initialize the context (empty for now)
    context = {}
    
    # Create EventValidator and process events
    event_validator = EventValidator(event_scopes, context, rule_set)
    event_validator.process_events()

if __name__ == "__main__":
    main()
