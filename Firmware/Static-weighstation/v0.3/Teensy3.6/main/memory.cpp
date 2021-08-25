/* 
 * Memory Card functions for reading and writing data.
 */

#include "memory.h"

static const int chipSelect = BUILTIN_SDCARD;
File root;


void Memory::init(){
  Serial.println("[MEMORY]: Intialising Memory Card...");
  if(!SD.begin(chipSelect)){
    Serial.println("[MEMORY]: Intialisation failed.");
    return;
  }
  else{
    Serial.println("[MEMORY]: Card Intialised");
  }
}


void Memory::write_data(int16_t* timeStamps, int16_t* weights, int16_t* parameters, int8_t devId){
  root = SD.open(FILENAME, FILE_WRITE);
  if(root){
    // Concatenate a single line string of data
    String concatData;
    concatData += (String)devId;
    for(uint16_t i = 0; i < (RESPONSE_SIZE / 2); i++){
      concatData += (String)(timeStamps[i] / 1000.0);
      concatData += ",";
      concatData += (String)weights[i];
      concatData += ";";
    }
    concatData += "Start: " + (String)parameters[0];
    concatData += ", Middle: " + (String)parameters[1];
    concatData += ", End: " + (String)parameters[2];
    concatData += ", Average: " + (String)parameters[3];
    
    root.println(concatData);
    root.close();
    Serial.println("[MEMORY]: Data written to SD card");
  }
  else{
    Serial.println("[MEMORY]: Error writing to SD card");
  }
}


void Memory::write_data_precise(int16_t* timeStamps, int16_t* weights, float* parameters, int8_t devId){
  root = SD.open(FILENAME, FILE_WRITE);
  if(root){
    // Concatenate a single line string of data
    String concatData;
    concatData += (String)devId;
    for(uint16_t i = 0; i < (RESPONSE_SIZE / 2); i++){
      concatData += (String)(timeStamps[i] / 1000.0);
      concatData += ",";
      concatData += (String)((float)weights[i] / 100.0) ;
      concatData += ";";
    }
    concatData += "Start: " + (String)parameters[0];
    concatData += ", Middle: " + (String)parameters[1];
    concatData += ", End: " + (String)parameters[2];
    concatData += ", Average: " + (String)parameters[3];
    
    root.println(concatData);
    root.close();
    Serial.println("[MEMORY]: Data written to SD card");
  }
  else{
    Serial.println("[MEMORY]: Error writing to SD card");
  }
}



void Memory::read_data(const char* fileName){
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
