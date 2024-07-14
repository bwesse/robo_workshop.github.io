# Example Markdown Page

This is an example page demonstrating how to use code annotations and copyable code blocks with Material for MkDocs.

## Annotated Code Block Example

In this section, we'll show how to annotate code to provide explanations for specific lines.

``` python title="greet.py" linenums="1" hl_lines="2 5"
# Import necessary libraries
import time  # (1)

# Function to greet the user
def greet(name):  # (2)
    print(f"Hello, {name}!")  # (3)

# Call the function with a name
greet("Robotics Enthusiast")  # (4)
time.sleep(2)  # (5) Pause for 2 seconds before ending the script
```