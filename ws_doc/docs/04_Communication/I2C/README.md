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
|            | **MPU6050**              | **OLED**        | **VL53L0X**             |
|------------|--------------------------|-----------------|-------------------------|
| **VCC**    |  5V               | 5V              | 5V              |
| **GND**    | GND                      | GND             | GND                     |
| **SCL**    | A5 (SCL)                 | A5 (SCL)        | A5 (SCL)                |
| **SDA**    | A4 (SDA)                 | A4 (SDA)        | A4 (SDA)                |

### Installing Required Libraries

1. Open the Arduino IDE.
2. Go to **Sketch** > **Include Library** > **Manage Libraries**.
3. Install the following libraries:
   - `SSD1306AsciiWire`
   - `Adafruit_VL53L0X`
   - `Adafrui MPU6050`

## Basic Code Example MPU6050 + OLED
```cpp
#include <SPI.h>
#include "printf.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <SSD1306AsciiWire.h>

// Create a BME280 object
Adafruit_MPU6050 mpu;

SSD1306AsciiWire oled;

uint8_t col0 = 0;  // First value column
uint8_t col1 = 0;  // Last value column.
uint8_t rows;      // Rows per line.


const char* labelMPU[] = { " x:", " y:", " z:", " Temp.:" };
const char* unitsMPU[] = { "m/s2", "m/s2", "m/s2", " degC" };


void setup() {
  Wire.begin();

  Serial.begin(115200);

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

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.println(" m/s^2");
  
  clearValue(0);
  oled.print(a.acceleration.x);
  clearValue(rows);
  oled.print(a.acceleration.y);
  clearValue(2 * rows);
  oled.print(a.acceleration.z);
  clearValue(3 * rows);
  oled.print(temp.temperature);
  
  // to make this example readable in the serial monitor
  delay(10);  // slow transmissions down by 1 second
}

// Function to clear a value on the OLED display
void clearValue(uint8_t row) {
  oled.clear(col0, col1, row, row + rows - 1);
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