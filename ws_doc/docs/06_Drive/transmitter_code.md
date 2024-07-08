# Explanation of the Transmitter Code

This document provides a detailed explanation of the transmitter code used to send data from an Arduino Nano with an MPU6050 sensor using the nRF24L01 module.

## Components Used

- **Arduino Nano**
- **nRF24L01** Transceiver Module
- **MPU6050** Accelerometer and Gyroscope Sensor
- **Breadboard and jumper wires**

## Libraries Included

- `SPI.h`: For SPI communication with the nRF24L01 module
- `printf.h`: For debugging the nRF24L01
- `RF24.h`: For controlling the nRF24L01 transceiver
- `Wire.h`: For I2C communication with the MPU6050 sensor
- `Adafruit_Sensor.h`: For sensor abstraction
- `Adafruit_MPU6050.h`: For controlling the MPU6050 sensor

## Pin Definitions

- **CE_PIN**: Pin connected to the CE pin of the nRF24L01 (D7)
- **CSN_PIN**: Pin connected to the CSN pin of the nRF24L01 (D8)

## Objects and Variables

- **data**: Array to hold the X and Y axis data to be transmitted
- **address**: Array to hold the addresses for the nRF24L01 communication
- **radio**: Object for the nRF24L01 transceiver
- **mpu**: Object for the MPU6050 sensor
- **radioNumber**: Boolean to determine if the device is transmitting or receiving (0 for transmitting, 1 for receiving)

## Setup Function

### Wire and Serial Initialization

```cpp
Wire.begin();
Serial.begin(115200);
Serial.println(F("Custom Transmitter running:"));
Serial.println("By Tim Schumann");
Serial.print(F("radioNumber = "));
Serial.println((int)radioNumber);
```

- Initializes the I2C bus for the MPU6050 sensor.
- Starts the serial communication for debugging purposes.

### nRF24L01 Initialization

```cpp
if (!radio.begin()) {
  Serial.println(F("radio hardware is not responding!!"));
  while (1) {}
}

radio.setPALevel(RF24_PA_LOW);
radio.setPayloadSize(sizeof(data));
radio.openWritingPipe(address[radioNumber]);
radio.stopListening();
```

- Initializes the nRF24L01 transceiver, sets power level, payload size, and opens the writing pipe for transmitting data.

### Debugging Information

```cpp
printf_begin();
radio.printPrettyDetails();
```

- Initializes the printf debugging and prints detailed information about the nRF24L01 module.

### MPU6050 Initialization

```cpp
if (!mpu.begin()) {
  Serial.println("Failed to find MPU6050 chip");
  while (1) {
    delay(10);
  }
}
```

- Initializes the MPU6050 sensor. If the sensor is not found, the program enters an infinite loop.

## Loop Function

### Reading Sensor Data

```cpp
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);

data[0] = max(-10.0, min(10.0, a.acceleration.x)) * 10;
data[1] = max(-10.0, min(10.0, a.acceleration.y)) * 10;
```

- Reads the acceleration data from the MPU6050 sensor and scales it to fit within the range of -100 to 100.

### Transmitting Data

```cpp
unsigned long start_timer = micros();
bool report = radio.write(&data, sizeof(data));
unsigned long end_timer = micros();
```

- Transmits the data using the nRF24L01 module and measures the time taken for transmission.

### Debugging Information

```cpp
if (report) {
  Serial.print(F("Transmission successful! "));
  Serial.print(F("Time to transmit = "));
  Serial.print(end_timer - start_timer);
  Serial.print(F(" us. Sent: "));
  Serial.print(data[0]);
  Serial.print(" : ");
  Serial.println(data[1]);
} else {
  Serial.println(F("Transmission failed or timed out"));
  radio.printPrettyDetails();
}
```

- If the transmission is successful, it prints the time taken and the transmitted data. If the transmission fails, it prints an error message.

### Loop Delay

```cpp
delay(10);
```

- Adds a delay of 10 milliseconds to slow down the transmission rate.

## Conclusion

This code reads acceleration data from an MPU6050 sensor and transmits it using an nRF24L01 module. The setup and loop functions handle the initialization and continuous data transmission, respectively. Debugging information is printed to the Serial Monitor to assist in monitoring the system's operation.


## Full code:
```cpp
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>


#define CE_PIN 7
#define CSN_PIN 8

//Define packet for the direction (X axis and Y axis)
int data[2];

// Let these addresses be used for the pair
uint8_t address[][6] = { "1Trans", "1Rec" };

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

// Create a BME280 object
Adafruit_MPU6050 mpu;

bool radioNumber = 0;  // 0 uses address[0] to transmit, 1 uses address[1] to receive

void setup() {
  Wire.begin();

  Serial.begin(115200);

  // print example's introductory prompt
  Serial.println(F("Custom Transmitter running:"));
  Serial.println("By Tim Schumann");


  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

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

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0

  radio.stopListening();  // put radio in TX mode

  // For debugging info
  printf_begin();              // needed only once for printing details
  radio.printPrettyDetails();  // (larger) function that prints human readable data


  //start mpu
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /*Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.println(" m/s^2");
*/

  data[0] = max(-10.0, min(10.0, a.acceleration.x)) * 10;
  data[1] = max(-10.0, min(10.0, a.acceleration.y)) * 10;

  unsigned long start_timer = micros();            // start the timer
  bool report = radio.write(&data, sizeof(data));  // transmit & save the report
  unsigned long end_timer = micros();              // end the timer

  if (report) {
    /*Serial.print(F("Transmission successful! "));  // payload was delivered
    Serial.print(F("Time to transmit = "));
    Serial.print(end_timer - start_timer);  // print the timer result
    Serial.print(F(" us. Sent: "));
    Serial.print(data[0]);  // print payload sent
    Serial.print(" : ");
    Serial.println(data[1]);*/
  } else {
    //Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
    //radio.printPrettyDetails();                             // (larger) function that prints human readable data
  }

  // to make this example readable in the serial monitor
  delay(10);  // slow transmissions down by 1 second
}
```	