#include <math.h>

// Declare variables
float angle = 0.0;
float angleIncrement = 0.1; // Default angle increment
int delayTime = 100; // Fixed delay time in milliseconds

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(LED_BUILTIN, OUTPUT); // Set the built-in LED pin as an output
  Serial.println("Author: Your Name");
  Serial.println("Enter a number to set the angle increment (frequency) in radians.");
}

void loop() {
  if (Serial.available() > 0) {
    float input = Serial.parseFloat(); // Read float input
    if (input > 0) {
      angleIncrement = input;
      Serial.print("Angle increment set to ");
      Serial.print(angleIncrement);
      Serial.println(" radians");
    } else {
      Serial.println("Invalid input. Please enter a positive number.");
    }

    // Clear the buffer
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  float value = sin(angle); // Calculate sine of the angle
  Serial.println(value); // Print value to Serial Plotter

  // Blink the LED based on the sine wave value
  if (value > 0) {
    digitalWrite(LED_BUILTIN, HIGH); // Turn LED on
  } else {
    digitalWrite(LED_BUILTIN, LOW); // Turn LED off
  }

  angle += angleIncrement; // Increment angle
  if (angle > TWO_PI) {
    angle = 0; // Reset angle
  }

  delay(delayTime); // Wait for the fixed delay time
}
