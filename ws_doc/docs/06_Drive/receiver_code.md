# Explanation of the Robot Receiver Code

This document provides a detailed explanation of the receiver code for a robot controlled by an Arduino Nano, using various sensors and modules.

## Components Used

- **Arduino Nano**
- **nRF24L01** Transceiver Module
- **L298N Motor Driver**
- **DC Motors**
- **WS2812B LED Strip**
- **SSD1306 OLED Display**
- **VL53L0X ToF Sensor**

## Libraries Included

- `SSD1306AsciiWire.h`: For OLED display control
- `Adafruit_VL53L0X.h`: For the VL53L0X ToF sensor
- `SPI.h`: For SPI communication with the nRF24L01 module
- `printf.h`: For debugging the nRF24L01
- `RF24.h`: For controlling the nRF24L01 transceiver
- `Wire.h`: For I2C communication with sensors and display
- `VNH3SP30.h`: For controlling the VNH3SP30 motor driver
- `FastLED.h`: For controlling the WS2812B LED strip

## Pin Definitions

- **NUM_LEDS**: Number of LEDs in the strip
- **DATA_PIN**: Pin connected to the LED strip
- **CE_PIN** and **CSN_PIN**: Pins connected to the nRF24L01 module
- **M1_PWM**, **M1_INA**, **M1_INB**: Pins connected to motor 1 control
- **M2_PWM**, **M2_INA**, **M2_INB**: Pins connected to motor 2 control

## Objects and Variables

- **Motor1, Motor2**: Objects for controlling the motors
- **leds**: Array for LED strip
- **colorPalette**: Color palette for the LED strip
- **oled**: Object for OLED display
- **tof**: Object for ToF sensor
- **radio**: Object for nRF24L01 transceiver
- **col0, col1, rows**: Variables for OLED display layout
- **labelMPU, unitsMPU**: Labels and units for OLED display
- **ax, ay**: Accelerometer data
- **trig**: Boolean for ToF sensor trigger
- **data**: Array for received data
- **address**: Addresses for nRF24L01 communication
- **radioNumber**: Flag for the transceiver role

## Setup Function

### Wire and Serial Initialization

```cpp
Wire.begin();
Wire.setClock(400000L);
Serial.begin(115200);
```

### LED Initialization

```cpp
FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
FastLED.setBrightness(50);
```

### OLED Display Initialization

```cpp
oled.begin(&Adafruit128x64, 0x3C);
oled.setFont(Callibri15);
oled.setLetterSpacing(3);
oled.clear();
```

- The display layout is set up with labels and units for MPU6050 data.

### nRF24L01 Initialization

```cpp
if (!radio.begin()) {
  Serial.println(F("radio hardware is not responding!!"));
  while (1) {}
}
radio.setPALevel(RF24_PA_LOW);
radio.setPayloadSize(sizeof(data));
radio.openReadingPipe(1, address[!radioNumber]);
radio.startListening();
```

- Initializes the nRF24L01 transceiver, sets power level, payload size, and opens the reading pipe for receiving data.

### ToF Sensor Initialization

```cpp
if (!tof.begin()) {
  Serial.println(F("Failed to boot VL53L0X"));
  while (1);
}
```

- Initializes the VL53L0X ToF sensor.

### Motor Initialization

```cpp
Motor1.begin(M1_PWM, M1_INA, M1_INB, -1, -1);
Motor2.begin(M2_PWM, M2_INB, M2_INA, -1, -1);
```

- Initializes the motor control objects with specified pins.

## Loop Function

### Reading ToF Sensor Data

```cpp
VL53L0X_RangingMeasurementData_t measure;
tof.rangingTest(&measure, false);
if (100 < measure.RangeMilliMeter && measure.RangeMilliMeter < 200) {
  trig = true;
  for (int led = 0; led < NUM_LEDS; led++) {
    leds[led] = CRGB::Red;
  }
} else {
  trig = false;
  sinelon();
}
```

- Reads the distance measurement from the ToF sensor and updates the LED strip based on the distance.

### Receiving Data from nRF24L01

```cpp
uint8_t pipe;
if (radio.available(&pipe)) {
  uint8_t bytes = radio.getPayloadSize();
  radio.read(&data, bytes);
  ax = data[0], ay = data[1];
  ax = float(ax) / 10;
  ay = float(ay) / 10;
  Serial.print(F("Received "));
  Serial.print(bytes);
  Serial.print(F(" bytes on pipe "));
  Serial.print(pipe);
  Serial.print(F(": "));
  Serial.print(data[0]);
  Serial.print(" : ");
  Serial.print(data[1]);
  Serial.print(", remapped to: ");
  Serial.print(ax);
  Serial.print(" : ");
  Serial.println(ay);
}
```

- Checks for available data from the nRF24L01 transceiver, reads the data, and prints it to the Serial Monitor.

### LED Strip Update

```cpp
FastLED.show();
```

- Updates the LED strip with the new colors.

### Motor Control

```cpp
if (-1.5 < ax && ax < 1.5 && -1.5 < ay && ay < 1.5) {
  setSpeed(0);
} else {
  setSpeed(max(-255, min(255, ay * 25)));
  turn(-ax * 9);
}
Serial.print(Motor1.speed);
Serial.print(" : ");
Serial.println(Motor2.speed);
```

- Controls the motor speed and direction based on the received accelerometer data.

## Supporting Functions

### clearValue Function

```cpp
void clearValue(uint8_t row) {
  oled.clear(col0, col1, row, row + rows - 1);
}
```

- Clears a value on the OLED display.

### sinelon Function

```cpp
void sinelon() {
  fadeToBlackBy(leds, NUM_LEDS, 50);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(100, 255, 192);
}
```

- Creates a sweeping colored dot effect on the LED strip.

### setSpeed Function

```cpp
void setSpeed(int speed) {
  Motor1.setSpeed(speed);
  Motor2.setSpeed(speed);
}
```

- Sets the speed for both motors.

### brake Function

```cpp
void brake(int brakePower) {
  Motor1.brake(brakePower);
  Motor2.brake(brakePower);
}
```

- Applies the brake with specified power to both motors.

### turn Function

```cpp
void turn(int angle) {
  int currentSpeed = (Motor1.speed + Motor2.speed) / 2;
  Motor1.setSpeed(currentSpeed + angle);
  Motor2.setSpeed(currentSpeed - angle);
}
```

- Turns the vehicle by providing different speed settings to the motors.

## Conclusion

This code integrates multiple sensors and modules to control a robot using an Arduino Nano. It receives data from an nRF24L01 transceiver, processes sensor data, and controls motors and LEDs accordingly. The OLED display is used to show real-time data from the sensors.


## Full Code

```cpp
#include <SSD1306AsciiWire.h>
#include "Adafruit_VL53L0X.h"
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <Wire.h>
#include <VNH3SP30.h>
#include <FastLED.h>

#define NUM_LEDS 8
#define DATA_PIN 2
#define CE_PIN 6
#define CSN_PIN 7


VNH3SP30 Motor1;  // define control object for motor 1
VNH3SP30 Motor2;  // define control object for motor 2

// motor pins (DIAG and CS pins not used in this example)
#define M1_PWM 9   // pwm pin motor
#define M1_INA 8   // control pin INA
#define M1_INB 10  // control pin INB

#define M2_PWM 3  // pwm pin motor
#define M2_INA 5  // control pin INA
#define M2_INB 4  // control pin INB

// Create a WS2812B object
CRGB leds[NUM_LEDS];
CRGBPalette16 colorPalette = CRGBPalette16(
  CRGB::Green, CRGB::Yellow, CRGB::Red);

// Create a OLED object
SSD1306AsciiWire oled;

Adafruit_VL53L0X tof = Adafruit_VL53L0X();

uint8_t col0 = 0;  // First value column
uint8_t col1 = 0;  // Last value column.
uint8_t rows;      // Rows per line.


const char* labelMPU[] = { " x:", " y:", " dist:", " Trig.:" };
const char* unitsMPU[] = { "m/s2", "m/s2", "mm", "" };

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

float ax, ay;

bool trig = false;

//Define packet for the direction (X axis and Y axis)
int data[2];

// Let these addresses be used for the pair
uint8_t address[][6] = { "1Trans", "1Rec" };

bool radioNumber = 1;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit


void setup() {
  Wire.begin();
  Wire.setClock(400000L);

  Serial.begin(115200);

  // print example's introductory prompt
  Serial.println(F("Custom Receiver running:"));
  Serial.println("By Tim Schumann");

  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);


  //start leds
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);

  //start OLED
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(Callibri15);
  oled.setLetterSpacing(3);
  oled.clear();

  // Setup form and find longest label.
  for (uint8_t i = 0; i < 4; i++) {
    oled.println(labelMPU[i]);
    uint8_t w = oled.strWidth(labelMPU[i]);
    col0 = col0 < w ? w : col0;
  }
  // Six pixels after label.
  col0 += 6;
  // Allow two or more pixels after value.
  col1 = col0 + oled.strWidth("99.99") + 2;
  // Line height in rows.
  rows = oled.fontRows();
  // Print units.
  for (uint8_t i = 0; i < 4; i++) {
    oled.setCursor(col1 + 1, i * rows);
    oled.print(unitsMPU[i]);
  }

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  radio.setPayloadSize(sizeof(data));  // float datatype occupies 4 bytes

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1

  radio.startListening();  // put radio in TX mode

  // For debugging info
  // printf_begin();              // needed only once for printing details
  // radio.printPrettyDetails();  // (larger) function that prints human readable data

  if (!tof.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }

  Motor1.begin(M1_PWM, M1_INA, M1_INB, -1, -1);  // Motor 1 object connected through specified pins
  Motor2.begin(M2_PWM, M2_INB, M2_INA, -1, -1);  // Motor 2 object connected through specified pins
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  tof.rangingTest(&measure, false);  // pass in 'true' to get debug data printout!
  if (100 < measure.RangeMilliMeter && measure.RangeMilliMeter < 200) {
    trig = true;
    for (int led = 0; led < NUM_LEDS; led++) {
      leds[led] = CRGB::Red;
    }
  } else {
    trig = false;
    sinelon();
  }
  uint8_t pipe;
  if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
    radio.read(&data, bytes);                // fetch payload from FIFO
    ax = data[0], ay = data[1];
    ax = float(ax) / 10;
    ay = float(ay) / 10;
    Serial.print(F("Received "));
    Serial.print(bytes);  // print the size of the payload
    Serial.print(F(" bytes on pipe "));
    Serial.print(pipe);  // print the pipe number
    Serial.print(F(": "));
    Serial.print(data[0]);  // print the payload's value
    Serial.print(" : ");
    Serial.print(data[1]);
    Serial.print(", remapped to: ");
    Serial.print(ax);
    Serial.print(" : ");
    Serial.println(ay);
  }

  /* clearValue(0);
  oled.print(ax);
  clearValue(rows);
  oled.print(ay);
  clearValue(2 * rows);
  oled.print(measure.RangeMilliMeter);
  clearValue(3 * rows);
  oled.print(trig);
*/
  FastLED.show();

  // dead zone
  if (-1.5 < ax && ax < 1.5 && -1.5 < ay && ay < 1.5) {
    setSpeed(0);
  } else {
    setSpeed(max(-255, min(255, ay * 25)));
    turn(-ax * 9);
  }
  Serial.print(Motor1.speed);
  Serial.print(" : ");
  Serial.println(Motor2.speed);
}


// Function to clear a value on the OLED display
void clearValue(uint8_t row) {
  oled.clear(col0, col1, row, row + rows - 1);
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 50);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(100, 255, 192);
}


void setSpeed(int speed) {
  Motor1.setSpeed(speed);
  Motor2.setSpeed(speed);
}

void brake(int brakePower) {
  Motor1.brake(brakePower);
  Motor2.brake(brakePower);
}

void turn(int angle) {
  // turn vehicle by providing different speed settings to the motors.
  // angle can be positive (right turn) or negative (left turn).
  // If the vehicle is already stopped, the vehicle will turn in place.
  int currentSpeed = (Motor1.speed + Motor2.speed) / 2;
  Motor1.setSpeed(currentSpeed + angle);
  Motor2.setSpeed(currentSpeed - angle);
}



//end of file
```