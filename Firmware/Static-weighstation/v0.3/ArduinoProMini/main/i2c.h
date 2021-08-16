#ifndef I2C
#define I2C

#include <Wire.h>
#include <Arduino.h>
#include "scale.h"
#include "config.h"



class ISquaredC {
  
  public:
    inline static uint16_t weightsPosition = 0; // Position in weight array for multiple i2c messages
    inline static uint16_t timeStampsPosition = 0; // Position in timestamp array for multiple i2c messages
    inline static const uint16_t RESPONSE_SIZE = 400;
    static void init();
    static void respond();
    static void request_transmission();
};


#endif
