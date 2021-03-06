#ifndef _HEADER_GEARSHIFTREADERG29_H
#define _HEADER_GEARSHIFTREADERG29_H

/****************************************************************************
 * GearshiftReaderG29
 * Arduino Library
 * by JamoDevNich <github@nich.dev>
 * 
 * Version 0.1.2
 * 
 * Licensed under the GNU LGPL v3
 * 
 * (c) 2020
 ****************************************************************************/

class GearshiftReaderG29 {
  private:
    bool initialised = false;
    unsigned short analogPinAxisX = 0;
    unsigned short analogPinAxisY = 0;
    unsigned short digitalPinButton = 0;
    unsigned short shifterAxisX = 0;
    unsigned short shifterAxisY = 0;
    byte lastShifterPosition = 0;
    bool shifterWasPreviouslyHeldDown = false;
    bool shifterIsConnected = false;

    byte readShifterPosition();
    
  public:
    GearshiftReaderG29(short analogPinAxisX, short analogPinAxisY, short digitalPinButton);
    void begin();
    void readInputs();
    byte currentPosition();
    bool readShifterButton();
    bool readShifterButton(bool debounce);
    bool shifterConnected();
};

#endif
