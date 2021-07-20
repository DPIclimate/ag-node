/* 
 * Memory Card functions for reading and writing data.
 */

#include "memorycard.h"

static const int chipSelect = BUILTIN_SDCARD;
File root;

void MemoryCard::init(){
  Serial.println("[MEMORY]: Intialising Memory Card...");
  if(!SD.begin(chipSelect)){
    Serial.println("[MEMORY]: Intialisation failed.");
    return;
  }
  else{
    Serial.println("[MEMORY]: Card Intialised");
  }
}


void MemoryCard::write_data(const char* fileName, float* xValues, float* yValues, unsigned int size, unsigned long captureTime){
  root = SD.open(fileName, FILE_WRITE);
  if(root){
    Serial.print("[MEMORY]: Writing to ");
    Serial.print(fileName);
    Serial.println(" on SD card.");

    // Compile a single line string of data
    String data;
    data += captureTime;
    data += ";";
    Serial.println(size);
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
    Serial.println(data);
    
    root.close();
    Serial.println("[MEMORY]: Data written to SD card");
  }
  else{
    Serial.println("[MEMORY]: Error writing to SD card");
  }
}


void MemoryCard::write_parameters(const char* fileName, byte* values){
  root = SD.open(fileName, FILE_WRITE);
  if(root){
    for(unsigned int i = 0; i < sizeof(values); i++){
      root.print(values[i]);
    }
    root.println();
    root.close();
  }
}


void MemoryCard::read_data(const char* fileName){
  root = SD.open(fileName, FILE_READ);
  if(root){
    Serial.println("[MEMORY]: Output from SD card below.");
    while(root.available()){
      Serial.write(root.read());
    }
    root.close();
  }
  else{
    Serial.println("[MEMORY]: Error reading from SD card.");
  }
}
