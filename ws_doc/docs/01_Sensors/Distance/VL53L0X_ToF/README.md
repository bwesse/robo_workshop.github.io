# VL53L0X Distance Sensor Tutorial

![ToF](../../../99_Resources/images/vl53l0x.jpeg)


## Introduction
The VL53L0X is a time-of-flight (ToF) distance sensor that measures the time it takes for light to travel to an object and back, providing accurate distance measurements. 

### Theory
- **Time-of-Flight (ToF):** Measures the time taken for a light pulse to travel to a target and return.
- **Laser Emission:** Uses a 940nm laser for distance measurement.
- **Range:** Capable of measuring distances from 30mm to 2m.
- **Accuracy:** Highly accurate with a low error rate.
- **I2C Communication:** Uses I2C protocol for communication with microcontrollers like Arduino.

## Wiring
Connect the VL53L0X sensor to the Arduino as follows:
- **VCC:** 5V
- **GND:** GND
- **SDA:** A4 (SDA)
- **SCL:** A5 (SCL)

## Code Example
Here's a simple Arduino sketch to test the VL53L0X sensor:

```cpp
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

  delay(1000); // (1)
}

```
1.  :man_raising_hand: I'm a code annotation! I can contain `code`, __formatted
    text__, images, ... basically anything that can be written in Markdown.


# Code Examples

## Example: Switching Between receiver.cpp and transmitter.cpp

# Code Examples

```cpp
#include <iostream>
using namespace std;

void bubbleSort(int arr[], int n) {  // (1)
    for (int i = 0; i < n - 1; i++) {  // (2)
        for (int j = 0; j < n - i - 1; j++) {  // (3)
            if (arr[j] > arr[j + 1]) {  // (4)
                swap(arr[j], arr[j + 1]);  // (5)
            }
        }
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr)/sizeof(arr[0]);
    bubbleSort(arr, n);
    cout << "Sorted array: \n";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
    return 0;
}
{ .annotate }

1. This is the function definition for bubble sort.
2. Outer loop to traverse through all elements.
3. Inner loop to compare adjacent elements.
4. Compare the adjacent elements.
5. Swap the elements if they are in the wrong order.
```

## Example: Switching Between receiver.cpp and transmitter.cpp

=== "receiver.cpp"
    ```cpp
    // receiver.cpp
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>

    RF24 radio(9, 10); // CE, CSN

    void setup() {
        Serial.begin(9600);
        radio.begin();
        radio.openReadingPipe(1, "00001");
        radio.startListening();
    }

    void loop() {
        if (radio.available()) {
            char text[32] = "";
            radio.read(&text, sizeof(text));
            Serial.println(text);
        }
    }
    ```

=== "transmitter.cpp"
    ```cpp
    // transmitter.cpp
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>

    RF24 radio(9, 10); // CE, CSN

    void setup() {
        Serial.begin(9600);
        radio.begin();
        radio.openWritingPipe("00001");
        radio.stopListening();
    }

    void loop() {
        const char text[] = "Hello, World!";
        radio.write(&text, sizeof(text));
        delay(1000);
    }
    ```


## Tasks

### Task 1: Basic Distance Measurement
**Objective:** Measure and display the distance on the Serial Monitor.

**Instructions:**
1. Set up the VL53L0X sensor with your Arduino.
2. Upload the provided code to your Arduino.
3. Open the Serial Monitor to view the distance readings.

**Hints:**
<details>
  <summary>Hint 1</summary>
  Ensure that the sensor is connected correctly to the I2C pins (SDA and SCL).
</details>
<details>
  <summary>Hint 2</summary>
  If you receive a "Timeout" message, check the wiring and ensure the sensor is not obstructed.
</details>

### Task 2: Distance-Based LED Indicator
**Objective:** Use an LED to indicate if an object is within a certain distance.

**Instructions:**
1. Connect an LED to pin 13 of the Arduino.
2. Modify the code to turn on the LED if the distance is less than 100mm.
3. Upload the modified code and test with different distances.

**Hints:**
<details>
  <summary>Hint 1</summary>
  Use a conditional statement to check the distance.
</details>
<details>
  <summary>Hint 2</summary>
  Use `digitalWrite(13, HIGH)` to turn on the LED and `digitalWrite(13, LOW)` to turn it off.
</details>

### Task 3: Distance Threshold Alarm
**Objective:** Create an alarm system that activates a buzzer if the distance falls below a threshold.

**Instructions:**
1. Connect a buzzer to pin 8 of the Arduino.
2. Modify the code to activate the buzzer if the distance is less than 50mm.
3. Upload the code and test the system.

**Hints:**
<details>
  <summary>Hint 1</summary>
  Use a similar conditional statement as in Task 2 to check the distance.
</details>
<details>
  <summary>Hint 2</summary>
  Use `digitalWrite(8, HIGH)` to turn on the buzzer and `digitalWrite(8, LOW)` to turn it off.
</details>

## Conclusion
The VL53L0X distance sensor is a versatile and accurate ToF sensor, useful in various applications such as robotics and automation. This tutorial provided basic examples and tasks to help you get started with this sensor.