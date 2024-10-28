
def parse_script(script):
    # Initialize the data structure
    parsed_script = []
    
    # Split the script into lines
    lines = script.split('\n')
    
    # Initialize variables to track the current section
    current_trigger = None
    current_condition_block = None
    current_unconditional_block = None
    current_condition = None
    current_responses = []
    
    for line in lines:
        line = line.strip()
        
        # Skip empty lines and comments
        if not line or line.startswith('?'):
            continue
        
        # Check for trigger start
        if line.startswith('begin_trigger'):
            if current_trigger:
                # If there's an existing trigger, finalize it
                finalize_trigger(parsed_script, current_trigger, current_condition_block, current_unconditional_block)
            current_trigger = {'name': line.split('Input: ')[1], 'body': {'conditions': [], 'unconditional': []}}
            current_condition_block = []
            current_unconditional_block = []
        
        # Check for condition start
        elif line.startswith('begin_condition'):
            current_condition = None
            current_responses = []
        
        # Check for condition end
        elif line.startswith('end_condtion'):
            if current_condition:
                current_condition_block.append(current_condition)
            current_condition = None
            current_responses = []
        
        # Check for unconditional start
        elif line.startswith('begin_unconditional'):
            current_unconditional_block = []
        
        # Check for unconditional end
        elif line.startswith('end_unconditional'):
            current_trigger['body']['unconditional'] = current_unconditional_block
            current_unconditional_block = []
        
        # Check for trigger end
        elif line.startswith('end_trigger'):
            finalize_trigger(parsed_script, current_trigger, current_condition_block, current_unconditional_block)
            current_trigger = None
            current_condition_block = None
            current_unconditional_block = None
        
        # Check for condition lines
        elif line.startswith('if') or line.startswith('elif'):
            if current_condition:
                current_condition_block.append(current_condition)
            condition_key = line
            condition_expression = ' '.join(line.split()[1:])  # Extract the full condition expression
            current_condition = {condition_key: {'condition': condition_expression, 'resolution': None, 'responses': []}}
        
        # Check for else line
        elif line.startswith('else'):
            if current_condition:
                current_condition_block.append(current_condition)
            condition_key = line
            condition_expression = 'else'  # Set the condition expression to 'else'
            current_condition = {condition_key: {'condition': condition_expression, 'resolution': None, 'responses': []}}
        
        # Check for response lines
        elif line.startswith('-'):
            if current_condition:
                current_condition[list(current_condition.keys())[0]]['responses'].append(line[2:])
            elif current_unconditional_block is not None:
                current_unconditional_block.append({'condition': 'True', 'resolution': None, 'responses': [line[2:]]})
    
    # Finalize the last trigger if any
    if current_trigger:
        finalize_trigger(parsed_script, current_trigger, current_condition_block, current_unconditional_block)
    
    return parsed_script

def finalize_trigger(parsed_script, current_trigger, current_condition_block, current_unconditional_block):
    if current_condition_block:
        current_trigger['body']['conditions'] = current_condition_block
    if current_unconditional_block:
        current_trigger['body']['unconditional'] = current_unconditional_block
    parsed_script.append(current_trigger)


def evaluate_conditions(parsed_script, context):
    for trigger in parsed_script:
        any_condition_true = False  # Initialize a flag to track if any condition is true
        for condition_block in trigger['body']['conditions']:
            for condition_key, condition_data in condition_block.items():
                if condition_key != 'unconditional':
                    if condition_key.startswith('if '):
                        any_condition_true = False
                        try:
                            condition_data['resolution'] = str(eval(condition_data['condition'], context))
                            if condition_data['resolution'] == 'True':
                                any_condition_true = True  # Set the flag to True if the condition is True
                        except SyntaxError:
                            # Handle cases where the condition is 'else'
                            condition_data['resolution'] = 'False'
                    if condition_key.startswith('elif '):
                        # Set resolution to 'false' if any previous condition in the block is true
                        if any_condition_true:
                            condition_data['resolution'] = 'False'
                        else:
                            try:
                                condition_data['resolution'] = str(eval(condition_data['condition'], context))
                                if condition_data['resolution'] == 'True':
                                    any_condition_true = True  # Set the flag to True if the condition is True
                            except SyntaxError:
                            # Handle cases where the condition is 'else'
                                condition_data['resolution'] = 'False'
                               
                    if condition_key.startswith('else'):
                        if any_condition_true:
                            condition_data['resolution'] = 'False'
                        else:
                            condition_data['resolution'] = 'True'       

        
        # Evaluate unconditional actions
        for unconditional_action in trigger['body']['unconditional']:
            unconditional_action['resolution'] = 'True'




# Example usage
script = """
? this is comment 

begin_trigger Input: start_button_pressed

begin_condition
if is_dolly_enabled == True
        - Sound played: bet_placed_sound
        - Animation started: bet_confirmation
        - State changed: dealing

elif bet_value == 0
        - Sound played: bet_placed_sound
        - Sound played: bet_error_sound
        - Animation started: bet_again
else
        - Sound played: error
end_condtion

begin_condition
if rrr == False
        - rrr everyone
else
        - rr is not true
end_condtion

begin_unconditional
    - Sound played: gggo
    - Animation started: ooo
end_unconditional

end_trigger


? this is comment 

begin_trigger Input: OnMoney in

begin_condition
if credit > 0
        - Sound played: credit_enough

else
        - Sound played: no_credit
end_condtion

begin_condition
if ToatlIn == 10
        - StartBetting
else
        - StopBetting
end_condtion

begin_unconditional
    - Sound played: credit update
end_unconditional

end_trigger
"""
context = {
    "is_dolly_enabled": False,
    "bet_value": 0,
    "rrr": False,
    "credit": 10,
    "ToatlIn": 10
}

parsed_script = parse_script(script)
evaluate_conditions(parsed_script, context)
print(parsed_script)