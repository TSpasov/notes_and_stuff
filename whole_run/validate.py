import json
import re

# Function to extract variables from the conditions
def extract_variables(rules_file_path):
    variables = {}
    valid_operators = ['>', '<', '==', 'AND', 'OR', '<=', '>=', '+', '-', '*', '/']
    logical_constants = ['TRUE', 'FALSE']
    try:
        with open(rules_file_path, 'r') as file:
            rules_data = json.load(file)
            for event in rules_data["events"]:
                if "condition" in event:
                    condition = event["condition"]
                    # Extract variable names using regular expressions
                    variable_names = re.findall(r'\b\w+\b', condition)
                    # Filter out valid operators and logical constants
                    variable_names = [name for name in variable_names if name not in valid_operators and not name.isdigit() and name not in logical_constants]
                    # Add variables to the dictionary with initial value None
                    for name in variable_names:
                        variables[name] = None
    except FileNotFoundError:
        print(f"Error: The file {rules_file_path} was not found.")
    except json.JSONDecodeError:
        print(f"Error: The file {rules_file_path} is not a valid JSON file.")
    return variables

# Function to extract event names from the rules file
def extract_event_names(rules_file_path):
    events = []
    try:
        with open(rules_file_path, 'r') as file:
            rules_data = json.load(file)
            for event in rules_data["events"]:
                events.append(event["name"])
    except FileNotFoundError:
        print(f"Error: The file {rules_file_path} was not found.")
    except json.JSONDecodeError:
        print(f"Error: The file {rules_file_path} is not a valid JSON file.")
    return events

# Function to read and print the rules file
def read_and_print_rules_file(rules_file_path):
    try:
        with open(rules_file_path, 'r') as file:
            rules_data = json.load(file)
            print("Rules File Content:")
            print(json.dumps(rules_data, indent=4))
    except FileNotFoundError:
        print(f"Error: The file {rules_file_path} was not found.")
    except json.JSONDecodeError:
        print(f"Error: The file {rules_file_path} is not a valid JSON file.")

# Function to read and print the log file
def read_and_print_log_file(log_file_path):
    try:
        with open(log_file_path, 'r') as file:
            log_data = file.readlines()
            print("\nLog File Content:")
            for line in log_data:
                print(line.strip())
    except FileNotFoundError:
        print(f"Error: The file {log_file_path} was not found.")

# Function to extract and print event names from the rules file
def extract_and_print_event_names(rules_file_path):
    try:
        with open(rules_file_path, 'r') as file:
            rules_data = json.load(file)
            print("Event Names:")
            for event in rules_data["events"]:
                print(f"event -> {event['name']}")
    except FileNotFoundError:
        print(f"Error: The file {rules_file_path} was not found.")
    except json.JSONDecodeError:
        print(f"Error: The file {rules_file_path} is not a valid JSON file.")


def parse_log_file(log_file_path, events):
    event_scopes = {}
    current_event = None
    current_scope = []

    try:
        with open(log_file_path, 'r') as file:
            for line in file:
                line = line.strip()
                if line.startswith('['):
                    # Extract the event name from the log line
                    event_name = line.split('::AT: ')[1].strip()
                    if event_name in events:
                        # If the event is in the events list, start a new scope
                        if len(current_scope) != 0:
                              current_scope.append(line)

                        if current_event:
                            event_scopes[current_event] = current_scope
                        current_event = event_name
                        current_scope = []
                    else:
                        # If the event is not in the events list, add the line to the current scope
                        current_scope.append(line)
            # Add the last event scope to the dictionary
            if current_event:
                event_scopes[current_event] = current_scope
    except FileNotFoundError:
        print(f"Error: The file {log_file_path} was not found.")

    # Print the event scopes
    for event, scope in event_scopes.items():
        print(f"Event: {event}")
        print("Scope:")
        for line in scope:
            print(line)
        print()



# def parse_log_file(log_file_path, events):
#     event_scopes = {}
#     current_event = None
#     current_scope = []

#     try:
#         with open(log_file_path, 'r') as file:
#             for line in file:
#                 line = line.strip()
#                 if line.startswith('['):
#                     # Extract the event name from the log line
#                     event_name = line.split('::AT: ')[1].strip()
#                     if current_event:
#                         # If there is a current event, start a new scope
#                         event_scopes[current_event] = current_scope
#                     current_event = event_name
#                     current_scope = []
#                 # Add the line to the current scope
#                 current_scope.append(line)
#             # Add the last event scope to the dictionary
#             if current_event:
#                 event_scopes[current_event] = current_scope
#     except FileNotFoundError:
#         print(f"Error: The file {log_file_path} was not found.")

#     # Print the event scopes
#     for event, scope in event_scopes.items():
#         print(f"Event: {event}")
#         print("Scope:")
#         for line in scope:
#             print(line)
#         print()

def update_variables(log_file_path, variables):
    try:
        with open(log_file_path, 'r') as file:
            for line in file:
                line = line.strip()
                # Remove the timestamp and the "h1::AT:" prefix from the log line
                if '::AT: ' in line:
                    event_name = line.split('::AT: ')[1].strip()
                    # Check if the line contains a variable assignment
                    if '=' in event_name:
                        assignments = event_name.split(',')
                        for assignment in assignments:
                            variable_name, variable_value = assignment.split('=')
                            variable_name = variable_name.strip()
                            variable_value = variable_value.strip()
                            # Update the variable value in the variables dictionary
                            variables[variable_name] = variable_value
    except FileNotFoundError:
        print(f"Error: The file {log_file_path} was not found.")


# Main function
def main():
    rules_file_path = 'rules.json'  # Replace with the actual path to your rules file
    log_file_path = 'log_json.txt'      # Replace with the actual path to your log file

    read_and_print_rules_file(rules_file_path)
    read_and_print_log_file(log_file_path)

    events = extract_event_names(rules_file_path)
    print("Event Names:")
    for event in events:
        print(event)


    variables = extract_variables(rules_file_path)
    print("Variables:")
    for name, value in variables.items():
        print(f"{name}: {value}")


    parse_log_file(log_file_path, events)

    update_variables(log_file_path, variables)

    # Print the updated variables dictionary
    print("Updated Variables:")
    for name, value in variables.items():
        print(f"{name}: {value}")
    
if __name__ == "__main__":
    main()

