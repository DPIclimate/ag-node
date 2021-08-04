#include "scale.h"

static uint16_t weights[(RESPONSE_SIZE / 2)];
static float fweights[(RESPONSE_SIZE / 2)];
static uint16_t timeStamps[(RESPONSE_SIZE / 2)];
static uint16_t parameters[4]; // Stores parameters as uint16_t
static float floatParameters[4]; // Stores parameters as uint16_t
static uint8_t byteParameters[8]; // Stores parameters as uint8_t for LoRaWAN


void Scale::init(){
  Wire.begin();
}


void Scale::one(){
  request_event(1);
}


void Scale::two(){
  request_event(2);
}


void Scale::three(){
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
          #if HIGH_PRECSION
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

  #if HIGH_PRECSION
    Scale::extract_parameters(fweights);
    Memory::write_data(timeStamps, fweights, parameters, devId);
  #else
    Scale::extract_parameters(weights);
    Memory::write_data(timeStamps, weights, parameters, devId);
  #endif

   Lora::request_send(byteParameters);

  #if DEBUG == 2
    for(int i = 0; i < (RESPONSE_SIZE / 2); i++){
      Serial.print(timeStamps[i]);
      Serial.print(": \t");
      Serial.println(weights[i]);
    }
  #endif

  interrupts();
}


void Scale::extract_parameters(uint16_t* weights){

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

  #if DEBUG == 1
    oneQuarter = 10;
    halve = 20;
    threeQuarters = 30;
  #endif

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

  #if DEBUG == 1
    Serial.println(reading);
    average = 40;
  #endif
  
  parameters[3] = average;
  byteParameters[6] = average >> 8;
  byteParameters[7] = average;
}


void Scale::extract_parameters(float* fweights){
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

  floatParameters[0] = oneQuarter;
  byteParameters[0] = oneQuarter >> 8;
  byteParameters[1] = oneQuarter;

  floatParameters[1] = halve;
  byteParameters[2] = halve >> 8;
  byteParameters[3] = halve;

  floatParameters[2] = threeQuarters; 
  byteParameters[4] = threeQuarters >> 8;
  byteParameters[5] = threeQuarters;

  unsigned long sum = 0;
  for(uint16_t i = 0; i < (RESPONSE_SIZE / 2); i++){
    sum += weights[i];
  }

  uint16_t average = (sum / (((float)reading * 0.75 - (float)reading * 0.25)));
  
  floatParameters[3] = average;
  byteParameters[6] = average >> 8;
  byteParameters[7] = average;
}
