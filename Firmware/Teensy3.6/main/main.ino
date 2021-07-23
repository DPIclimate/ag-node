
// User Imports

#include "lora.h"
#include "scale.h"
#include "memory.h"


void setup(){
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  Lora::init();
  while(!Lora::check_state()){
    os_runloop_once();
  }
  Scale::init();
  Memory::init();
}


void loop(){
  while(!Lora::check_state()){
    os_runloop_once();
  }
  delay(1000);
}
