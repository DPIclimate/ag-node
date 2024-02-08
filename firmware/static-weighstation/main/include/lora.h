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
  static void init(); ///< Initialise Lora class
  static void test_connection(); ///< Send a test message over LoRaWAN
  static void request_send(int8_t* payload, 
		  uint8_t port); ///< Request payload be sent over LoRaWAN
  static bool check_state(); ///< Check if payload has been sent
  static void set_state(bool s); ///< Set the state of transmission
};

#endif
