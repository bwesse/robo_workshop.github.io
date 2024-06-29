### L298 Motor Driver and Gearmotor

### Overview

The L298 motor driver is a dual H-Bridge driver that can control the direction and speed of two DC motors independently. It's ideal for driving gearmotors in small robotics projects.

![L298N-Module-Pinout.jpg](https://prod-files-secure.s3.us-west-2.amazonaws.com/b333f334-e0e6-48d1-aedc-47ceea98427f/a0c927dc-ad0c-477c-a6e3-c2fc369ca738/L298N-Module-Pinout.jpg)

### Components

- **L298 Motor Driver Module**
    - Input pins: IN1, IN2, IN3, IN4
    - Output pins: OUT1, OUT2, OUT3, OUT4
    - Enable pins: ENA, ENB
    - Power supply pins: VCC, GND
    - Logic supply pin: 5V
        
        ![A406.jpg](https://prod-files-secure.s3.us-west-2.amazonaws.com/b333f334-e0e6-48d1-aedc-47ceea98427f/338a5c3b-78e2-4e5b-80cb-8619fb60706a/A406.jpg)
        
- **Gearmotor**
    - DC motor with gear reduction
    - Typically includes two wires for power

### Wiring the L298 Motor Driver to an Arduino

1. **Power Supply**
    - Connect the VCC pin of the L298 to the power supply (e.g., 9V battery).
    - Connect the GND pin of the L298 to the ground of the power supply.
    - Connect the 5V pin of the L298 to the 5V pin on the Arduino.
    - Connect the GND pin of the L298 to the GND pin on the Arduino.
2. **Motor Connections**
    - Connect OUT1 and OUT2 to the terminals of the first gearmotor.
    - Connect OUT3 and OUT4 to the terminals of the second gearmotor (if used).
3. **Control Pins**
    - Connect IN1 and IN2 to two digital pins on the Arduino (e.g., D2 and D3).
    - Connect IN3 and IN4 to two other digital pins on the Arduino (e.g., D4 and D5) if using a second motor.
    - Connect ENA to a PWM-capable digital pin (e.g., D9).
    - Connect ENB to another PWM-capable digital pin (e.g., D10) if using a second motor.

### Arduino Code Example

```cpp
// Pin definitions
int ENA = 9;
int IN1 = 2;
int IN2 = 3;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Initialize motor stopped
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

void loop() {
  // Move motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 200); // Set speed

  delay(2000); // Run for 2 seconds

  // Stop motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  delay(1000); // Stop for 1 second

  // Move motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 200); // Set speed

  delay(2000); // Run for 2 seconds

  // Stop motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  delay(1000); // Stop for 1 second
}

```

### Tasks

1. **Basic Motor Control**
    - Wire the L298 motor driver to the Arduino and a single gearmotor.
    - Upload the provided Arduino code.
    - Verify that the gearmotor moves forward for 2 seconds, stops for 1 second, then moves backward for 2 seconds, and stops for 1 second.
2. **Speed Variation**
    - Modify the Arduino code to vary the speed of the gearmotor using different PWM values (e.g., 100, 150, 255).
    - Observe and document the changes in the gearmotor's speed.
3. **Dual Motor Setup**
    - Connect a second gearmotor to the L298 motor driver.
    - Adjust the Arduino code to control both motors independently.
    - Create a sequence where one motor moves forward while the other moves backward, then both stop, and then both move forward together.