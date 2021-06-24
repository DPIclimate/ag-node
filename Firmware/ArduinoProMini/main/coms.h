/*
 * Static functions for communications between Feather M0 and node. 
 */

#ifndef COMS
#define COMS

// LoRaWAN library imports
#include "lmic.h"
#include <hal/hal.h>
#include <SPI.h>

#include "scale.h"
#include "deviceid.h"

// payload for things network gateway
static osjob_t sendjob; 

// Schedule TX (transmit) every this many seconds (might become longer due to duty
// cycle limitations). If setTimedCalledBack() is run.
// const unsigned TX_INTERVAL = 10; // Not used in this weigh scale setup, data is only send when recorded

class Communications {

//  inline static uint8_t* payload;
  
public:
  
  static void init();
  static void request_send(uint8_t* payload);
  static void set_payload(uint8_t* payload);
  static uint8_t* get_payload();
  static bool check_state();
  static void set_state(bool s);
};



#endif
