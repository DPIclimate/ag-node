/* 
 * Memory Card functions for reading and writing data.
 */
  
#ifndef MEMORY_CARD
#define MEMORY_CARD

#include "Arduino.h"
#include <SD.h>
#include <SPI.h>

class MemoryCard {
  
public:
  static void init();
  static void write_data(const char* fileName, float* xValues, float* yValues, unsigned int size, unsigned long captureTime);
  static void write_parameters(const char* fileName, byte* values);
  static void read_data(const char* fileName);
};

#endif
