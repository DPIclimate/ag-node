/* 
 * Memory Card functions for reading and writing data.
 */

#include "memory.h"


static const int chipSelect = BUILTIN_SDCARD;
File root;

void Memory::init(){
  /*
   * Initialise memory card to see if it is plugged in and available.
   */
  Serial.print("[MEMORY]: Intialising memory card... ");
  if(!SD.begin(chipSelect)){
    Serial.println("failed");
    return;
  }
  else{
    Serial.println("success");
  }
}


void Memory::write_weigh_data(int16_t* weights, int32_t* timeStamps, int8_t* payload, struct parameters_t &parameters){
  /*
   * Write weights and timestamps to build in SD-card. Also appends caluclated parameters.
   * @param weights Weights array from scale.
   * @param timeStamps Corresponding timestamps for each weight reading.
   * @param payload Raw payload to add to table.
   * @param parameters Parameters struct with calculated variables from corresponding scale reading.
   */
  // Open SD card file
  root = SD.open(RAW_DATA_FILENAME, FILE_WRITE);
  if(root){
    // Concatenate a single line string of data
    String concatData;
    root.println((String)parameters.unixTime);
    // Loop over arrays and append to file
    for(uint16_t i = 0; i < sizeof(timeStamps) / sizeof(timeStamps[0]); i++){
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

  // Do the same as above to all parameters (in a seperate file) 
  root = SD.open(PARAMETERS_FILENAME, FILE_WRITE);
  if(root){
    // Concatenate a single line string of data
    String concatData;
    concatData += (String)parameters.unixTime;
    concatData += ",";

    // Append raw payload
    for(uint8_t i = 0; i < WEIGH_PAYLOAD_SIZE; i++){
      if (i != 0) {
        concatData += '-';
      }
      payload[i] &= 0xff;
      if (payload[i] < 16){
        concatData += '0';
      }
      concatData += String(payload[i], HEX);
    }

    // Add parameters to concatenated string 
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
    concatData += String((float)(parameters.timeOnScale / 1000.0)); // convert to seconds
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
  /*
   * Prints out contents of file to serial monitor.
   * @param fileName Name of .txt file.
   */
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
