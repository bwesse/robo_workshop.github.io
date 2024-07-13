### Quick Tutorial on I2C Bus
**What is I2C?**
I2C stands for Inter-Integrated Circuit. It’s a communication protocol used to connect low-speed devices like sensors, microcontrollers, and other peripherals to a microcontroller (like Arduino) or single-board computer (like Raspberry Pi).

### Key Concepts

**1. Master-Slave Architecture:**
- **Master**: The device that initiates communication (usually a microcontroller).
- **Slave**: The devices that respond to the master (sensors, displays, etc.).

**2. Two-Wire Communication:**
- **SDA (Serial Data Line)**: Carries the data.
- **SCL (Serial Clock Line)**: Carries the clock signal.

### Advantages
- **Simplicity**: Only two wires needed.
- **Scalability**: Multiple devices can be connected to the same two wires.

### Basic Terms

**1. Address:**
Each I2C device has a unique address (usually 7 bits) that the master uses to communicate with it.

**2. Data Frame:**
Communication is done in frames:
- **Start Condition**: The master sends a signal to start communication.
- **Address Frame**: The master sends the address of the target slave.
- **Data Frame**: The data being sent or received.
- **Stop Condition**: The master sends a signal to stop communication.

# OLED Display with I2C, MPU6050, and VL53L0X Integration Tutorial

This tutorial will guide you through the integration of a 1.3-inch OLED display with I2C, MPU6050 sensor, and VL53L0X ToF sensor using an Arduino Nano. We will display data from both sensors on the OLED.

## Requirements

- 1.3-inch OLED Display with I2C
- MPU6050 Sensor
- VL53L0X ToF Sensor
- Arduino Nano
- Breadboard and jumper wires
- Arduino IDE installed on your computer
- Necessary libraries: `SSD1306AsciiWire`, `Wire`, `Adafruit_VL53L0X`, `MPU6050`

## Setup

### Wiring the Sensors and Display to Arduino Nano

#### OLED Display

- **VCC** (Power) -> Connect to Arduino Nano 5V
- **GND** (Ground) -> Connect to Arduino Nano GND
- **SCL** (Clock) -> Connect to Arduino Nano A5 (SCL)
- **SDA** (Data) -> Connect to Arduino Nano A4 (SDA)

#### MPU6050

- **VCC** (Power) -> Connect to Arduino Nano 3.3V or 5V (Check sensor specifications)
- **GND** (Ground) -> Connect to Arduino Nano GND
- **SCL** (Clock) -> Connect to Arduino Nano A5 (SCL)
- **SDA** (Data) -> Connect to Arduino Nano A4 (SDA)

#### VL53L0X

- **VCC** (Power) -> Connect to Arduino Nano 3.3V or 5V (Check sensor specifications)
- **GND** (Ground) -> Connect to Arduino Nano GND
- **SCL** (Clock) -> Connect to Arduino Nano A5 (SCL)
- **SDA** (Data) -> Connect to Arduino Nano A4 (SDA)

### Installing Required Libraries

1. Open the Arduino IDE.
2. Go to **Sketch** > **Include Library** > **Manage Libraries**.
3. Install the following libraries:
   - `SSD1306AsciiWire`
   - `Adafruit_VL53L0X`
   - `MPU6050`

## Basic Code Example

```cpp
#include <Wire.h>
#include <SSD1306AsciiWire.h>
#include <MPU6050.h>
#include "Adafruit_VL53L0X.h"

// Create OLED display object
SSD1306AsciiWire oled;

// Create MPU6050 object
MPU6050 mpu;

// Create VL53L0X object
Adafruit_VL53L0X tof = Adafruit_VL53L0X();

// Variables to store sensor data
float ax, ay, az;
VL53L0X_RangingMeasurementData_t measure;

void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  
  Serial.begin(115200);

  // Initialize OLED display
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(Callibri15);
  oled.clear();

  // Initialize MPU6050
  if (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    while (1);
  }

  // Initialize VL53L0X
  if (!tof.begin()) {
    Serial.println("Failed to boot VL53L0X");
    while (1);
  }

  // Display setup labels
  oled.println("MPU6050 x:");
  oled.println("MPU6050 y:");
  oled.println("Distance:");
}

void loop() {
  // Read MPU6050 data
  Vector rawAccel = mpu.readRawAccel();
  ax = rawAccel.XAxis;
  ay = rawAccel.YAxis;

  // Read VL53L0X data
  tof.rangingTest(&measure, false);

  // Clear display
  oled.clear();
  
  // Display MPU6050 data
  oled.setCursor(0, 0);
  oled.print("MPU6050 x: "); oled.println(ax);
  oled.print("MPU6050 y: "); oled.println(ay);

  // Display VL53L0X data
  oled.print("Distance: ");
  if (measure.RangeStatus != 4) {
    oled.print(measure.RangeMilliMeter);
    oled.println(" mm");
  } else {
    oled.println("Out of range");
  }

  delay(500);
}
```

### Code Explanation

- **Libraries**: Includes the necessary libraries for OLED display, MPU6050, and VL53L0X sensors.
- **Initialization**: Initializes the sensors and the OLED display in the `setup` function.
- **Sensor Reading**: Reads data from MPU6050 and VL53L0X sensors in the `loop` function.
- **Display Data**: Clears the OLED display and then updates it with the latest sensor data.

## Running the Code

1. Connect your Arduino Nano to your computer via USB.
2. Open the Arduino IDE and paste the code above.
3. Select the correct board (Arduino Nano) and port under **Tools**.
4. Upload the code to your Arduino Nano.
5. The OLED display should show the data from the MPU6050 and VL53L0X sensors.

## Advanced Tasks

### Task 1: Change the Display Update Speed

**Objective**: Adjust the `delay` value in the `loop` function to change how often the display updates.

**Instructions**:
- Change the `delay(500);` line to a different value (e.g., `delay(100);` for faster updates or `delay(1000);` for slower updates).

### Task 2: Add More Sensor Data to Display

**Objective**: Display the Z-axis acceleration from the MPU6050.

**Instructions**:
1. Modify the setup and loop functions to include the Z-axis data.
2. Update the OLED display to show the new data.

### Task 3: Implement a Trigger Based on Sensor Data

**Objective**: Implement a trigger that changes the display color or shows a message when certain conditions are met (e.g., distance is within a certain range).

**Instructions**:
1. Add a condition in the `loop` function to check if the distance is within a specified range.
2. Display a message or change the display color when the condition is met.

## Tips

- Ensure your wiring is secure to avoid intermittent connections.
- Use a stable power supply to avoid fluctuations in sensor readings.
- Calibrate the sensors if you notice significant drift in the readings.

## Troubleshooting

- **Sensor Not Detected**: Check the wiring, ensure the correct voltage is supplied.
- **Incorrect Readings**: Make sure the sensors are not subjected to excessive vibrations or movements during initialization.
- **No Output on OLED**: Verify the I2C address of the OLED display and ensure it matches the address used in the code.

## Additional Resources

- [MPU6050 Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- [VL53L0X Datasheet](https://www.st.com/resource/en/datasheet/vl53l0x.pdf)
- [SSD1306Ascii Library Documentation](https://github.com/greiman/SSD1306Ascii)

This tutorial and the tasks provided should help you integrate and use the OLED display, MPU6050, and VL53L0X sensors with your Arduino Nano. Experiment with different setups and configurations to fully explore the capabilities of your project. Happy experimenting!