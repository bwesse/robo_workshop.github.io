# Example Markdown Page


=== "C"

    ``` c
    #include <stdio.h>

    int main(void) {
      printf("Hello world!\n");
      return 0;
    }
    ```

=== "C++"

    ``` c++
    #include <iostream>

    int main(void) {
      std::cout << "Hello world!" << std::endl;
      return 0;
    }
    ```


# Example Markdown Page


??? question "How to set a page template for an entire folder?"

    With the help of the [built-in meta plugin], you can set a custom template
    for an entire section and all nested pages, by creating a `.meta.yml` file
    in the corresponding folder with the following content:

    ``` yaml
    template: custom.html
    ```

  [theme extension]: ../customization.md#extending-the-theme
  [built-in meta plugin]: ../plugins/meta.md


This is an example page demonstrating how to use code annotations and copyable code blocks with Material for MkDocs.

## Annotated Code Block Example

In this section, we'll show how to annotate code to provide explanations for specific lines.

```python hl_lines="2 5"
# Import necessary libraries
import time  # (1)

# Function to greet the user
def greet(name):  # (2)
    print(f"Hello, {name}!")  # (3)

# Call the function with a name
greet("Robotics Enthusiast")  # (4)
time.sleep(2)  # (5) Pause for 2 seconds before ending the script
```

1. `import time`: The `time` module is imported to use the `sleep` function.
2. `def greet(name):`: A function named `greet` is defined, which takes one parameter, `name`.
3. `print(f"Hello, {name}!")`: This line prints a greeting message, using an f-string for formatting.
4. `greet("Robotics Enthusiast")`: The `greet` function is called with the argument `"Robotics Enthusiast"`.
5. `time.sleep(2)`: The script pauses for 2 seconds before ending.

## Copyable Code Block Example

The following code block allows you to copy the code easily by clicking the copy button.

```python
# Example function to add two numbers
def add(a, b):
    return a + b

# Call the function with example numbers
result = add(5, 7)
print(result)  # This should print 12
```

You can click the copy button on the top right corner of the code block to copy the code to your clipboard.

## Conclusion

By utilizing the features provided by Material for MkDocs, you can enhance your documentation with annotated code blocks and make code snippets easily copyable. This improves the readability and usability of your documentation.



### Explanation:

1. **Annotated Code Block**: The first code block demonstrates how to use annotations to explain each line of the code. The `hl_lines` attribute is used to highlight specific lines, and the comments with numbers (e.g., `# (1)`) correspond to the explanations below the code block.
2. **Copyable Code Block**: The second code block shows how to create a simple code block that allows users to copy the code easily. The copy button is automatically provided by the Material theme for MkDocs.

Make sure you have the required features enabled in your `mkdocs.yml` file, specifically `content.code.annotate` and `content.code.copy`, to support annotations and copyable code blocks.