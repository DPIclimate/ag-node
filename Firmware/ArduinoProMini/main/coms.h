/*
 * Static functions for communications between Feather M0 and node. 
 */

#ifndef COMS
#define COMS

// LoRaWAN library imports
#include <arduino_lmic.h>
#include <hal/hal.h>
#include <SPI.h>
//#include "scale.h"

#ifndef DEVICEID
#define DEVICEID

// Application unique identifier (Format = LSB)
static const u1_t PROGMEM APPEUI[8] = { 0x22, 0xFD, 0x03, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };

// Device unique identifier (Format = LSB)
static const u1_t PROGMEM DEVEUI[8] = { 0x47, 0xC1, 0x11, 0x00, 0x00, 0xB6, 0x76, 0x98 };

// APP-KEY (Sensitive - Remove if going public) (Format = MSB)
static const u1_t PROGMEM APPKEY[16] = { 0x8C, 0x41, 0x2C, 0xEF, 0x73, 0x63, 0x49, 0xF1, 0x4F, 0xAF, 0xF6, 0x6F, 0x2E, 0x0E, 0x46, 0xD4 };

#endif

// payload for things network gateway
static osjob_t sendjob; 

class Communications {
  
public:
  inline static bool state = false;
  static void init();
  static void request_send(uint8_t* payload);
  static void set_payload(uint8_t* payload);
  static uint8_t* get_payload();
};


#endif
