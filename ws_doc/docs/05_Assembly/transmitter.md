# Building the Transmitter with Arduino Nano, MPU6050, and nRF24L01

This tutorial will guide you through assembling a transmitter using an Arduino Nano, MPU6050 sensor, and nRF24L01 module. This transmitter will send data to the robot we assembled in the previous tutorial.

## Components Needed

- Arduino Nano
- Breadboard
- MPU6050 Sensor
- nRF24L01 Module
- Jumper wires
- 9V battery (or appropriate power supply)

## Tools Needed

- Screwdriver
- Double-sided tape or mounting brackets
- Wire cutters/strippers
- Soldering iron (optional for more secure connections)

## Step-by-Step Assembly

### Step 1: Prepare the Breadboard

1. **Place the Arduino Nano on the Breadboard**:
   - Ensure it is placed in a central location for easy access to all pins.

### Step 2: Wiring the Components on the Breadboard

1. **Connect the nRF24L01 Module to the Arduino Nano**:
   - **VCC** (3.3V) on nRF24L01 to **3.3V** on Arduino Nano
   - **GND** on nRF24L01 to **GND** on Arduino Nano
   - **CE** to **D9** on Arduino Nano
   - **CSN** to **D10** on Arduino Nano
   - **SCK** to **D13** on Arduino Nano
   - **MOSI** to **D11** on Arduino Nano
   - **MISO** to **D12** on Arduino Nano

2. **Connect the MPU6050 Sensor to the Arduino Nano**:
   - **VCC** to **5V** on the Arduino Nano
   - **GND** to **GND** on the Arduino Nano
   - **SCL** to **A5** on the Arduino Nano
   - **SDA** to **A4** on the Arduino Nano

### Step 3: Powering Up

1. **Connect the Battery**:
   - Connect the positive terminal of the battery to the **VIN** pin on the Arduino Nano.
   - Connect the negative terminal of the battery to the **GND** pin on the Arduino Nano.

### Step 4: Final Check

1. **Inspect Wiring**:
   - Double-check all connections to ensure they are correct and secure.
   - Ensure there are no short circuits.

2. **Test the Setup**:
   - Power on the Arduino Nano and check if the MPU6050 sensor and nRF24L01 module are receiving power.

## Diagram

Here's a simplified wiring diagram to help with the assembly:

```
Breadboard Power Rails:          Arduino Nano          nRF24L01          MPU6050
-----------------------          -------------         ---------         -------
(3.3V Rail) --- (3.3V)           (3.3V) --- (VCC)      (VCC) --- (3.3V)  (VCC) --- (5V)
(GND Rail) --- (GND)             (GND) --- (GND)       (GND) --- (GND)   (GND) --- (GND)
                                 (D9)  --- (CE)
                                 (D10) --- (CSN)
                                 (D13) --- (SCK)
                                 (D11) --- (MOSI)
                                 (D12) --- (MISO)
                                 (A5)  --- (SCL)       (SCL) --- (A5)
                                 (A4)  --- (SDA)       (SDA) --- (A4)
```

## Tips

- Use zip ties or cable organizers to keep the wiring neat and secure.
- Ensure the power supply can handle the current requirements of the components.
- If using a breadboard, ensure all connections are firm to avoid intermittent issues.

## Troubleshooting

- **No Power**: Check the battery connections and ensure the power switch (if any) is turned on.
- **No Sensor Data**: Verify the connections between the MPU6050 and the Arduino Nano.
- **Intermittent Connections**: Ensure all jumper wires are firmly connected and not loose.

This guide should help you assemble your transmitter with the components discussed. Once assembled, you can proceed with programming the Arduino Nano to send data from the MPU6050 sensor using the nRF24L01 module. Happy building!