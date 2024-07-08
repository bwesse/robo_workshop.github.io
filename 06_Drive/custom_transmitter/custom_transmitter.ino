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
