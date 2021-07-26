#include "scale.h"

static uint16_t weights[(RESPONSE_SIZE / 2)];
static float fweights[(RESPONSE_SIZE / 2)];
static uint16_t timeStamps[(RESPONSE_SIZE / 2)];
static uint16_t parameters[4]; // Stores parameters as uint16_t
static float floatParameters[4]; // Stores parameters as uint16_t
static uint8_t byteParameters[8]; // Stores parameters as uint8_t for LoRaWAN


void Scale::init(){
  Wire.begin();
  attachInterrupt(WEIGH_SCALE_1, one, RISING);
  attachInterrupt(WEIGH_SCALE_2, two, RISING);
  attachInterrupt(WEIGH_SCALE_3, three, RISING);
}


void Scale::one(){
  detachInterrupt(digitalPinToInterrupt(WEIGH_SCALE_2));
  detachInterrupt(digitalPinToInterrupt(WEIGH_SCALE_3));
  request_event(1);
  attachInterrupt(WEIGH_SCALE_2, two, RISING);
  attachInterrupt(WEIGH_SCALE_3, three, RISING);
}


void Scale::two(){
  detachInterrupt(digitalPinToInterrupt(WEIGH_SCALE_1));
  detachInterrupt(digitalPinToInterrupt(WEIGH_SCALE_3));
  request_event(2);
  attachInterrupt(WEIGH_SCALE_1, one, RISING);
  attachInterrupt(WEIGH_SCALE_3, three, RISING);
}


void Scale::three(){
  detachInterrupt(digitalPinToInterrupt(WEIGH_SCALE_1));
  detachInterrupt(digitalPinToInterrupt(WEIGH_SCALE_2));
  request_event(3);
  attachInterrupt(WEIGH_SCALE_1, one, RISING);
  attachInterrupt(WEIGH_SCALE_2, two, RISING);
}


void Scale::request_event(uint8_t devId){
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
}


void Scale::extract_parameters(uint16_t* weights){
  uint16_t oneQuarter = weights[uint16_t((RESPONSE_SIZE / 2) * 0.25)];
  uint16_t halve = weights[uint16_t((RESPONSE_SIZE / 2) * 0.50)];
  uint16_t threeQuarters = weights[uint16_t((RESPONSE_SIZE / 2) * 0.75)];

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
  for(uint16_t i = 0; i < (RESPONSE_SIZE / 2); i++){
    sum += weights[i];
  }

  uint16_t average = (sum / (RESPONSE_SIZE / 2));
  parameters[3] = average;
  byteParameters[6] = average >> 8;
  byteParameters[7] = average;
}


void Scale::extract_parameters(float* fweights){
  uint16_t oneQuarter = (weights[uint16_t((RESPONSE_SIZE / 2) * 0.25)]);
  uint16_t halve = (weights[uint16_t((RESPONSE_SIZE / 2) * 0.50)]);
  uint16_t threeQuarters = (weights[uint16_t((RESPONSE_SIZE / 2) * 0.75)]);

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

  uint16_t average = ((sum / (RESPONSE_SIZE / 2)) * 100);
  floatParameters[3] = average;
  byteParameters[6] = average >> 8;
  byteParameters[7] = average;
}
