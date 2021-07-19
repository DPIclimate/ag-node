/*
 * Static functions for communications between Feather M0 and node. 
 */

#ifndef COMS
#define COMS

// LoRaWAN library imports
#include "lmic.h"
#include <hal/hal.h>
#include <SPI.h>

// Application unique identifier (Format = LSB)
static const u1_t PROGMEM APPEUI[8] = { 0x7C, 0x33, 0x04, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };

// Device unique identifier (Format = LSB)
static const u1_t PROGMEM DEVEUI[8] = { 0x36, 0xF4, 0x1E, 0x45, 0x44, 0x45, 0xE8, 0x00 };

// APP-KEY (Sensitive - Remove if going public) (Format = MSB)
static const u1_t PROGMEM APPKEY[16] = { 0x22, 0x2B, 0x4C, 0x19, 0x68, 0x16, 0x7F, 0x44, 0x01, 0x41, 0x2A, 0x01, 0x4F, 0x37, 0x68, 0xCE };

// payload for things network
static osjob_t sendjob; 

class Communications {

public:

  static void init();
  static void request_send(uint8_t* payload);
  static void set_payload(uint8_t* payload);
  static uint8_t* get_payload();
  static bool check_state();
  static void set_state(bool s);
};



#endif
