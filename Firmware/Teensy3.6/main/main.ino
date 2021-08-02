
// User Imports

#include "lora.h"
#include "scale.h"
#include "memory.h"
#include <Snooze.h>

SnoozeDigital digital;
SnoozeBlock config(digital);

void setup(){
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  Lora::init();
  while(!Lora::check_state()){
    os_runloop_once();
  }
  Scale::init();
  Memory::init();
  delay(100);
}


void loop(){
  while(!Lora::check_state()){
    os_runloop_once();
  }
//  int who = Snooze.sleep(config);
}
