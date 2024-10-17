import ast

def evaluate_expression(expression):
    try:
        # Split the input string into variable definitions and the expression
        parts = expression.split(",")
        variables = parts[:-1]
        expr = parts[-1].strip()

        # Dictionary to store variables
        var_dict = {}

        # Assign variables from the string
        for var in variables:
            var_name, var_value = var.split("=")
            var_name = var_name.strip()
            # Safely evaluate the value using ast.literal_eval
            var_value = ast.literal_eval(var_value.strip())
            var_dict[var_name] = var_value

        # Safely evaluate the expression within the context of the variables
        result = eval(expr, {}, var_dict)
        return result
    except Exception as e:
        return str(e)

# Test the function
expressions = [
    "c = 100.0, a = 500.0, a == c / 2",
    "x = 10.5, y = 20.3, x + y == 30.8",
    "a = True, b = False, a and b",
    "p = 5.2, q = 10.1, p < q",
    "r = 10.0, s = 3.0, r % s == 1.0",
    "a = 23, b = 12, c = 6, a + b - c == 27"
]

for expression in expressions:
    result = evaluate_expression(expression)
    print(f"{expression} => {result}")

