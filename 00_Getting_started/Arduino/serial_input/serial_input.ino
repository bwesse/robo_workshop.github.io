//BW


void setup() {
  // Initialize the serial communication at a baud rate of 9600
  Serial.begin(9600);

  // Initial print to the serial monitor
  Serial.println("Arduino setup complete.");
  Serial.println("Please enter a value and press Enter:");
}

void loop() {
  // Check if there is any serial input
  if (Serial.available() > 0) {
    // Read the input as a string
    String input = Serial.readStringUntil('\n');

    // Print the input back to the serial monitor
    Serial.print("You entered: ");
    Serial.println(input);

    // Print a prompt for the next input
    Serial.println("Please enter another value and press Enter:");
  }
}

//end of file