# Input and Output with the Serial Monitor

## Taking Input from the Serial Monitor

This example demonstrates how to take input from the Serial Monitor and use it to control the built-in LED and print a response back to the Serial Monitor.

### Example: Controlling LED with Serial Input

This example shows how to control the built-in LED on an Arduino board using commands entered via the Serial Monitor.

```cpp
void setup() {
  // Initialize the serial communication at a baud rate of 9600
  Serial.begin(9600);
  // Set the built-in LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  // Print instructions to the Serial Monitor
  Serial.println("Enter 'ON' to turn on the LED.");
  Serial.println("Enter 'OFF' to turn off the LED.");
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming data as a string
    String input = Serial.readString();

    // Remove any whitespace characters
    input.trim();

    // Check the input value
    if (input.equalsIgnoreCase("ON")) {
      // Turn on the LED
      digitalWrite(LED_BUILTIN, HIGH);
      // Print confirmation message to the Serial Monitor
      Serial.println("LED is ON");
    } else if (input.equalsIgnoreCase("OFF")) {
      // Turn off the LED
      digitalWrite(LED_BUILTIN, LOW);
      // Print confirmation message to the Serial Monitor
      Serial.println("LED is OFF");
    } else {
      // Print error message to the Serial Monitor
      Serial.println("Invalid command. Please enter 'ON' or 'OFF'.");
    }
  }
}
```

### Explanation:

1. **Setup Function**:
   - `Serial.begin(9600);` initializes serial communication at a baud rate of 9600 bits per second.
   - `pinMode(LED_BUILTIN, OUTPUT);` sets the built-in LED pin as an output.
   - `Serial.println("Enter 'ON' to turn on the LED.");` and `Serial.println("Enter 'OFF' to turn off the LED.");` print instructions to the Serial Monitor.

2. **Loop Function**:
   - `if (Serial.available() > 0)` checks if data is available to read.
   - `String input = Serial.readString();` reads the incoming data as a string.
   - `input.trim();` removes any whitespace characters from the input.
   - `if (input.equalsIgnoreCase("ON"))` checks if the input is "ON" and turns on the LED if true.
   - `else if (input.equalsIgnoreCase("OFF"))` checks if the input is "OFF" and turns off the LED if true.
   - `else` prints an error message if the input is invalid.

### Using the Serial Monitor for Input:

1. Upload the sketch to your Arduino board.
2. Open the Serial Monitor by navigating to `Tools > Serial Monitor` in the Arduino IDE.
3. Ensure the baud rate is set to 9600 in the Serial Monitor.
4. Enter "ON" or "OFF" in the Serial Monitor and press "Send" to control the LED.

## Tasks

### 1. Add a Command to Blink the LED

Write a program that adds a command to blink the LED for a specified number of times when entered via the Serial Monitor.

<details>
<summary>Hint 1</summary>
Read the input string and check if it contains the command to blink the LED.
</details>

<details>
<summary>Hint 2</summary>
Extract the number of times to blink the LED from the input string.
</details>

<details>
<summary>Hint 3</summary>
Use a loop to blink the LED the specified number of times.
</details>

<details>
<summary>Solution</summary>

```cpp
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Enter 'ON' to turn on the LED.");
  Serial.println("Enter 'OFF' to turn off the LED.");
  Serial.println("Enter 'BLINK x' to blink the LED x times.");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();

    if (input.equalsIgnoreCase("ON")) {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED is ON");
    } else if (input.equalsIgnoreCase("OFF")) {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("LED is OFF");
    } else if (input.startsWith("BLINK")) {
      int times = input.substring(6).toInt();
      Serial.print("Blinking LED ");
      Serial.print(times);
      Serial.println(" times");
      for (int i = 0; i < times; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
      }
    } else {
      Serial.println("Invalid command. Please enter 'ON', 'OFF', or 'BLINK x'.");
    }
  }
}
```

</details>

### 2. Add a Command to Adjust the Blink Speed

Write a program that allows the user to adjust the blink speed of the LED using a command entered via the Serial Monitor.

<details>
<summary>Hint 1</summary>
Read the input string and check if it contains the command to set the blink speed.
</details>

<details>
<summary>Hint 2</summary>
Extract the speed value from the input string.
</details>

<details>
<summary>Hint 3</summary>
Use the extracted speed value to set the delay time for the blink.
</details>

<details>
<summary>Solution</summary>

```cpp
int blinkSpeed = 500; // Default blink speed in milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Enter 'ON' to turn on the LED.");
  Serial.println("Enter 'OFF' to turn off the LED.");
  Serial.println("Enter 'BLINK x' to blink the LED x times.");
  Serial.println("Enter 'SPEED y' to set the blink speed to y milliseconds.");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();

    if (input.equalsIgnoreCase("ON")) {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED is ON");
    } else if (input.equalsIgnoreCase("OFF")) {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("LED is OFF");
    } else if (input.startsWith("BLINK")) {
      int times = input.substring(6).toInt();
      Serial.print("Blinking LED ");
      Serial.print(times);
      Serial.println(" times");
      for (int i = 0; i < times; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(blinkSpeed);
        digitalWrite(LED_BUILTIN, LOW);
        delay(blinkSpeed);
      }
    } else if (input.startsWith("SPEED")) {
      blinkSpeed = input.substring(6).toInt();
      Serial.print("Blink speed set to ");
      Serial.print(blinkSpeed);
      Serial.println(" milliseconds");
    } else {
      Serial.println("Invalid command. Please enter 'ON', 'OFF', 'BLINK x', or 'SPEED y'.");
    }
  }
}
```

</details>

### 3. Add a Command to Report the Current LED State and Blink Speed

Write a program that reports the current state of the LED and the blink speed when a specific command is entered via the Serial Monitor.

<details>
<summary>Hint 1</summary>
Read the input string and check if it contains the command to report the status.
</details>

<details>
<summary>Hint 2</summary>
Use variables to keep track of the LED state and blink speed.
</details>

<details>
<summary>Hint 3</summary>
Print the current LED state and blink speed to the Serial Monitor.
</details>

<details>
<summary>Solution</summary>

```cpp
int blinkSpeed = 500; // Default blink speed in milliseconds
bool ledState = LOW; // Current state of the LED

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Enter 'ON' to turn on the LED.");
  Serial.println("Enter 'OFF' to turn off the LED.");
  Serial.println("Enter 'BLINK x' to blink the LED x times.");
  Serial.println("Enter 'SPEED y' to set the blink speed to y milliseconds.");
  Serial.println("Enter 'STATUS' to report the current LED state and blink speed.");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();

    if (input.equalsIgnoreCase("ON")) {
      ledState = HIGH;
      digitalWrite(LED_BUILTIN, ledState);
      Serial.println("LED is ON");
    } else if (input.equalsIgnoreCase("OFF")) {
      ledState = LOW;
      digitalWrite(LED_BUILTIN, ledState);
      Serial.println("LED is OFF");
    } else if (input.startsWith("BLINK")) {
      int times = input.substring(6).toInt();
      Serial.print("Blinking LED ");
      Serial.print

(times);
      Serial.println(" times");
      for (int i = 0; i < times; i++) {
        ledState = HIGH;
        digitalWrite(LED_BUILTIN, ledState);
        delay(blinkSpeed);
        ledState = LOW;
        digitalWrite(LED_BUILTIN, ledState);
        delay(blinkSpeed);
      }
    } else if (input.startsWith("SPEED")) {
      blinkSpeed = input.substring(6).toInt();
      Serial.print("Blink speed set to ");
      Serial.print(blinkSpeed);
      Serial.println(" milliseconds");
    } else if (input.equalsIgnoreCase("STATUS")) {
      Serial.print("LED is ");
      Serial.println(ledState ? "ON" : "OFF");
      Serial.print("Blink speed is ");
      Serial.print(blinkSpeed);
      Serial.println(" milliseconds");
    } else {
      Serial.println("Invalid command. Please enter 'ON', 'OFF', 'BLINK x', 'SPEED y', or 'STATUS'.");
    }
  }
}
```

</details>

These tasks will help you explore more ways to interact with the Arduino via the Serial Monitor, adding commands to control and get feedback from the Arduino, and building more complex and interactive projects.