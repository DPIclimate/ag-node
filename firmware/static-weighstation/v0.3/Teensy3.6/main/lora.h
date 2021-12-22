/*
 * Static functions for communications between Feather M0 and node. 
 */

#ifndef LORA
#define LORA

// LoRaWAN library imports
#include "lmic.h"
#include <hal/hal.h>
#include <SPI.h>
#include "config.h"
#include "sensors.h"

// payload for things network
static osjob_t sendjob; 

class Lora {

public:

  static void init();
  static void request_send(int8_t* payload);
  static bool check_state();
  static void set_state(bool s);
};



#endif
