/* 
 * Memory Card functions for reading and writing data.
 */
  
#ifndef MEMORY_CARD
#define MEMORY_CARD

#include "Arduino.h"
#include <SdFat.h>
#include <SPI.h>

class MemoryCard {
  
public:
  static void init();
  static void write_data(const char* fileName, int* xValues, int* yValues, unsigned int size, unsigned int captureTime);
  static void read_data(const char* fileName);
};

#endif
