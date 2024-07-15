
### Basic Communication with Serial Monitor

This example demonstrates the basic communication with the Serial Monitor by printing any input received.

```cpp
void setup() {
  Serial.begin(9600);  // Initialize the serial communication at a baud rate of 9600
  Serial.println("Send any text and it will be printed here.");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    Serial.print("You sent: ");
    Serial.println(input);
  }
}
```

### Tasks

### 1. Control the LED with Serial Input

Expand the program to allow control of the built-in LED using commands entered via the Serial Monitor.

```cpp
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Enter 'ON' to turn on the LED or 'OFF' to turn it off.");
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
    } else {
      Serial.print("You sent: ");
      Serial.println(input);
    }
  }
}
```

### 2. Add a Command to Blink the LED

Write a program that adds a command to blink the LED a specified number of times when entered via the Serial Monitor.

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
  Serial.println("Enter 'ON' to turn on the LED, 'OFF' to turn it off, or 'BLINK x' to blink the LED x times.");
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
      Serial.print("You sent: ");
      Serial.println(input);
    }
  }
}
```

</details>

### 3. Add a Command to Adjust the Blink Speed

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
  Serial.println("Enter 'ON' to turn on the LED, 'OFF' to turn it off, 'BLINK x' to blink the LED x times, or 'SPEED y' to set the blink speed to y milliseconds.");
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
      Serial.print("You sent: ");
      Serial.println(input);
    }
  }
}
```

</details>

These tasks will help you explore more ways to interact with the Arduino via the Serial Monitor, adding commands to control and get feedback from the Arduino, and building more complex and interactive projects.