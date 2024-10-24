import ast

def evaluate_expression(expression, var_dict):
    try:
        # Split the input string into variable definitions and the expression
        parts = expression.split(",")
        expr = parts[-1].strip()

        # Safely evaluate the expression within the context of the variables
        result = eval(expr, {}, var_dict)
        return result
    except Exception as e:
        return str(e)

# Test the function
variables = {
    "c": 100.0,
    "a": 500.0,
    "x": 10.5,
    "y": 20.3,
    "p": 5.2,
    "q": 10.1,
    "r": 10.0,
    "bet_value": 0,
    "s": 3.0,
}

expressions = [
    "a == c / 2",
    "x + y == 30.8",
    "a and b",
    "p < q",
    "r % s == 1.0",
    "a + b - c == 27"
]

for expression in expressions:
    result = evaluate_expression(expression, variables)
    print(f"{expression} => {result}")

