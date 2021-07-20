
// User Imports
#include "scale.h"
#include <Wire.h>

#define wakeUp 11

void setup(){
  Scale::init();
  Wire.begin(1); // Specific to each device (1 = WS1, 2 = WS2, 3 = WS3)
  Wire.onRequest(requestEvent);
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  pinMode(wakeUp, OUTPUT);
}


void requestEvent(){
  // Response to request with weight data
  byte response[9];

  response[0] = Scale::startWeight >> 8;
  response[1] = Scale::startWeight;

  response[2] = Scale::midWeight >> 8;
  response[3] = Scale::midWeight;

  response[4] = Scale::endWeight >> 8;
  response[5] = Scale::endWeight;

  response[6] = Scale::avWeight >> 8;
  response[7] = Scale::avWeight;
  
  response[8] = Scale::nReadings;
  
  Wire.write(response, sizeof(response));

  digitalWrite(wakeUp, LOW);
}

void requestSend(){
  digitalWrite(wakeUp, HIGH);
}


void loop(){
  delay(500);
  Scale::scan();
  Scale::compile();
  requestSend();
}
