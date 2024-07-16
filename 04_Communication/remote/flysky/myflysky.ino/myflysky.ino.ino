


#include <SSD1306AsciiWire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <FastLED.h>
#include <Wire.h>
#include <VNH3SP30.h>
#include <IBusBM.h>

#define NUM_LEDS 8
#define DATA_PIN 2


bool trig = false;

VNH3SP30 Motor1;  // define control object for motor 1
VNH3SP30 Motor2;  // define control object for motor 2

// motor pins (DIAG and CS pins not used in this example)
#define M1_PWM 9  // pwm pin motor
#define M1_INA 8  // control pin INA
#define M1_INB 7  // control pin INB

#define M2_PWM 3  // pwm pin motor
#define M2_INA 5  // control pin INA
#define M2_INB 4  // control pin INB


IBusBM IBus;  // IBus object for receivig signals from transmitter/receiver


// Create a WS2812B object
CRGB leds[NUM_LEDS];
CRGBPalette16 colorPalette = CRGBPalette16(
  CRGB::Green, CRGB::Yellow, CRGB::Red);

// Create a BME280 object
Adafruit_MPU6050 mpu;

// Create a OLED object
SSD1306AsciiWire oled;

uint8_t col0 = 0;  // First value column
uint8_t col1 = 0;  // Last value column.
uint8_t rows;      // Rows per line.


const char* labelMPU[] = { " x:", " y:", " z:", " Trig.:" };
const char* unitsMPU[] = { "m/s2", "m/s2", "m/s2", "" };


void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  //baudrate if broke
  Serial.begin(9600);
  Serial.println("\r\nBens Bot");
  Serial.println("By Tim Schumann");
  Serial.println();

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

  //start mpu
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    //while (1) {
    //  delay(10);
    //}
  }


  Motor1.begin(M1_PWM, M1_INA, M1_INB, -1, -1);  // Motor 1 object connected through specified pins
  Motor2.begin(M2_PWM, M2_INB, M2_INA, -1, -1);  // Motor 2 object connected through specified pins

  IBus.begin(Serial);  // iBUS connected to Serial1: 1 (RX)

  // We have to wait for the receiver to receive data from the transmitter (transmitter needs to be turned on)
  // as the channel values all read 0 as long as the transmitter is turned off at boot time.
  // We do not want the car to drive full speed backwards out of control.
  Serial.println("Wait for receiver");
  while (IBus.cnt_rec == 0) delay(100);
  Serial.println("Init done");
}


//---------------------------------------------------------------------------------------------------------

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  //mpu.getEvent(&a, &g, &temp);

  clearValue(0);
  oled.print(a.acceleration.x);
  clearValue(rows);
  oled.print(a.acceleration.y);
  clearValue(2 * rows);
  oled.print(a.acceleration.z);
  clearValue(3 * rows);
  oled.print((int)IBus.readChannel(1));

  if (abs(a.acceleration.x) + abs(a.acceleration.y) + abs(a.acceleration.z) > 16) {
    trig = true;
    for (int led = 0; led < NUM_LEDS; led++) {
      leds[led] = CRGB::Red;
    }
    // setSpeed(0);  // motor stop
  } else {
    trig = false;
    sinelon();
    //setSpeed(50);
  }

  FastLED.show();

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");

  if (IBus.readChannel(4) > 1500) {  //armswitch


    int spd, turn;
    // speed depends on front switch (channel 5) (forward/backwards) and channel 2 (speed)
    spd = ((int)IBus.readChannel(1) - 1500);
    // every value below 1050 we interprete as stop
    if (spd < 50 && spd > -50) spd = 0;
    else spd = (spd * 4) / 15;                   // value could reach (2000-1050)*4/9 = 422, but setspeed() will max at 400
    if (IBus.readChannel(5) > 1500) spd = -spd;  // backward/forward depends on switch at channel 6


    // turn depends on channel 0, scale down to -200, +200
    turn = -(((int)IBus.readChannel(3) - 1500) * 4) / 10;
    if (turn > -20 && turn < 20) turn = 0;

    // set combined speed and turn (if speed==0, then only turn in place)
    speedturn(spd, turn);
  }


  delay(10);
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

void speedturn(int speed, int angle) {
  // set speed (-400 -> +400) and turn (-400 -> +400)
  // turn vehicle by providing different speed settings to the motors.
  // angle can be positive (right turn) or negative (left turn).
  // If the vehicle is already stopped, the vehicle will turn in place.
  Motor1.setSpeed(speed + angle);
  Motor2.setSpeed(speed - angle);
}



//end of file