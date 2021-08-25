#ifndef SCALE
#define SCALE

#include <Wire.h>
#include "Arduino.h"
#include "config.h"
#include "memory.h"
#include "lora.h"


class Scale {
  public:
    static void init();
    static void one();
    static void two();
    static void three();
    static void request_event(uint8_t devId);
    static void extract_parameters(int16_t* weights, int8_t devId);
};


#endif
