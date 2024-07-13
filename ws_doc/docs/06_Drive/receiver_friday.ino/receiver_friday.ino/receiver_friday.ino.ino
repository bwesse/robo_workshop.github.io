/* ============================================

    Sketch for Receiver

    Author: S. Puchas, T. Schumann
    Date: 2024-12-7

    Dependencies:
    SSD1306Ascii - https://github.com/greiman/SSD1306Ascii
    VL53L0X - https://github.com/adafruit/Adafruit_VL53L0X
    RF24 - https://nrf24.github.io/RF24/
    VNH3SP30 - https://github.com/bmellink/VNH3SP30
    FastLED - https://github.com/FastLED/FastLED

    All rights reserved. Copyright Tim Schumann 2024

  ===============================================
*/

// preinstalled
#include <Wire.h>
#include <SPI.h>
#include "printf.h"

// need to be added
#include <SSD1306AsciiWire.h>
#include <VL53L0X.h>
#include <RF24.h>
#include <VNH3SP30.h>
#include <FastLED.h>

#define NUM_LEDS 8
#define DATA_PIN 2
#define CE_PIN 10
#define CSN_PIN 6


VNH3SP30 Motor1;  // define control object for motor 1
VNH3SP30 Motor2;  // define control object for motor 2

// motor pins (DIAG and CS pins not used in this example)
#define M1_PWM 9  // pwm pin motor
#define M1_INA 8   // control pin INA
#define M1_INB 7  // control pin INB

#define M2_PWM 3  // pwm pin motor
#define M2_INA 5  // control pin INA
#define M2_INB 4  // control pin INB

// Create a WS2812B object
CRGB leds[NUM_LEDS];
CRGBPalette16 colorPalette = CRGBPalette16(
  CRGB::Green, CRGB::Yellow, CRGB::Red);

// Create a OLED object
SSD1306AsciiWire oled;

VL53L0X tof;

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

//-----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------

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

  if (!tof.init()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }
  tof.setTimeout(500);
  tof.startContinuous();

  Motor1.begin(M1_PWM, M1_INA, M1_INB, -1, -1);  // Motor 1 object connected through specified pins
  Motor2.begin(M2_PWM, M2_INB, M2_INA, -1, -1);  // Motor 2 object connected through specified pins
}

void loop() {
  int distance = tof.readRangeContinuousMillimeters();
  if (100 < distance && distance < 200) {
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

  clearValue(0);
  oled.print(ax);
  clearValue(rows);
  oled.print(ay);
  clearValue(2 * rows);
  oled.print(distance);
  clearValue(3 * rows);
  oled.print(trig);

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



//end of file