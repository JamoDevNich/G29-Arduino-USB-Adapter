#include <Joystick.h>             // For Traditional input (see below)  
#include "KeyboardOutput.h"       // For OMSI 2 input (see below)
#include "GearshiftReaderG29.h"

#define SHIFTER_AXIS_X A0
#define SHIFTER_AXIS_Y A1
#define SHIFTER_BUTTON 10
#define NEUTRAL_GEAR 0

/****************************************************************************
 * G29 Arduino USB Adapter
 * by JamoDevNich <github@nich.dev>
 * 
 * Enables the use of the G29 "Driving Force" Gearshifter with a PC. Supports three profiles (Gamepad Manual, Keyboard OMSI Manual and Keyboard OMSI Automatic).
 * 
 * Version 0.1.1
 * 
 * Licensed under the GNU LGPL v3
 * 
 * (c) 2020
 ****************************************************************************/

byte currentShifterGear = 0;             // Gear currently selected on the shifter
byte currentActiveGear = 0;              // Gear currently being sent to the host
byte currentOutputModeSelected = 0;      // 1 - Traditional (Gamepad), 2 - OMSI2 Manual (Keyboard), 3 - OMSI2 Automatic
byte shifterPressCount = 0;              // To switch output mode, press the shifter three times. We count those each occurence here.
bool sendGearToSerial = true;

const char OMSIVehicleGearsManual [8] = {'n', '1', '2', '3', '4', '5', '6', 'r'};
const char OMSIVehicleGearsAutomatic [8] = {'n', 'd', 'd', 'd', 'r', 'd', 'd', 'r'};

Joystick_ Joystick;
KeyboardOutput KeyboardOut;
GearshiftReaderG29 GearshiftReader(SHIFTER_AXIS_X, SHIFTER_AXIS_Y, SHIFTER_BUTTON);

/**
 * Output to the Gamepad HID
 * 
 * This will output a each gear as a joystick button. The buttons will stay held down until the gear is changed.
 * Buttons 1-6 are sent for gears 1-6. Button 7 is sent for reverse. Neutral is not sent.
 */
void outputTraditionalJoystick() {
  if (currentShifterGear != currentActiveGear) {
    sendGearToSerial = true;
    if (currentActiveGear != NEUTRAL_GEAR) {
      Joystick.setButton(currentActiveGear-1, LOW);
    }
    if (currentShifterGear != NEUTRAL_GEAR) {
      Joystick.setButton(currentShifterGear-1, HIGH);
    }
    currentActiveGear = currentShifterGear;
  }
}


/**
 * Output to Keyboard HID: OMSI2 Manual
 * 
 * Sends a keyboard button press when the shifter position has changed e.g Gear 1 is '1', Reverse is 'r', Neutral is 'n'
 */
void outputKeyboardManual1() {
  if (currentShifterGear != currentActiveGear) {
    sendGearToSerial = true;
    KeyboardOut.appendButton(OMSIVehicleGearsManual[currentShifterGear]);
    currentActiveGear = currentShifterGear;
  }
}


/**
 * Output to Keyboard HID: OMSI2 Automatic
 * 
 * Sends a keyboard button press when the shifter position has changed. Up is 'd', Down is 'r', Neutral is 'n'
 */
void outputKeyboardAutomatic1() {
  if (currentShifterGear != currentActiveGear) {
    sendGearToSerial = true;
    KeyboardOut.appendButton(OMSIVehicleGearsAutomatic[currentShifterGear]);
    currentActiveGear = currentShifterGear;
  }
}


/**
 * Listen for gearshift presses
 * 
 * Pressing the gearshift three times will switch between the available modes.
 * If the gearshift is pressed less than three times, this is ignored.
 */
unsigned long lastShifterPressMillis = millis();
void handleModeSwitching() {
  if (GearshiftReader.readShifterButton(true)) {
    lastShifterPressMillis = millis();
    shifterPressCount += 1;
    sendGearToSerial = true;
  }

  // We wait for a second before processing the mode switch
  if (millis() - lastShifterPressMillis > 1000) {
    lastShifterPressMillis = millis();
    if (shifterPressCount > 1) {
      // Currently 3 available output modes, counted from 0 to 2.
      if (currentOutputModeSelected > 1) currentOutputModeSelected = 0;
      else currentOutputModeSelected += 1;
    }
    shifterPressCount = 0;
  }
}


/**
 * Switch gears
 * 
 * Selects a method to handle a gear change, depending on the output mode selected by the user.
 * To switch between available output modes, press the gearshift three times.
 */
unsigned long previousRunSetGears = millis();
void handleGearChanges() {
  if (millis() - previousRunSetGears > 10) {
    previousRunSetGears = millis();
    
    currentShifterGear = GearshiftReader.currentPosition();
    
    if (currentOutputModeSelected == 0) {
      outputTraditionalJoystick();
    }
    else if (currentOutputModeSelected == 1) {
      outputKeyboardManual1();
      KeyboardOut.processQueue();
    }
    else if (currentOutputModeSelected == 2) {
      outputKeyboardAutomatic1();
      KeyboardOut.processQueue();  // Duplication is intentional for readability
    }
  }
}


/**
 * Setup
 * 
 * Runs once on start-up
 */
void setup() { 
  Serial.begin(115200);
  Joystick.begin();
  KeyboardOut.begin();
  GearshiftReader.begin();
}

/**
 * Loop
 * 
 * Runs for the entire MCU lifecycle
 */
void loop() {
  handleGearChanges();
  handleModeSwitching();
  
  if (sendGearToSerial) {
    Serial.println("currentActiveGear:" + String(currentActiveGear) + ", currentShifterGear:" + String(currentShifterGear) + ", currentOutputModeSelected:" + String(currentOutputModeSelected) + ", shifterPressCount:" + String(shifterPressCount));
    sendGearToSerial = false;
  } 
}
