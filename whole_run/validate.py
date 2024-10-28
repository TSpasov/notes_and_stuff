import json
import ast
import re


AT_PREFIX = '::AutomationTest:'

# Function to extract variables from the conditions
def extract_variables(rules_file_path):
    variables = {}
    valid_operators = ['>', '<', '==', 'and', 'or', '<=', '>=', '+', '-', '*', '/']
    logical_constants = ['TRUE', 'FALSE']
    try:
        with open(rules_file_path, 'r') as file:
            rules_data = json.load(file)
            for event in rules_data["rules"]:
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

# Function to extract variables from the conditions
def read_variables(rules_file_path):
    variables = {}
    try:
        with open(rules_file_path, 'r') as file:
            rules_data = json.load(file)
            variables = rules_data.get("vars", {})
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
            for event in rules_data["rules"]:
                events.append(event["trigger"])
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
            for event in rules_data["rules"]:
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
                if AT_PREFIX in line:
                    # Extract the event name from the log line
                    event_name = line.split(AT_PREFIX)[1].strip()
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
        print(f"Trigger: {event}")
        print("Scope:")
        for line in scope:
            print(line)
        print()

def update_variables(log_file_path, variables):
    try:
        with open(log_file_path, 'r') as file:
            for line in file:
                line = line.strip()
                # Remove the timestamp and the "h1::AT:" prefix from the log line
                if AT_PREFIX in line:
                    event_name = line.split(AT_PREFIX)[1].strip()
                    # Check if the line contains a variable assignment
                    if ':' in event_name:
                        assignments = event_name.split(',')
                        for assignment in assignments:
                            variable_name, variable_value = assignment.split(':')
                            variable_name = variable_name.strip()
                            variable_value = variable_value.strip()
                            # Update the variable value in the variables dictionary
                            variables[variable_name] = variable_value
    except FileNotFoundError:
        print(f"Error: The file {log_file_path} was not found.")

def read_variable(logEntry, variables):
    assignments = logEntry.split(',')
    for assignment in assignments:
        variable_name, variable_value = assignment.split(':')
        variable_name = variable_name.strip()
        variable_value = variable_value.strip()
        if variable_name in variables:
            variables[variable_name] = variable_value

def evaluate_expression(expression, variables):
    try:
        # Safely evaluate the expression within the context of the variables
        result = eval(expression, {}, variables)
        return result
    except Exception as e:
        print(e)
        return False



def process_json_logic(file_path, variables):
    try:
        # Read the JSON data from the file
        with open(file_path, 'r') as file:
            json_data = file.read()
        
        # Check if the file is empty
        if not json_data:
            print("Error: The file is empty.")
            return
        
        # Load JSON data
        try:
            data = json.loads(json_data)
        except json.JSONDecodeError as e:
            print(f"Error: Invalid JSON data - {e}")
            return
        
        # Extract variable types from the data section
        variable_types = data.get("vars", {})
        
        # Convert variables to their appropriate types
        for var_name, var_value in variables.items():
            var_type = variable_types.get(var_name)
            if var_type == "int":
                variables[var_name] = int(var_value) if var_value is not None else None
            elif var_type == "float":
                variables[var_name] = float(var_value) if var_value is not None else None
            elif var_type == "str":
                variables[var_name] = str(var_value) if var_value is not None else None
            elif var_type == "bool":
                variables[var_name] = bool(var_value) if var_value is not None else None
        
        # Get the trigger from the variables dictionary
        trigger = variables.pop("trigger", None)
        
        # Iterate through rules
        for rule in data.get("rules", []):
            # Check trigger
            if rule.get("trigger") == trigger:
                # Check conditions if present
                print(f"\nEvaluating trigger: {trigger}\n")  # Debug print
                if "conditions" in rule:
                    for condition in rule["conditions"]:
                        # Substitute variables in the condition string
                        condition_str = condition["condition"].format(**variables)
                        print(f"Evaluating condition: {condition_str}")  # Debug print
                        try:
                            # Evaluate condition using the custom evaluation function
                            if evaluate_expression(condition_str, variables):
                                # Print responses
                                for response in condition["response"]:
                                    print(response)
                                break
                        except Exception as e:
                            print(f"Error evaluating condition '{condition_str}': {e}")
                    else:
                        # If no conditions are met, execute else response
                        for response in rule.get("else", {}).get("response", []):
                            print(response)
                else:
                    # If no conditions, execute response directly
                    for response in rule.get("response", []):
                        print(response)
                
                # Execute unconditional response
                for response in rule.get("unconditional_response", []):
                    print(response)

    except FileNotFoundError:
        print(f"Error: The file {file_path} does not exist.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

#//
def process_log(log_file_path, variables, triggers):
    try:
        with open(log_file_path, 'r') as file:
            for line in file:
                line = line.strip()
                # Remove the timestamp and the "h1::AT:" prefix from the log line
                if AT_PREFIX in line:
                    logEntry = line.split(AT_PREFIX)[1].strip()
                    if '=' in logEntry:
                        read_variable(logEntry, variables)
                    elif logEntry in triggers:
                        print(f"Trigger: {logEntry}")
                    else:
                        print(f"logEntry: {logEntry}")

    except FileNotFoundError:
        print(f"Error: The file {log_file_path} was not found.") 
#//
# Main function
def main():
    rules_file_path = 'rules.json'  # Replace with the actual path to your rules file
    log_file_path = 'roulette.log'      # Replace with the actual path to your log file

    # read_and_print_rules_file(rules_file_path)
    # read_and_print_log_file(log_file_path)

    triggers = extract_event_names(rules_file_path)
    print("trigger Names:")
    for trigger in triggers:
        print(trigger)

    variables = extract_variables(rules_file_path)
    for name, value in variables.items():
        print("Variables")
        print(f"{name}: {value}")
        
    # process_log(log_file_path, variables, triggers)
    # for name, value in variables.items():
    #     print(f"{name}: {value}")

    # parse_log_file(log_file_path, triggers)

    # update_variables(log_file_path, variables)

    # # Print the updated variables dictionary
    # print("Updated Variables:")


    # print("Conditions:\n")
 


    # for trigger in triggers:
    #     process_json_logic(rules_file_path, {"trigger": trigger, **variables})

    
    
if __name__ == "__main__":
    main()

