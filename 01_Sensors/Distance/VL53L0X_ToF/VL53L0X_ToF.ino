#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  sensor.init();
  sensor.setTimeout(500);
  
  Serial.println("VL53L0X sensor test");
}

void loop() {
  // Measure distance
  uint16_t distance = sensor.readRangeSingleMillimeters();
  
  if (sensor.timeoutOccurred()) {
    Serial.print("Timeout occurred");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  }

  delay(1000);
}
