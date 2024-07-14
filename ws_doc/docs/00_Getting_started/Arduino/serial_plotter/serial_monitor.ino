
// Include the math library to use the sin() function
#include <math.h>

// Declare a variable to keep track of the angle
float angle = 0.0;

void setup() {
  // Initialize the serial communication at a baud rate of 9600
  Serial.begin(9600);
}

void loop() {
  // Calculate the sine of the angle
  float value = sin(angle);

  // Print the value to the serial monitor
  Serial.println(value);

  // Increment the angle
  angle += 0.1;

  // Reset the angle if it exceeds 2*PI
  if (angle > TWO_PI) {
    angle = 0;
  }

  // Wait for 100 milliseconds before the next loop
  delay(100);
}