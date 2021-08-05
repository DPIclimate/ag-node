#include "scale.h"

static uint16_t weights[(RESPONSE_SIZE / 2)];
static uint16_t fweights[(RESPONSE_SIZE / 2)];
static uint16_t timeStamps[(RESPONSE_SIZE / 2)];
static uint16_t parameters[5]; // Stores parameters as uint16_t
static float floatParameters[5]; // Stores parameters as uint16_t
static uint8_t byteParameters[9]; // Stores parameters as uint8_t for LoRaWAN


void Scale::init(){
  Wire.begin();
}


void Scale::one(){
  #if DEBUG == 1
    Serial.println("Device one requests I2C transmission...");
  #endif
  request_event(1);
}


void Scale::two(){
  #if DEBUG == 1
    Serial.println("Device two requests I2C transmission...");
  #endif
  request_event(2);
}


void Scale::three(){
  #if DEBUG == 1
    Serial.println("Device three requests I2C transmission...");
  #endif
  request_event(3);
}


void Scale::request_event(uint8_t devId){
  noInterrupts();
  Serial.println("==== Response ====");
  const uint8_t PACKET_SIZE = 20;
  uint8_t packet[RESPONSE_SIZE * 2];

  bool deviceFound = false;
  int16_t index = 0;
  for(unsigned int x = 0; x < (sizeof(packet) / PACKET_SIZE); x++){
    Wire.requestFrom(devId, PACKET_SIZE);
    while(Wire.available()) {
      packet[index++] = Wire.read(); // Append each byte to the response
      deviceFound = true;
    }
  }

  if (deviceFound){
      uint16_t weightsPosition = 0;
      uint16_t timeStampsPosition = 0;
      uint16_t shift = 0;
      for(uint16_t i = 0; i < (sizeof(packet) / 2); i++){
        shift = i + i;
        if(i < (RESPONSE_SIZE / 2)){
          #ifdef HIGH_PRECISION
            fweights[weightsPosition] = ((packet[shift] << 8) | packet[shift + 1]) / 100.0;
          #else
            weights[weightsPosition] = (packet[shift] << 8) | packet[shift + 1];
          #endif
          weightsPosition++;
        } else{
          timeStamps[timeStampsPosition] = (packet[shift] << 8) | packet[shift + 1];
          timeStampsPosition++;
        }
      }
    } else {
      Serial.println("Device not found...");
  }

  #ifdef HIGH_PRECISION
    Serial.println("=== High Precision ===");
    Scale::extract_parameters(fweights, devId);
    Memory::write_data_precise(timeStamps, fweights, floatParameters, devId);
  #else
    Serial.println("=== Low Precision ===");
    Scale::extract_parameters(weights, devId);
    Memory::write_data(timeStamps, weights, parameters, devId);
  #endif

   Lora::request_send(byteParameters);

  #if DEBUG == 1
    for(int i = 0; i < (RESPONSE_SIZE / 2); i++){
      Serial.print(timeStamps[i]);
      Serial.print(": \t");
      #ifdef HIGH_PRECISION
        Serial.println((float)((fweights[i]) / 100.0));
        if(fweights[i] == 0 && i > 5){
          break;
        }
      #else 
        Serial.println(weights[i]);
        if(fweights[i] == 0 && i > 5){
          break;
        }
      #endif
    }
  #endif
  interrupts();
}


void Scale::extract_parameters(uint16_t* weights, uint8_t devId){
  // Low precision
  
  /* Put array in context (i.e. search for datapoints that are non-zero) 
   * Need to do this to calc start and finishing weight
   * Small buffer added at start to ignore 0's
   */
  unsigned int reading = 0;
  while(weights[reading] != 0 || reading <= 5){
    reading++;
    if(reading == (RESPONSE_SIZE / 2)){
      break;
    }
  }
  
  uint16_t oneQuarter = weights[uint16_t((float)reading * 0.25)];
  uint16_t halve = weights[uint16_t((float)reading * 0.50)];
  uint16_t threeQuarters = weights[uint16_t((float)reading * 0.75)];

  parameters[0] = oneQuarter;
  byteParameters[0] = oneQuarter >> 8;
  byteParameters[1] = oneQuarter;

  parameters[1] = halve;
  byteParameters[2] = halve >> 8;
  byteParameters[3] = halve;

  parameters[2] = threeQuarters; 
  byteParameters[4] = threeQuarters >> 8;
  byteParameters[5] = threeQuarters;

  unsigned long sum = 0;
  for(uint16_t i = (int)((float)reading * 0.25); i < (int)((float)reading * 0.75); i++){
    sum += weights[i];
  }

  uint16_t average = (sum / (((float)reading * 0.75 - (float)reading * 0.25)));

  parameters[3] = average;
  parameters[4] = (uint16_t)devId;
  byteParameters[6] = average >> 8;
  byteParameters[7] = average;
  byteParameters[8] = (uint8_t)devId;
}


void Scale::extract_parameters(float* fweights, uint8_t devId){
  // High precision

  
  unsigned int reading = 0;
  while((int)fweights[reading] != 0 || reading <= 5){
    reading++;
    if(reading == (RESPONSE_SIZE / 2)){
      break;
    }
  }
  
  uint16_t oneQuarter = fweights[uint16_t((float)reading * 0.25)];
  uint16_t halve = fweights[uint16_t((float)reading * 0.50)];
  uint16_t threeQuarters = fweights[uint16_t((float)reading * 0.75)];

  floatParameters[0] = oneQuarter / 100.0;
  byteParameters[0] = oneQuarter >> 8;
  byteParameters[1] = oneQuarter;

  floatParameters[1] = halve / 100.0;
  byteParameters[2] = halve >> 8;
  byteParameters[3] = halve;

  floatParameters[2] = threeQuarters / 100.0; 
  byteParameters[4] = threeQuarters >> 8;
  byteParameters[5] = threeQuarters;

  unsigned long sum = 0;
  for(uint16_t i = 0; i < (RESPONSE_SIZE / 2); i++){
    sum += weights[i];
  }

  uint16_t average = (sum / (((float)reading * 0.75 - (float)reading * 0.25)));
  
  floatParameters[3] = average / 100.0;
  floatParameters[4] = (float)devId;
  byteParameters[6] = average >> 8;
  byteParameters[7] = average;
  byteParameters[8] = devId;
}
