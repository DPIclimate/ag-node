#ifndef I2C
#define I2C

#include <Wire.h>
#include <Arduino.h>
#include "scale.h"

#define DEVICE_ID 1 // Specific to each weigh scale

#define WAKE_UP 11 // Output for interrupt

class ISquaredC {
  
  public:
    inline static uint16_t weightsPosition = 0;
    inline static uint16_t timeStampsPosition = 0;
    inline static const uint16_t RESPONSE_SIZE = 400;
    static void init();
    static void respond();
    static void request_transmission();
};


#endif
