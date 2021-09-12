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


class Lora {
  static const int8_t IRQ = 0;

public:
  static void init();
  static void test_connection();
  static void request_send(int8_t* payload, uint8_t port);
  static bool check_state();
  static void set_state(bool s);
};



#endif
