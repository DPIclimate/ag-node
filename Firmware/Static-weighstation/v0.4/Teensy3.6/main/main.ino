
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

WeighStation weighStation;

// Temperature 
OneWire oneWire(ONE_WIRE_BUS);
//Temperature temperature(&oneWire);

void setup(){
  Serial.begin(57600);
  
  #ifdef DEBUG
    while(!Serial);
  #endif

  RealTimeClock::init();
  
  pinMode(13, OUTPUT);
  
  #ifdef ENABLE_LORAWAN
    Lora::init();
    while(!Lora::check_state()){
      os_runloop_once();
    }
  #endif

  Temperature::init(&oneWire);
  Monitoring::init();
  weighStation.init();
//  Memory::init();
}


void loop(){
  weighStation.scan();
  delay(100);
}
