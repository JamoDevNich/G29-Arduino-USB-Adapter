#include <Arduino.h>
#include "GearshiftReaderG29.h"

/****************************************************************************
 * GearshiftReaderG29
 * Arduino Library
 * by JamoDevNich <github@nich.dev>
 * 
 * Version 0.1.0
 * 
 * Licensed under the GNU LGPL v3
 * 
 * (c) 2020
 ****************************************************************************/

/**
 * Read shifter position
 * 
 * Reads the ADC values from the gear shifter and returns a position 1-6. It is recommended to use currentPosition() instead.
 * 
 * @return byte Current gear position 0-6
 */
byte GearshiftReaderG29::readShifterPosition() {
  unsigned short shifterAxisX=analogRead(this->analogPinAxisX);
  unsigned short shifterAxisY=analogRead(this->analogPinAxisY);

  // Top row of gears
  if (shifterAxisY > 700) {
    // Rows 1-3 respectively
    if (shifterAxisX < 365) return 1;
    else if (shifterAxisX < 580) return 3;
    else return 5;
  }
  
  // Bottom row of gears
  else if (shifterAxisY < 300) {
    // Rows 1-3 respectively
    if (shifterAxisX < 365) return 2;
    else if (shifterAxisX < 580) return 4;
    else return 6;
  }

  return 0;
}

/**
 * Read shifter button
 * 
 * Check if the gearshifter button is being pressed. This disables debounce, so if the gearshifter is being held down it will continously register button presses.
 * 
 * @return bool True if gearshift is pressed, otherwise false
 */
bool GearshiftReaderG29::readShifterButton() {
  return this->readShifterButton(false);
}

/**
 * Read shifter button
 * 
 * Check if the gearshifter button is being pressed.
 * 
 * @param debounce When set to true, pressing the gearshifter will only register a press once. The gearshifter must be released to send further button presses.
 * @return bool True if gearshift is pressed, otherwise false
 */
bool GearshiftReaderG29::readShifterButton(bool debounce) {
  bool shifterPressed = digitalRead(this->digitalPinButton);
  if (debounce && this->shifterWasPreviouslyHeldDown && shifterPressed) {
    shifterPressed = false;
  }
  else {
    this->shifterWasPreviouslyHeldDown = shifterPressed;
  }
  return shifterPressed;
}

/**
 * Library providing quick access to the Logitech G29 Shifter
 * 
 * @param analogPinAxisX the ADC pin connected to the gearshifter X axis
 * @param analogPinAxisY the ADC pin connected to the gearshifter Y axis
 * @param digitalPinButton the digital pin connected to the gearshifter button
 */
GearshiftReaderG29::GearshiftReaderG29(short analogPinAxisX, short analogPinAxisY, short digitalPinButton) {
  this->analogPinAxisX = analogPinAxisX;
  this->analogPinAxisY = analogPinAxisY;
  this->digitalPinButton = digitalPinButton;
}

/**
 * Setup inputs
 * 
 * Set pinmodes on the provided inputs
 */
void GearshiftReaderG29::begin() {
  pinMode(this->analogPinAxisX, INPUT_PULLUP);
  pinMode(this->analogPinAxisY, INPUT_PULLUP);
  pinMode(this->digitalPinButton, INPUT);
  this->initialised = true;
}

/**
 * Read shifter position (including Reverse)
 * 
 * Reads the current shifter position. Contains debounce to prevent rapid gear flickering when switching from reverse to neutral.
 * 
 * @return byte Current gear position 0-7
 */
byte GearshiftReaderG29::currentPosition() {
  if (!this->initialised) return 0;
  
  byte currentPosition = this->readShifterPosition();
  
  if (currentPosition == 6) {
    if (this->readShifterButton()) currentPosition = 7;
    else if (this->lastShifterPosition == 7) currentPosition = 7; // Fix debounce When switching from Reverse to Neutral. We can not go from Reverse to 6!
  }

  this->lastShifterPosition = currentPosition;
  return currentPosition;
}