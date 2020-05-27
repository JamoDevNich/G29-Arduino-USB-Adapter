#ifndef _HEADER_KEYBOARDOUTPUT_H
#define _HEADER_KEYBOARDOUTPUT_H

#include "Queue.h"

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
 * Version 0.1.0
 * 
 * Licensed under the GNU LGPL v3
 * 
 * (c) 2020
 ****************************************************************************/

class KeyboardOutput {
  private:
    Queue<char> buttonQueue = Queue<char>(10);
    unsigned long buttonLastPressed = 0;
    bool buttonIsPressed = false;
    
  public:
    KeyboardOutput();
    void begin();
    void appendButton(char button);
    void processQueue();
};

#endif
