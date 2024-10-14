# rule_validator.py

class Rule:
    def __init__(self, event, condition, action):
        self.event = event
        self.condition = condition
        self.action = action

    def validate(self, log_entry):
        if self.event == log_entry['event']:
            if self.condition(log_entry):
                return self.action(log_entry)
        return None

class LogParser:
    def parse_log(self, log_file):
        logs = []
        with open(log_file, 'r') as file:
            for line in file:
                # Assuming logs are in key-value pairs, e.g., "event=START, message='Game started'"
                log_entry = self._parse_line(line.strip())
                logs.append(log_entry)
        return logs

    def _parse_line(self, line):
        log_entry = {}
        for pair in line.split(','):
            key, value = pair.split('=')
            log_entry[key.strip()] = value.strip().strip("'\"")
        return log_entry

class Validator:
    def __init__(self, rules):
        self.rules = rules

    def validate_logs(self, logs):
        results = []
        for log in logs:
            for rule in self.rules:
                result = rule.validate(log)
                if result is not None:
                    results.append(result)
        return results

class ActionHandler:
    @staticmethod
    def mark_valid(log_entry):
        return f"Log entry valid: {log_entry}"

    @staticmethod
    def raise_error(log_entry):
        return f"ERROR: Log entry invalid - {log_entry}"

# Define conditions for the rules
def condition_is_start(log_entry):
    return log_entry['message'] == 'Game started'

def condition_is_error(log_entry):
    return log_entry['message'].startswith('ERROR')

# Main execution
if __name__ == "__main__":
    rules = [
        Rule('START', condition_is_start, ActionHandler.mark_valid),
        Rule('ERROR', condition_is_error, ActionHandler.raise_error)
    ]

    log_parser = LogParser()
    logs = log_parser.parse_log('game_logs.txt')  # Path to the log file

    validator = Validator(rules)
    results = validator.validate_logs(logs)

    for result in results:
        print(result)


def condition_is_valid_bet(log_entry):
    return log_entry['message'] == 'Game started'

def condition_is_state_change_allowed(log_entry):
    return log_entry['message'].startswith('ERROR')

# Define rules for the base game
base_game_rules = [
    Rule('BET_PLACED', condition_is_valid_bet, ActionHandler.mark_valid),
    Rule('STATE_CHANGE', condition_is_state_change_allowed, ActionHandler.mark_valid),
    # More rules...
]

def condition_is_bonus_triggered(log_entry):
    return log_entry['message'] == 'Game started'

def condition_is_bonus_start_allowed(log_entry):
    return log_entry['message'].startswith('ERROR')

# Define rules for the bonus game
bonus_game_rules = [
    Rule('BONUS_TRIGGER', condition_is_bonus_triggered, ActionHandler.mark_valid),
    Rule('BONUS_START', condition_is_bonus_start_allowed, ActionHandler.mark_valid),
    # More rules...
]

# Initialize Validator with rules by scenario
rules_by_scenario = {
    'BASE_GAME': base_game_rules,
    'BONUS_GAME': bonus_game_rules
}

validator = Validator(rules_by_scenario)

logs = log_parser.parse_log('game_logs.txt')  # Path to the log file
results = validator.validate_logs(logs)

for result in results:
    print(result)
