# G29 Arduino USB Adapter

Use the G29 "Driving Force" gearshifter on a PC with support for three input profiles

Version 0.1.2

## Getting Started
### Requirements
The Joystick library must be in your Arduino Libraries folder. It can be downloaded from here: https://github.com/MHeironimus/ArduinoJoystickLibrary. You'll also need an Arduino with support for USB, such as the Arduino Leonardo.

### Hardware
This was developed on an Arduino Pro Micro. The table below contains the pinout for the connector on the G29 Shifter, as well as the Arduino.

| DB9 Pin Number | Function | Arduino Pro Micro Pin |
|----------------|--------------|----------------|
| 1 |   | |
| 2 | Reverse Sw | 10 |
| 3 | VCC | |
| 4 | X Axis Potentiometer | A0 |
| 5 |   | |
| 6 | GND | |
| 7 | VCC | |
| 8 | Y Axis Potentiometer | A1 |
| 9 |   | |

## Functions
### Switching profiles
Switching profiles is done by pressing the gearshifter down three times. There are three included profiles:
1. Gamepad Input - each gear position (except Neutral) is mapped to buttons 1-7 **(Default)**
2. Keyboard Input, OMSI Manual - each gear position is mapped to numbers 1-6 and letters 'r' and 'n'
3. Keyboard Input, OMSI Automatic - Each gear position is mapped to letters 'r', 'n' and 'd'

Adding new profiles will require you to get your hands dirty in code. If help is needed feel free to open a new issue.
