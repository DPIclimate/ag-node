
// User Imports

#include "coms.h"
#include "scale.h"
#include "memorycard.h"
#include <Snooze.h>
//
//// Load drivers
SnoozeTouch touch;
SnoozeDigital digital;
SnoozeTimer timer;
//
//// install drivers to a SnoozeBlock
SnoozeBlock config(timer, digital);

Scale scale; // Construct scale class, needed for HX711 library

void setup(){
  Serial.begin(9600);
  Communications::init();
  MemoryCard::init();
  scale.init();
  timer.setTimer(10000);// milliseconds
}


void loop(){
  while(!Communications::check_state()){
    os_runloop_once();
  }
  int who = Snooze.deepSleep( config );// return module that woke processor
  scale.scan();
  scale.compile(scale.get_weights(), scale.get_time_stamps(), scale.get_capture_time());
  Communications::request_send(scale.get_payload());
  MemoryCard::write_data("scale.txt", scale.get_weights(), scale.get_time_stamps(), 
  scale.get_size(), scale.get_capture_time());
}
