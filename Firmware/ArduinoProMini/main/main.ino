
// User Imports

#include "coms.h"

Scale scale; // Construct scale class, needed for HX711 library

void setup(){
  Serial.begin(9600);
  Communications::init();
}

void loop(){
  while(!Communications::check_state()){
    os_runloop_once();
    Serial.println("Working");
    delay(1000);
  }
}
