# ESP32-Pixmob-IR-Audiosync

An ESP32-based controller for Pixmob LED devices that creates synchronized light shows at home. This project allows you to control Pixmob LED devices (commonly used in concerts and events) through either sound detection or Bluetooth commands.

## Overview

This project uses an ESP32 microcontroller to send infrared signals to Pixmob LED devices, creating synchronized light shows. The system supports two modes of operation:
- **Sound-activated mode**: Automatically triggers random colour changes when sound is detected
- **Bluetooth control mode**: Manual control via Bluetooth commands

## Hardware Requirements

### Required Components
- **ESP32 Development Board** (any ESP32 variant)
- **IR LED** (KY-005 or HW489) - Connected to GPIO pin 14
- **Sound Sensor Module** (KY-038 or HW485) - Connected to GPIO pin 27
- **Breadboard and jumper wires**
- **Resistors** (220Ω for IR LED, 10kΩ pull-up for sound sensor)

### Circuit Connections

```
ESP32 Pin    Component
---------    ---------
GPIO 14      IR LED (anode) + 220Ω resistor to 3.3V
GPIO 27      Sound Sensor DO pin
3.3V         Sound Sensor VCC
GND          Sound Sensor GND, IR LED cathode
```

### Optional Components
- **Bluetooth-enabled device** (phone, tablet, computer) for manual control
- **Pixmob LED devices** (the target devices to control)

## Software Requirements

- **PlatformIO** or **Arduino IDE**
- **ESP32 Board Package**
- **Required Libraries**:
  - `IRremoteESP8266` (for IR transmission)
  - `BluetoothSerial` (for Bluetooth communication)

## Installation

1. Clone this repository
2. Open the project in PlatformIO or Arduino IDE
3. Install the required libraries
4. Upload the code to your ESP32
5. Connect the hardware according to the circuit diagram above

## Usage

### Sound-Activated Mode (Default)
When no Bluetooth device is connected, the system automatically detects sound and triggers random colour changes on the Pixmob devices. Simply play music or make noise near the sound sensor.

### Bluetooth Control Mode
1. Connect to the ESP32 via Bluetooth (device name: "ESP32-Bluetooth")
2. Send colour commands as text strings:
   - `red`, `red_2`, `red_3`, `red_4`, `red_5`
   - `green`, `grn_2`, `grn_3`, etc.
   - `blue`, `blue_2`, `blue_3`, etc.
   - `yellow`, `yellow_2`, `yellow_3`, etc.
   - `pink`, `pink_2`, `pink_3`, etc.
   - `orange`, `orange_2`, `orange_3`, etc.
   - `magenta`, `mag_2`, `mag_3`, `mag_4`
   - `whitish`, `whitish_2`, `whitish_3`, etc.
   - `turquoise`, `turquoise_2`, `turquoise_3`, `turquoise_4`
   - And many more variants...

## IR Code Source

The IR codes used in this project are sourced from the [Daniel Weidman's Flipper Pixmob IR Codes repository](https://github.com/danielweidman/flipper-pixmob-ir-codes), providing compatibility with a wide range of Pixmob LED devices.

## Troubleshooting

- **No IR signals**: Check IR LED connections and ensure it's properly connected to GPIO 14
- **Sound sensor not working**: Verify connections to GPIO 27 and check sensor sensitivity
- **Bluetooth not connecting**: Ensure your device supports Bluetooth Classic (not just BLE)
- **Pixmob devices not responding**: Make sure the IR LED is pointed at the devices and they're within range

## TODO

- Add fade codes for colours