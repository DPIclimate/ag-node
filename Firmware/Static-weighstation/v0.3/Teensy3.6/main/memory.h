/* 
 * Memory Card functions for reading and writing data.
 */
  
#ifndef MEMORY_CARD
#define MEMORY_CARD

// External
#include <SD.h>
#include <SPI.h>

// Internal
#include "config.h"
#include "Arduino.h"

class Memory {
  
public:
  static void init();
  static void write_data(int16_t* timeStamps, int16_t* weights, int16_t* parameters, uint8_t devId);
  static void read_data(const char* fileName);
};

#endif
