#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <SSD1306AsciiWire.h>


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

SSD1306AsciiWire oled;

uint8_t col0 = 0;  // First value column
uint8_t col1 = 0;  // Last value column.
uint8_t rows;      // Rows per line.


const char* labelMPU[] = { " x:", " y:", " z:", " Temp.:" };
const char* unitsMPU[] = { "m/s2", "m/s2", "m/s2", " degC" };


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

  
  clearValue(0);
  oled.print(a.acceleration.x);
  clearValue(rows);
  oled.print(a.acceleration.y);
  clearValue(2 * rows);
  oled.print(a.acceleration.z);
  clearValue(3 * rows);
  oled.print(temp.temperature);
  


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

// Function to clear a value on the OLED display
void clearValue(uint8_t row) {
  oled.clear(col0, col1, row, row + rows - 1);
}
