#include "scale.h"

static uint16_t weights[(RESPONSE_SIZE / 2)];
static uint16_t timeStamps[(RESPONSE_SIZE / 2)];
static uint16_t parameters[5]; // Stores parameters as uint16_t
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
          weights[weightsPosition] = (packet[shift] << 8) | packet[shift + 1];
          weightsPosition++;
        } else{
          timeStamps[timeStampsPosition] = (packet[shift] << 8) | packet[shift + 1];
          timeStampsPosition++;
        }
      }
    } else {
      Serial.println("Device not found...");
  }

  Scale::extract_parameters(weights, devId);
  Memory::write_data(timeStamps, weights, parameters, devId);
  Lora::request_send(byteParameters);

  #if DEBUG == 1
    for(int i = 0; i < (RESPONSE_SIZE / 2); i++){
      Serial.print(timeStamps[i]);
      Serial.println(weights[i]);
      if(weights[i] == 0 && i > 5){
        break;
      }
    }
  #endif
  interrupts();
}


void Scale::extract_parameters(uint16_t* weights, uint8_t devId){
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
