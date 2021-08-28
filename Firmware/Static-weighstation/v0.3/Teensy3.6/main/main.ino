
// User
#ifdef ENABLE_LORAWAN
  #include "lora.h"
#endif
#include "scale.h"
#include "memory.h"

// External
#include <Snooze.h>

#ifndef DEBUG
  SnoozeDigital digital;
  SnoozeBlock config(digital);
#endif

void setup(){
  Serial.begin(57600);
  pinMode(13, OUTPUT);

  #ifdef ENABLE_LORAWAN
    Lora::init();
    while(!Lora::check_state()){
      os_runloop_once();
    }
  #endif
  
  Scale::init();
  Memory::init();

  #ifdef DEBUG
    pinMode(WEIGH_SCALE_1, INPUT_PULLDOWN);
    pinMode(WEIGH_SCALE_2, INPUT_PULLDOWN);
    pinMode(WEIGH_SCALE_3, INPUT_PULLDOWN);
    attachInterrupt(WEIGH_SCALE_1, Scale::one, RISING);
    attachInterrupt(WEIGH_SCALE_2, Scale::two, RISING);
    attachInterrupt(WEIGH_SCALE_3, Scale::three, RISING);
  #else
    digital.pinMode(WEIGH_SCALE_1, INPUT_PULLDOWN, RISING);
    digital.pinMode(WEIGH_SCALE_2, INPUT_PULLDOWN, RISING);
    digital.pinMode(WEIGH_SCALE_3, INPUT_PULLDOWN, RISING);
  #endif
}


void loop(){
  #ifndef DEBUG
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
  #endif


  
  delay(100);
}
