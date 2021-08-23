
// User
//#include "lora.h"
#include "scale.h"
#include "memory.h"

// External
#include <Snooze.h>

#ifndef DEBUG
  SnoozeDigital digital;
  SnoozeBlock config(digital);
#endif

WeighStation weighStation;

void setup(){
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  
//  Lora::init();
//  while(!Lora::check_state()){
//    os_runloop_once();
//  }
  
  weighStation.init();
  Memory::init();
}


void loop(){
  delay(100);
}
