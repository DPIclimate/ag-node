
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

  digital.pinMode(WEIGH_SCALE_1, INPUT_PULLUP, RISING);
  digital.pinMode(WEIGH_SCALE_2, INPUT_PULLUP, RISING);
  digital.pinMode(WEIGH_SCALE_3, INPUT_PULLUP, RISING);
}


void loop(){
  while(!Lora::check_state()){
    os_runloop_once();
  }
  int who = Snooze.deepSleep(config);
  switch(who) {
    case WEIGH_SCALE_1:
      Scale::one();
      break;
    case WEIGH_SCALE_2:
      Scale::two();
      break;
    case WEIGH_SCALE_3:
      Scale::three();
      break;
  }
}
