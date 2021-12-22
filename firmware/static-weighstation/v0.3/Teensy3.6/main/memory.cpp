/* 
 * Memory Card functions for reading and writing data.
 */

#include "memory.h"

static const int chipSelect = BUILTIN_SDCARD;
File root;


void Memory::init(){
  #ifdef DEBUG
    Serial.println("[MEMORY]: Intialising Memory Card...");
  #endif
  if(!SD.begin(chipSelect)){
    #ifdef DEBUG
      Serial.println("[MEMORY]: Intialisation failed.");
    #endif
    return;
  }
  else{
    #ifdef DEBUG
      Serial.println("[MEMORY]: Card Intialised");
    #endif
  }
}


void Memory::write_data(int16_t* weights, int16_t* timeStamps, int8_t* payload, struct parameters_t &parameters){
  root = SD.open(RAW_DATA_FILENAME, FILE_WRITE);
  if(root){
    // Concatenate a single line string of data
    String concatData;
    root.println((String)parameters.unixTime);
    for(uint16_t i = 0; i < (RESPONSE_SIZE / 2); i++){
      if(timeStamps[i] == 0) break;
      String row;
      row += (String)(timeStamps[i] / 1000.0f); 
      row += ",";
      row += (String)(weights[i] / 100.0f);
      row += ";";
      root.println(row);
    }
    root.close();
    #ifdef DEBUG
      Serial.print("[MEMORY]: RAW data written to ");
      Serial.println(RAW_DATA_FILENAME);
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

    // Append raw payload
    for(uint8_t i = 0; i < PAYLOAD_SIZE; i++){
      if (i != 0) {
        concatData += '-';
      }
      payload[i] &= 0xff;
      if (payload[i] < 16){
        concatData += '0';
      }
      concatData += String(payload[i], HEX);
    }

    // Parameters
    concatData += ",";
    concatData += String(parameters.scaleID);
    concatData += ",";
    concatData += String((float)(parameters.startWeight / 100.0)); // convert to kg
    concatData += ",";
    concatData += String((float)(parameters.middleWeight / 100.0));
    concatData += ",";
    concatData += String((float)(parameters.endWeight / 100.0));
    concatData += ",";
    concatData += String((float)(parameters.avWeight / 100.0));
    concatData += ",";
    concatData += String((float)(parameters.stdevWeight / 100.0));
    concatData += ",";
    concatData += String((float)(parameters.deltaWeight / 100.0));
    concatData += ",";
    concatData += String((float)(parameters.timeOnScale / 1000.0)); // convert to ms
    root.println(concatData);
    
    root.close();
    #ifdef DEBUG
      Serial.print("[MEMORY]: Parameters data written to ");
      Serial.println(PARAMETERS_FILENAME);
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
    Serial.print("[MEMORY]: Output from ");
    Serial.print(fileName);
    Serial.println(" below.");
    while(root.available()){
      Serial.write(root.read());
    }
    root.close();
  } 
  else{
    Serial.println("[MEMORY]: Error reading from SD card.");
  }
  Serial.println("");
}
