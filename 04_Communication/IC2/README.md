### Quick Tutorial on I2C Bus for 18-Year-Olds

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

### How to Use I2C with Arduino

**1. Hardware Setup:**
- Connect the SDA and SCL pins of your I2C device to the corresponding pins on the Arduino.
  - For Arduino Uno: SDA (A4), SCL (A5)
  - For Arduino Nano: SDA (A4), SCL (A5)

**2. Software Setup:**
- Use the `Wire` library to handle I2C communication.

To demonstrate how both an MPU6050 accelerometer/gyroscope sensor and a 1.3-inch OLED display can communicate over the same I2C bus with an Arduino Nano, we'll need to use the Wire library for I2C communication, along with specific libraries for the MPU6050 and the OLED display.

### Prerequisites:
1. **Hardware**:
   - Arduino Nano
   - MPU6050 sensor
   - 1.3-inch OLED display (assuming it's an SSD1306 or SH1106 based display)
   - Jumper wires
   - Pull-up resistors (typically 4.7kΩ) if not already on the breakout boards

2. **Connections**:
   - **MPU6050**:
     - VCC to 5V (or 3.3V if required)
     - GND to GND
     - SDA to A4
     - SCL to A5
   - **OLED Display**:
     - VCC to 5V (or 3.3V if required)
     - GND to GND
     - SDA to A4
     - SCL to A5

Both devices will share the SDA and SCL lines.

### Libraries to Install:
- **MPU6050 Library**: `MPU6050 by Electronic Cats`
- **Adafruit SSD1306 Library**: `Adafruit SSD1306`
- **Adafruit GFX Library**: `Adafruit GFX Library`

### Example Code:

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>

// Define the OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Create an MPU6050 object
MPU6050 mpu;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize I2C communication
  Wire.begin();

  // Initialize the OLED display
  if (!display.begin(SSD1306_I2C_ADDRESS, SCREEN_WIDTH, SCREEN_HEIGHT)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Initialize the MPU6050 sensor
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println(F("MPU6050 connection failed"));
    while (1);
  }
}

void loop() {
  // Read accelerometer and gyroscope values
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Print values to serial monitor
  Serial.print("aX = "); Serial.print(ax);
  Serial.print(" | aY = "); Serial.print(ay);
  Serial.print(" | aZ = "); Serial.println(az);
  Serial.print("gX = "); Serial.print(gx);
  Serial.print(" | gY = "); Serial.print(gy);
  Serial.print(" | gZ = "); Serial.println(gz);

  // Display values on the OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("MPU6050 Values:");
  display.print("aX = "); display.println(ax);
  display.print("aY = "); display.println(ay);
  display.print("aZ = "); display.println(az);
  display.print("gX = "); display.println(gx);
  display.print("gY = "); display.println(gy);
  display.print("gZ = "); display.println(gz);
  display.display();

  // Delay before next reading
  delay(1000);
}
```

### Explanation:
1. **Setup**: Initialize the serial communication, I2C bus, OLED display, and MPU6050 sensor.
2. **Reading Values**: Read the accelerometer and gyroscope values from the MPU6050.
3. **Display Values**: Print the values to the serial monitor and display them on the OLED screen.
4. **Loop**: Continuously read and display the sensor values every second.

### How to Run:
1. Connect your Arduino Nano to your computer.
2. Open the Arduino IDE.
3. Copy and paste the above code into a new sketch.
4. Select the correct board and port in the Arduino IDE.
5. Upload the code to the Arduino Nano.
6. Open the Serial Monitor (Tools -> Serial Monitor) to see the sensor values and check the OLED display for the same information.

This code will help you test and visualize how both the MPU6050 sensor and the OLED display can communicate over the same I2C bus. Make sure to install the required libraries through the Arduino Library Manager before uploading the code.


### Conclusion
I2C is a simple and efficient way to communicate with multiple devices using just two wires. Understanding the basics of I2C communication can help you integrate a variety of sensors and peripherals into your projects, making your designs more powerful and flexible.

### Useful Tips
- Always check the datasheet of your I2C device for the correct address and data format.
- Use pull-up resistors (typically 4.7kΩ) on the SDA and SCL lines to ensure proper communication.
- Start with simple examples and gradually integrate more complex devices as you get comfortable with I2C.