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
  static void write_data(int16_t* timeStamps, int16_t* weights, int16_t* parameters, int8_t devId);
  static void write_data_precise(int16_t* timeStamps, int16_t* weights, float* parameters, int8_t devId);
  static void read_data(const char* fileName);
};

#endif
