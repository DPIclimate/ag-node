
// User Imports
#include "scale.h"
#include "i2c.h"


void setup(){
  Serial.begin(57600);
  Scale::init();
  ISquaredC::init();
}


void loop(){
  Scale::scan();
  ISquaredC::request_transmission();
  delay(3000);
}
