import re

def parse_script(script):
    # Regular expressions to extract different sections
    comment_pattern = re.compile(r'\?.*')
    trigger_pattern = re.compile(r'begin_trigger\s+Input:\s+(.*?)\s+end_trigger', re.DOTALL)
    condition_pattern = re.compile(r'begin_condition\s+(.*?)\s+end_condtion', re.DOTALL)
    unconditional_pattern = re.compile(r'begin_unconditional\s+(.*?)\s+end_unconditional', re.DOTALL)
    
    # Extract comments
    comments = comment_pattern.findall(script)
    
    # Extract trigger
    trigger_match = trigger_pattern.search(script)
    trigger = trigger_match.group(1).strip() if trigger_match else None
    
    # Extract conditions
    conditions = condition_pattern.findall(script)
    
    # Extract unconditional actions
    unconditional_match = unconditional_pattern.search(script)
    unconditional = unconditional_match.group(1).strip() if unconditional_match else None
    
    return {
        'comments': comments,
        'trigger': trigger,
        'conditions': conditions,
        'unconditional': unconditional
    }

def evaluate_condition_block(condition_block, context):
    # Regular expressions to extract if, elif, and else conditions
    if_pattern = re.compile(r'if\s+(.*?)\s+', re.DOTALL)
    elif_pattern = re.compile(r'elif\s+(.*?)\s+', re.DOTALL)
    else_pattern = re.compile(r'else\s+', re.DOTALL)
    
    # Extract if condition
    if_match = if_pattern.search(condition_block)
    if_condition = if_match.group(1).strip() if if_match else None
    
    # Extract elif conditions
    elif_matches = elif_pattern.findall(condition_block)
    elif_conditions = [match.strip() for match in elif_matches]
    
    # Extract else condition (represented as None)
    else_match = else_pattern.search(condition_block)
    else_condition = None if else_match else None
    
    # Evaluate if condition
    if if_condition:
        if_result = eval(if_condition, context)
        print(f"Condition: {if_condition} ==> {if_result}")
        if if_result:
            return
    
    # Evaluate elif conditions
    for elif_condition in elif_conditions:
        elif_result = eval(elif_condition, context)
        print(f"Condition: {elif_condition} ==> {elif_result}")
        if elif_result:
            return
    
    # Evaluate else condition
    if else_condition is None:
        print("Condition: else ==> true")
    else:
        print("Condition: else ==> false")

def evaluate_script(script, context):
    parsed_script = parse_script(script)
    
    print("Comments:")
    for comment in parsed_script['comments']:
        print(f"  {comment.strip()}")
    
    print("\nTrigger:")
    print(f"  {parsed_script['trigger']}")
    
    print("\nConditions:")
    for condition_block in parsed_script['conditions']:
        print(f"  begin_condition\n  {condition_block.strip()}\n  end_condtion")
        evaluate_condition_block(condition_block, context)
    
    print("\nUnconditional Actions:")
    if parsed_script['unconditional']:
        actions = parsed_script['unconditional'].split('\n')
        for action in actions:
            print(f"  {action.strip()}")

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
if rrr == True
        - rrr everyone
else
        - rr is not true
end_condtion

begin_unconditional
    - Sound played: gggo
    - Animation started: ooo
end_unconditional

end_trigger
"""

context = {
    "is_dolly_enabled": False,
    "bet_value": 0,
    "rrr": True
}

evaluate_script(script, context)