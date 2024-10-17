import json

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

# Main function
def main():
    rules_file_path = 'rules.json'  # Replace with the actual path to your rules file
    log_file_path = 'log_json.txt'      # Replace with the actual path to your log file

    read_and_print_rules_file(rules_file_path)
    read_and_print_log_file(log_file_path)

    rules_file_path = 'rules.json'  # Replace with the actual path to your rules file

    extract_and_print_event_names(rules_file_path)
    
if __name__ == "__main__":
    main()

