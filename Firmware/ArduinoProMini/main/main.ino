
// User Imports

#include "coms.h"
#include "memorycard.h"

void setup(){
  Serial.begin(57600);
  Communications::init();
  MemoryCard::init();
}

void loop(){
  while(!Communications::state){
    Serial.println("Request");
    os_runloop_once();
    delay(1000);
  }
  Serial.println("R: file");
  MemoryCard::read_data("test.txt");
  delay(1000);
}
