
// User Imports

#include "coms.h"
//#include "memorycard.h"
//#include "LowPower.h"

//Scale scale; // Construct scale class, needed for HX711 library

void setup(){
  Serial.begin(57600);
  Communications::init();
//  MemoryCard::init();
}

void loop(){
  while(!Communications::state){
    os_runloop_once();
    delay(1000);
    Communications::state = true;
  }
  Communications::request_send("Hello");
//  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
//  MemoryCard::read_data("test.txt");
}
