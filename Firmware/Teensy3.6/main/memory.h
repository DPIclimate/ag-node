/* 
 * Memory Card functions for reading and writing data.
 */
  
#ifndef MEMORY_CARD
#define MEMORY_CARD

#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
#include "config.h"

class Memory {
  
public:
  static void init();
  static void write_data(uint16_t* timeStamps, uint16_t* weights, uint16_t* parameters, uint8_t devId);
  static void write_data(uint16_t* timeStamps, float* weights, float* parameters, uint8_t devId);
  static void read_data(const char* fileName);

  //
};

#endif
