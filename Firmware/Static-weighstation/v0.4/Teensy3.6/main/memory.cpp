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


void Memory::write_data(int16_t* timeStamps, int16_t* weights, int8_t* payload, struct parameters_t &parameters){
  root = SD.open(RAW_DATA_FILENAME, FILE_WRITE);
  Serial.println(parameters.scaleId);
  if(root){
    // Concatenate a single line string of data
    String concatData;
    concatData += (String)parameters.unixTime;
    concatData += ";";
    for(uint16_t i = 0; i < (RESPONSE_SIZE / 2); i++){
      concatData += (String)(timeStamps[i] / 1000.0f); 
      concatData += ",";
      concatData += (String)(weights[i] / 100.0f);
      concatData += ";";
    }
    root.println(concatData);
    root.close();
    #ifdef DEBUG
      Serial.println("[MEMORY]: RAW data written to SD card");
    #endif
  } else{
    #ifdef DEBUG
      Serial.println("[MEMORY]: Error writing RAW data to SD card");
    #endif
  }
  
  root = SD.open(PARAMETERS_FILENAME, FILE_WRITE);
  if(root){
    // Concatenate a single line string of data
    String concatData;
    concatData += (String)parameters.unixTime;
    concatData += ",";
    root.print(concatData);

    // Append raw payload
    for(uint8_t i = 0; i < sizeof(payload); i++){
      root.print((char)payload[i]);
    }

    // Parameters
    concatData += ",";
    concatData += (String)parameters.unixTime;
    concatData += ",";
    concatData += String(parameters.scaleId);
    concatData += ",";
    concatData += String((float)(parameters.startWeight / 100.0)); // convert to kg
    concatData += ",";
    concatData += String((float)(parameters.middleWeight / 100.0));
    concatData += ",";
    concatData += String((float)(parameters.endWeight / 100.0));
    concatData += ",";
    concatData += String((float)(parameters.avWeight / 100.0));
    concatData += ",";
    concatData += String((float)(parameters.deltaWeight / 100.0));
    concatData += ",";
    concatData += String((float)(parameters.timeOnScale / 1000.0)); // convert to ms

    root.close();
    #ifdef DEBUG
      Serial.println("[MEMORY]: Parameters written to SD card");
    #endif
  } else{
    #ifdef DEBUG
      Serial.println("[MEMORY]: Error writing RAW data to SD card");
    #endif
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
