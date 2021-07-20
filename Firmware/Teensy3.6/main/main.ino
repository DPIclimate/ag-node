
// User Imports

#include "coms.h"
#include "memorycard.h"
//#include <Snooze.h>
#include <Wire.h>

// Interrupts 
#define ws1 21
#define ws2 20
#define ws3 17

//// Load drivers
//SnoozeTouch touch;
//SnoozeDigital digital;
//SnoozeTimer timer;
//
//SnoozeBlock config(timer, digital);

void setup(){
  Serial.begin(57600);
  Wire.begin();
  pinMode(13, OUTPUT);
  Communications::init();
  while(!Communications::check_state()){
    os_runloop_once();
  }
  MemoryCard::init();
  attachInterrupt(ws1, weigh_scale_1, RISING);
  attachInterrupt(ws2, weigh_scale_2, RISING);
  attachInterrupt(ws3, weigh_scale_3, RISING);
}


void request_event(uint8_t devId, uint8_t nResults = 9){
  Wire.requestFrom(devId, nResults);
  byte response[9];
  int index = 0;
  while(Wire.available()) { // slave may send less than requested
    response[index] = Wire.read();   // receive a byte as character
    index++;
  }

  float startWeight = ((response[0] << 8) | response[1]) / 100.0f;
  Serial.print("Start Weight: ");
  Serial.println(startWeight);

  float middleWeight = ((response[2] << 8) | response[3]) / 100.0f;
  Serial.print("Middle Weight: ");
  Serial.println(middleWeight);

  float endWeight = ((response[4] << 8) | response[5]) / 100.0f;
  Serial.print("End Weight: ");
  Serial.println(endWeight);

  float avWeight = ((response[6] << 8) | response[7]) / 100.0f;
  Serial.print("Average Weight: ");
  Serial.println(avWeight);

  int nReadings = response[8];
  Serial.print("Number of Readings: ");
  Serial.println(nReadings);

  Communications::request_send(response);

  MemoryCard::write_parameters("weighdata.txt", response);
}


void weigh_scale_1(){
  request_event(1);
}

void weigh_scale_2(){
  request_event(2);
}

void weigh_scale_3(){
  request_event(3);
}


void loop(){
  while(!Communications::check_state()){
    os_runloop_once();
  }
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);
  delay(2000);
}
