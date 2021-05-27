
// User Imports

#include "coms.h"
#include "scale.h"
#include "memorycard.h"

Scale scale; // Construct scale class, needed for HX711 library

void setup(){
  Serial.begin(9600);
  Communications::init();
  MemoryCard::init();
  scale.init();
}


void loop(){
  while(!Communications::check_state()){
    os_runloop_once();
  }
  scale.scan();
  scale.compile(scale.get_weights(), scale.get_time_stamps(), scale.get_capture_time());
  Communications::request_send(scale.get_payload());
  MemoryCard::write_data("scale.txt", scale.get_weights(), scale.get_time_stamps(), 
  scale.get_size(), scale.get_capture_time());
}
