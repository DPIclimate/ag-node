#ifndef SENSORS
#define SENSORS

// External libraries
#include <Wire.h>
#include <DS1307RTC.h>
#include <TimeLib.h>
#include "Arduino.h"

// Internal libraries
#include "config.h"
#include "memory.h"
#ifdef ENABLE_LORAWAN
  #include "lora.h"
#endif

// Struct that holds all desired variables
struct parameters_t{
  uint32_t unixTime;
  uint8_t scaleID;
  int16_t startWeight;
  int16_t middleWeight;
  int16_t endWeight;
  int16_t avWeight;
  int16_t stdevWeight;
  int16_t deltaWeight;
  int16_t timeOnScale;
}; 

class Sensors {
  public:
  // Payload to be sent over LoRaWAN
  static int8_t payload[PAYLOAD_SIZE];
  
  // Construct payload to send over LoRaWAN
  static void construct_payload(uint8_t scaleID);
};


class WeighStation {
  public:
  // Arrays for holding weights and corresponding time
  static int16_t weights[(RESPONSE_SIZE / 2)];
  static int16_t timeStamps[(RESPONSE_SIZE / 2)]; 
  
  // Setup I2C communications to weigh scales (ATMega328's -> HX711)
  static void init();

  // Request weights from an individual weigh scale
  static void scale_one();
  static void scale_two();
  static void scale_three();

  // Reqeust weights and timestamps from a particualr weigh scale
  static void request_weights(uint8_t scaleID);
};


class RealTimeClock{
  uint32_t unixTime = 1630237934; // Current UNIX time
  
  public:
  // Initalise RTC
  static void init();

  // Set RTC
  static void set_time();
    
};


#endif
