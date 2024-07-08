#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <Wire.h>
#include <VNH3SP30.h>

#define CE_PIN 7
#define CSN_PIN 8

//Define packet for the direction (X axis and Y axis)
int data[2];

// Let these addresses be used for the pair
uint8_t address[][6] = { "1Trans", "1Rec" };

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

float ax, ay;

bool radioNumber = 1;  // 0 uses address[0] to transmit, 1 uses address[1] to receive


void setup() {
  Serial.begin(115200);

  // print example's introductory prompt
  Serial.println(F("Custom Receiver running:"));

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

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1

  radio.startListening();  // put radio in TX mode

  // For debugging info
  // printf_begin();              // needed only once for printing details
  // radio.printPrettyDetails();  // (larger) function that prints human readable data
}

void loop() {
  uint8_t pipe;
  if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
    radio.read(&data, bytes);                // fetch payload from FIFO
    ax = data[0] / 10;
    ay = data[1] / 10;
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
}
