/* 
 * Memory Card functions for reading and writing data.
 */

#include "memorycard.h"

static const int chipSelect = 16;

File root;
SdFat SD;

void MemoryCard::init(){
  SD.begin(chipSelect);
}


void MemoryCard::write_data(const char* fileName, int* xValues, int* yValues, unsigned int size, unsigned int captureTime){
  root = SD.open(fileName, FILE_WRITE);
  if(root){
    // Compile a single line string of data
    String data;
    data += captureTime;
    data += ";";
    for(unsigned int i = 0; i < size; i++){
      if(int(xValues[i]) == 0 && int(yValues[i]) == 0){
        break;
      }
      data += String(xValues[i]);
      data += ",";
      data += String(yValues[i]);
      data += ";";
    }
    root.println(data);
    root.close();
  }
}


void MemoryCard::read_data(const char* fileName){
  root = SD.open(fileName, FILE_READ);
  if(root){
    while(root.available()){
      Serial.write(root.read());
    }
    root.close();
  }
}
