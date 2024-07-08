# MPU6050 Sensor Tutorial for Arduino Nano

![MPU6050](../../99_Resources/images/MPU6050-Module-Gyroscope-Axis.webp)

The MPU6050 is a 6-axis motion tracking device that combines a 3-axis gyroscope and a 3-axis accelerometer. This tutorial will guide you through the basics of setting up and using the MPU6050 sensor with an Arduino Nano.

## Requirements

- MPU6050 Sensor
- Arduino Nano
- Breadboard and jumper wires
- Arduino IDE installed on your computer
- MPU6050 Arduino library

## Setup

### Wiring the Sensor to Arduino Nano

![mpu6050pinout](../../99_Resources/images/MPU6050-Pinout.png)

- **VCC** (Power) -> Connect to Arduino Nano 3.3V or 5V (Check sensor specifications for exact voltage)
- **GND** (Ground) -> Connect to Arduino Nano GND
- **SCL** (Clock) -> Connect to Arduino Nano A5 (SCL pin)
- **SDA** (Data) -> Connect to Arduino Nano A4 (SDA pin)

### Installing the MPU6050 Library

1. Open the Arduino IDE.
2. Go to **Sketch** > **Include Library** > **Manage Libraries**.
3. In the Library Manager, search for "MPU6050".
4. Find the "MPU6050 by Electronic Cats" library and click **Install**.

## Basic Code Example

```cpp
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("Failed to connect to MPU6050!");
    while (1) {}
  }
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  Serial.print("aX = "); Serial.print(ax);
  Serial.print(" | aY = "); Serial.print(ay);
  Serial.print(" | aZ = "); Serial.println(az);

  Serial.print("gX = "); Serial.print(gx);
  Serial.print(" | gY = "); Serial.print(gy);
  Serial.print(" | gZ = "); Serial.println(gz);

  delay(1000);
}
```

### Code Explanation

- **Libraries**: Includes the Wire library for I2C communication and the MPU6050 library for sensor functions.
- **Sensor Initialization**: Sets up serial communication, initializes the I2C communication, and the sensor itself.
- **Reading Data**: In the `loop` function, reads acceleration and gyroscope data and prints it to the Serial Monitor.

## Running the Code

1. Connect your Arduino Nano to your computer via USB.
2. Open the Arduino IDE and paste the code above.
3. Select the correct board (Arduino Nano) and port under **Tools**.
4. Upload the code to your Arduino Nano.
5. Open the Serial Monitor (**Tools** > **Serial Monitor**) and set the baud rate to 9600.
6. You should see the acceleration and gyroscope readings displayed.

## Tips

- Ensure your wiring is secure to avoid intermittent connections.
- Place the sensor on a stable surface to get accurate readings.
- Calibrate the sensor if you notice significant drift in the readings.

## Troubleshooting

- **Sensor Not Detected**: Check the wiring, ensure the correct voltage is supplied.
- **Incorrect Readings**: Make sure the sensor is not subjected to excessive vibrations or movements during initialization.
- **No Output**: Verify the baud rate of the Serial Monitor matches the one set in the code.

## Additional Resources

- [MPU6050 Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- [Arduino MPU6050 Library Documentation](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050)

This simple tutorial should get you started with using the MPU6050 sensor with your Arduino Nano. Experiment with different setups and movements to fully explore its capabilities. Happy experimenting!