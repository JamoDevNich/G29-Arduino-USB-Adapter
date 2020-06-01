#include <Arduino.h>
#include <Keyboard.h>
#include "KeyboardOutput.h"

/****************************************************************************
 * KeyboardOutput
 * Arduino Library
 * by JamoDevNich <github@nich.dev>
 *
 * Allows key presses to be processed asynchronously. Instead of using a delay
 * to virtually press and release a key, it is placed into a queue. A separate
 * method running in the 'main' loop will check for pending keys in the queue
 * and handle pressing and releasing of these keys using 'millis'. This is
 * because OMSI is unable to detect a keypress quickly.
 *
 * Version 0.1.1
 *
 * Licensed under the GNU LGPL v3
 *
 * (c) 2020
 ****************************************************************************/

KeyboardOutput::KeyboardOutput() {}

/*
 * Call begin on Arduino's 'Keyboard' class.
 */
void KeyboardOutput::begin() {
  Keyboard.begin();
}

/*
 * Append a button to the keypress queue
 *
 * Pushes a provided button onto the internal button queue. This button will be
 * pressed at the next available opportunity.
 *
 * @param char button Button to insert into internal queue.
 */
void KeyboardOutput::appendButton(char button) {
  this->buttonQueue.push(button);
}

/*
 * Process the keypress queue
 *
 * Checks for pending keys in the internal queue and presses them at the next
 * available opportunity. This must be run in the main loop of the application.
 */
void KeyboardOutput::processQueue() {
  // If there is a button in the Queue
  if (this->buttonQueue.count() > 0) {
    // If we are not currently pressing a button
    if (!this->buttonIsPressed) {
      Keyboard.press(this->buttonQueue.peek());
      this->buttonIsPressed = true;
      this->buttonLastPressed = millis();
    }
    // If we are currently pressing a button
    else {
      if (millis() - this->buttonLastPressed > 45) { // To decrease the time a key is held down, modify '45' here.
        this->buttonQueue.pop();
        Keyboard.releaseAll();
        this->buttonIsPressed = false;
      }
    }
  }
}
