### Controlling a WS2812B LED Strip with Arduino

![WS2812B LED Strip](../../99_Resources/images/RGBstrip.jpg)

#### 1. **Gather Necessary Components:**
   - **Arduino Board:** (e.g., Arduino Nano).
   - **Cable:** mini-usb to usb
   - **WS2812B LED Strip:** Ensure it has 8 LEDs.
   - **Connecting Wires:** Jumper wires for connections.
   - **Breadboard,** for prototyping.

#### Breadboards

![Breadboard](../../99_Resources/images/BB-01.jpg)

#### **1. Purpose:**
   - Used for prototyping electronic circuits without soldering.

#### **2. Structure:**
   - **Rows and Columns:**
     - Consist of a grid of holes arranged in rows and columns.
   - **Metal Strips:**
     - Metal strips underneath the board connect the holes electrically.

#### **3. Power Rails:**
   - **Lines:**
     - Two long rows typically run along the top and bottom for power distribution.
   - **Purpose:**
     - Used to connect power supply (positive and negative).

#### **4. Terminal Strips:**
   - **Rows:**
     - Consist of short horizontal rows in the middle.
   - **Connections:**
     - Each row of five holes is electrically connected, allowing components to be inserted and interconnected.

#### **5. Component Placement:**
   - **Insertion:**
     - Components like resistors, capacitors, and ICs are inserted into the holes.
   - **Connections:**
     - Wires or jumper cables are used to connect different components.

#### **6. Integration:**
   - **Circuits:**
     - Allows easy creation and modification of circuits.
   - **Testing:**
     - Facilitates testing and troubleshooting before finalizing designs.

#### **7. Advantages:**
   - **Non-permanent:**
     - Components can be easily removed or replaced.
   - **Reusable:**
     - Breadboards can be reused for multiple projects.

#### **8. Common Uses:**
   - Ideal for learning, prototyping, and debugging electronic circuits.

------------------------------------------


![led pinout](../../99_Resources/images/WS2812B-Addressable-RGB-LED-pinout-diagram.webp)	


#### 2. **Circuit Connections:**
   - **Power Connections:**
     - Connect the 5V and GND of the power supply to the LED strip.
   - **Data Connection:**
     - Connect the Data Input (DIN) of the LED strip to a digital pin on the Arduino (e.g., pin 6).
   - **Arduino Power:**
     - Connect the Arduino to your computer via USB

   **Basic Wiring Diagram:**
   ```
   Arduino          WS2812B LED Strip
   5V  ------------  5V
   GND ------------  GND
   Pin 6 ----------  DIN
   ```

#### 3. **Install Necessary Library:**
   - **Fastled Library:**
     - Open Arduino IDE.
     - Navigate to `Sketch` > `Include Library` > `Manage Libraries`.
     - Search for "Fastled" and install the library.


#### 4. **Write the Arduino Code:**

   ```cpp
   #include <FastLED.h>

   // Define the pin that is connected to the DIN of the LED strip
   #define LED_PIN 6

   // Define the number of LEDs in the strip
   #define NUM_LEDS 8

   // Create an array of LED objects
   CRGB leds[NUM_LEDS];

   void setup() {
     // Initialize the LED strip
     FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
     FastLED.clear();
     FastLED.show(); // Initialize all pixels to 'off'
   }

   void loop() {
     // Call a function to perform an animation
     rainbowCycle(20);
   }

   // Function to fill the strip with a rainbow pattern
   void rainbowCycle(uint8_t wait) {
     for (int j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on the wheel
       for (int i = 0; i < NUM_LEDS; i++) {
         leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
       }
       FastLED.show();
       delay(wait);
     }
   }

   // Function to generate rainbow colors across 0-255 positions
   CRGB Wheel(byte WheelPos) {
     WheelPos = 255 - WheelPos;
     if (WheelPos < 85) {
       return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
     }
     if (WheelPos < 170) {
       WheelPos -= 85;
       return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
     }
     WheelPos -= 170;
     return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
   }
   ```

#### 5. **Upload the Code:**
   - Connect your Arduino to your computer via USB.
   - Open the Arduino IDE and select the appropriate board and port.
   - Copy the above code into the Arduino IDE.
   - Click the `Upload` button to upload the code to the Arduino.

#### 6. **Observe the LED Strip:**
   - Once the code is uploaded, the LED strip should start displaying a rainbow cycle pattern.
   - You can modify the animation function or create new ones to experiment with different effects.

#### 7. **Experiment with Other Animations:**
   - Try creating other animations such as solid colors, color wipes, theater chase, etc., by writing new functions and calling them in the `loop()`.