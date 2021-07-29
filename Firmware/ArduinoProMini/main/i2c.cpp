#include "i2c.h"

void ISquaredC::init(){
  Wire.begin(DEVICE_ID);
  Wire.onRequest(ISquaredC::respond);
  pinMode(WAKE_UP, OUTPUT);
}


void ISquaredC::request_transmission(){
  // Request teensy to accept read weights and timestamps
  weightsPosition = 0; // Reset to prevent overflow
  timeStampsPosition = 0; // Reset to prevent overflow
  digitalWrite(WAKE_UP, HIGH);
}


void ISquaredC::respond(){
  digitalWrite(WAKE_UP, LOW); // Reset request
  // Response to request with weight data
  const int8_t packetSize = 20;
  uint8_t packet[packetSize];
  
  uint16_t shift = 0;
  for(uint16_t i = shift; i < (packetSize / 2) ; i++){
    shift = i + i;
    if(weightsPosition < (RESPONSE_SIZE / 2)){
      uint16_t weight = Scale::weights[weightsPosition];
      packet[shift] = weight >> 8;
      packet[shift+1] = weight;
      weightsPosition++;
    } else{
      uint16_t timeStamp = Scale::timeStamps[timeStampsPosition];
      packet[shift] = timeStamp >> 8;
      packet[shift+1] = timeStamp;
      timeStampsPosition++;
    }
    
    
  }
  Wire.write(packet, packetSize);
}
