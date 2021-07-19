
// User Imports
#include "scale.h"
#include <Wire.h>


void setup(){
  Scale::init();
  Wire.begin(8); // Specific to each device (1 = WS1, 2 = WS2, 3 = WS3)
  Wire.onRequest(requestEvent);
  Serial.begin(57600);
  pinMode(13, OUTPUT);
}


void requestEvent(){
  digitalWrite(13, HIGH);
  byte arr[5];
  arr[0] = 1;
  arr[1] = 2;
  arr[2] = 3;
  arr[3] = 4;
  arr[4] = 5;
  Wire.write(arr, 5);
  digitalWrite(13, LOW);
}


void loop(){
  delay(100);
}
