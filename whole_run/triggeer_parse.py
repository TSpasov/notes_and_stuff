import re

def parse_script(script):
    lines = script.splitlines()
    trigger = None
    conditions = []
    final_response = []

    for line in lines:
        line = line.strip()
        if line.startswith("comment:"):
            continue
        elif line.startswith("trigger"):
            trigger = line.split("->", 2)[1].strip()
        elif line.startswith("if"):
            condition = line.split(" ", 1)[1].strip()
            response = []
            i = lines.index(line)
            while i + 1 < len(lines) and not lines[i + 1].startswith("elif") and not lines[i + 1].startswith("else") and not lines[i + 1].startswith("response:"):
                if lines[i + 1].startswith("-"):
                    response.append(lines[i + 1].strip()[2:])  # Remove the leading "- "
                i += 1
            conditions.append((condition, response))
        elif line.startswith("elif"):
            condition = line.split(" ", 1)[1].strip()
            response = []
            i = lines.index(line)
            while i + 1 < len(lines) and not lines[i + 1].startswith("else:") and not lines[i + 1].startswith("response:"):
                if lines[i + 1].startswith("-"):
                    response.append(lines[i + 1].strip()[2:])  # Remove the leading "- "
                i += 1
            conditions.append((condition, response))
        elif line.startswith("else:"):
            response = []
            i = lines.index(line)
            while i + 1 < len(lines) and not lines[i + 1].startswith("response:"):
                if lines[i + 1].startswith("-"):
                    response.append(lines[i + 1].strip()[2:])  # Remove the leading "- "
                i += 1
            conditions.append((None, response))
        elif line.startswith("response:"):
            final_response = []
            i = lines.index(line)
            while i + 1 < len(lines) and lines[i + 1].startswith("-"):
                final_response.append(lines[i + 1].strip()[2:])  # Remove the leading "- "
                i += 1

    return trigger, conditions, final_response

def evaluate_condition(condition, context):
    # Replace variable names with context values
    for var, value in context.items():
        condition = condition.replace(var, repr(value))
    return eval(condition)

def evaluate_script(trigger, conditions, context, final_response):
    output = [f"Trigger: {trigger}"]

    for condition, response in conditions:
        if condition is None:
            output.extend(response)
            break
        try:
            if evaluate_condition(condition, context):
                output.extend(response)
                break
        except Exception as e:
            print(f"Error evaluating condition: {e}")
            break

    output.extend(final_response)

    for line in output:
        print(line)

# Example usage
script = """
comment: "This is the new syntax "
trigger-> Input: start_button_pressed
if is_dolly_enabled == True:
    response:
        - Sound played: bet_placed_sound
        - Animation started: bet_confirmation
        - State changed: dealing
elif bet_value == 0:
    response:
        - Sound played: bet_placed_sound
        - Sound played: bet_error_sound
        - Animation started: bet_again
else:
    response:
        - Sound played: error

response:
    - Sound played: gggo
    - Animation started: ooo
"""

context = {
    "is_dolly_enabled": False,
    "bet_value": 0
}

trigger, conditions, final_response = parse_script(script)
evaluate_script(trigger, conditions, context, final_response)