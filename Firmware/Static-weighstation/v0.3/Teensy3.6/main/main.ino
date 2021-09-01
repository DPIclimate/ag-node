
// Local imports
#ifdef ENABLE_LORAWAN
  #include "lora.h"
#endif
#include "sensors.h"
#include "memory.h"

// Enables sleep (disables serial communications)
#if !defined(DEBUG) && defined(LOW_POWER)
  #include <Snooze.h>
  SnoozeDigital digital;
  SnoozeBlock config(digital);
#endif

void setup(){
  Serial.begin(57600);
  #ifdef DEBUG
    while(!Serial);
  #endif

  RealTimeClock::init();
  RealTimeClock::set_time();

  pinMode(13, OUTPUT);

  #ifdef ENABLE_LORAWAN
    Lora::init();
    while(!Lora::check_state()){
      os_runloop_once();
    }
  #endif
  
  WeighStation::init();
  Memory::init();

  #if !defined(DEBUG) && defined(LOW_POWER)
    digital.pinMode(WEIGH_SCALE_1, INPUT_PULLDOWN, RISING);
    digital.pinMode(WEIGH_SCALE_2, INPUT_PULLDOWN, RISING);
    digital.pinMode(WEIGH_SCALE_3, INPUT_PULLDOWN, RISING);
  #else
    pinMode(WEIGH_SCALE_1, INPUT_PULLDOWN);
    pinMode(WEIGH_SCALE_2, INPUT_PULLDOWN);
    pinMode(WEIGH_SCALE_3, INPUT_PULLDOWN);
    attachInterrupt(WEIGH_SCALE_1, WeighStation::scale_one, RISING);
    attachInterrupt(WEIGH_SCALE_2, WeighStation::scale_two, RISING);
    attachInterrupt(WEIGH_SCALE_3, WeighStation::scale_three, RISING);
  #endif
}


void loop(){
  #if !defined(DEBUG) && defined(LOW_POWER)
    int who = Snooze.deepSleep(config);
    switch(who) {
      case WEIGH_SCALE_1:
        WeighStation::scale_one();
        break;
      case WEIGH_SCALE_2:
        WeighStation::scale_two();
        break;
      case WEIGH_SCALE_3:
        WeighStation::scale_three();
        break;
    }
  #endif
  delay(100);
}
