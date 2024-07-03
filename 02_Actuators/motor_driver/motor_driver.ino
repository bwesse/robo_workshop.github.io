

//todotesting


// Define motor control pins
#define ENA 9
#define IN1 8
#define IN2 7
//


void setup() {
  // Set all the motor control pins to outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Initialize the serial communication
  Serial.begin(9600);
  Serial.println("L298N Motor Driver Test");
}

void loop() {
  // Rotate motor clockwise
  Serial.println("Rotating motor clockwise");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 200); // Speed control (0-255)
  delay(2000);

  // Stop the motor
  Serial.println("Stopping motor");
  analogWrite(ENA, 0);
  delay(1000);

  // Rotate motor counterclockwise
  Serial.println("Rotating motor counterclockwise");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 200); // Speed control (0-255)
  delay(2000);

  // Stop the motor
  Serial.println("Stopping motor");
  analogWrite(ENA, 0);
  delay(1000);
}
