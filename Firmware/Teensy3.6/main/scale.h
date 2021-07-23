#ifndef SCALE
#define SCALE

#include <Wire.h>
#include "Arduino.h"
#include "config.h"
#include "memory.h"
#include "lora.h"

// Interrupts 
#define WEIGH_SCALE_1 21
#define WEIGH_SCALE_2 20
#define WEIGH_SCALE_3 17


class Scale {
  public:
    static void init();
    static void request_event();
    static void extract_parameters(uint16_t* weights);
    static void extract_parameters(float* weights);
};


#endif
