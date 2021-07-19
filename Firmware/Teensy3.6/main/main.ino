
// User Imports

#include "coms.h"
#include "memorycard.h"
//#include <Snooze.h>
#include <Wire.h>

//// Load drivers
//SnoozeTouch touch;
//SnoozeDigital digital;
//SnoozeTimer timer;
//
//SnoozeBlock config(timer, digital);

byte response[8];

void setup(){
  Serial.begin(57600);
  Wire.begin();
  pinMode(13, OUTPUT);
//  Communications::init();
//  MemoryCard::init();
//  timer.setTimer(10000); // milliseconds
}


void loop(){
  digitalWrite(13, HIGH);  // briefly flash the LED
  Wire.requestFrom(8, 5);   // request 6 bytes from slave device #8
  
  while(Wire.available()) { // slave may send less than requested
    byte c = Wire.read();   // receive a byte as character
    Serial.print(c);        // print the character
  }
  
  Serial.println();
  digitalWrite(13, LOW);
  delay(2000);
}
