/* 
 * Memory Card functions for reading and writing data.
 */
  
#ifndef MEMORY_CARD
#define MEMORY_CARD

#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
#include "config.h"
#include "sensors.h"

class Memory {
  
public:
  static void init(); ///< Initailse SD-Card function
  static void write_weigh_data(int16_t* weights, 
		  int32_t* timeStamps, 
		  int8_t* payload, 
		  struct parameters_t &parameters); ///< Write data

  static void read_data(const char* fileName); ///< Read data (DEBUG)
};

#endif
