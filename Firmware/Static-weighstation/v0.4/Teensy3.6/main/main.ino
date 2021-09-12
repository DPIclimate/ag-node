// Local imports
#ifdef ENABLE_LORAWAN
  #include "lora.h"
#endif
#include "sensors.h"
#include "memory.h"

// Temperature related setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

bool sensorPayload = true; // Has payload been sent over LoRa
uint8_t tareScales = 0; // Tare scales after 255 readings

// Time related variables for distributing LoRaWAN paylaods
uint32_t timer = 0;
uint32_t lastMessage = 0;
uint32_t messageSpacing = 1000 * 60; // ms

WeighStation weighStation; // Create weighstation object

void setup(){
  Serial.begin(57600);
  
  #ifdef DEBUG
    while(!Serial) delay(100);
  #endif

  pinMode(13, OUTPUT);
  
  #ifdef ENABLE_LORAWAN
    Lora::init();
    while(!Lora::check_state()){
      os_runloop_once();
    }
  #endif

  // Setup classes
  RealTimeClock::init();
  Temperature::init(sensors);
  Monitoring::init();
  weighStation.init();
  Memory::init();
}


void loop(){
  
  // Keep track of when the last message was sent
  timer = millis() - lastMessage;
  
  // Scan scales to see if animal is on scale
  weighStation.scan();

  // Reset scale to zero (every 255 readings) - providing there isn't an animal on scale
  if(tareScales == 255 && !weighStation.check_state()){
    WeighStation::tare_scales();
    tareScales++;
  } else tareScales++;

  // Send stored weigh payloads over LoRa
  if((!weighStation.check_state() && weighStation.payloadPos != 0) && timer >= messageSpacing){
    weighStation.forward_payload();
    // Reset timer - prevents slow LoRa transimission
    lastMessage = millis();
    // Reset scales timer (just incase there is now an animal on scale)
    tareScales = 0; 
  }

  // Reset sensor payload send state - prevents multiple packets
  if(timer >= messageSpacing && sensorPayload) sensorPayload = false;

  // Every 15 min send sensor payload (moitoring and temperature)
  if(minute() % 15 == 0 && !sensorPayload){
    int8_t* sensorsPayload = Sensors::construct_payload();
    Lora::request_send(sensorsPayload, 2);
    while(!Lora::check_state()){
      os_runloop_once();
    }
    sensorPayload = true;
    // Reset timer - preventing slow LoRa transmission
    lastMessage = millis();
    // Reset scales timer (just incase there is now an animal on scale)
    tareScales = 0; 
  }

}
