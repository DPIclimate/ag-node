/*
 * To be ran at 24 MHz
 */

// Local imports
#ifdef ENABLE_LORAWAN
  #include "lora.h"
#endif
#include "sensors.h"
#include "memory.h"

#ifdef LOW_POWER
  #include <Snooze.h>
  SnoozeUSBSerial usb;
  SnoozeAlarm alarm;
  SnoozeBlock sleepConfig(usb, alarm);
#endif

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

  pinMode(LED_BUILTIN, OUTPUT);
  
  #ifdef ENABLE_LORAWAN
    Lora::init();
    while(!Lora::check_state()){
      os_runloop_once();
    }
  #endif

  #ifdef LOW_POWER
    // Send a sensor payload every night at 30 minute intervals
    alarm.setRtcTimer(0, 30, 0);
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

  // ======== Weight Payload ========
  
  // Scan scales to see if animal is on scale
  weighStation.scan();

  // Reset scale to zero (every 255 readings) - providing there isn't an animal on scale
  if(tareScales == 255 && !weighStation.check_state()){
    WeighStation::tare_scales();
    tareScales++;
  } else tareScales++;

  // Send stored weigh payloads over LoRa
  if(!weighStation.check_state() && weighStation.payloadPos != 0 && timer >= messageSpacing){
    weighStation.forward_payload();
    // Reset timer - prevents slow LoRa transimission
    lastMessage = millis();
    // Reset scales timer (just incase there is now an animal on scale)
    tareScales = 0; 
  }

  // Small delay to prevent high current usage 
  if(!weighStation.check_state()){
    delay(200);
  }

  // ======== Sensor Payload ========

  // Reset sensor payload send state - prevents multiple packets
  if(timer >= messageSpacing && sensorPayload) sensorPayload = false;

  // Every 15 min send sensor payload (moitoring and temperature)
  if(minute() % 15 == 0 && !sensorPayload){
    int8_t* sensorsPayload = Sensors::construct_payload();
    #ifdef ENABLE_LORAWAN
      Lora::request_send(sensorsPayload, 2);

      while(!Lora::check_state()){
        os_runloop_once();
      }
    #endif
    sensorPayload = true;
    // Reset timer - preventing slow LoRa transmission
    lastMessage = millis();
    // Reset scales timer (just incase there is now an animal on scale)
    tareScales = 0; 
  }

  // ======== Sleep (Night-time) ========
  
  #ifdef LOW_POWER
    // Between 6 pm and 6 am
    while(hour() >= 18 || hour() < 6) {
      weighStation.sleep();
      int who;
      who = Snooze.sleep(sleepConfig);
      
      // Send a payload
      int8_t* sensorsPayload = Sensors::construct_payload();
      #ifdef ENABLE_LORAWAN
        Lora::request_send(sensorsPayload, 2); 
        while(!Lora::check_state()){
          os_runloop_once();
        }
      #endif
      
      weighStation.wakeup();
    }
  #endif
  
}
