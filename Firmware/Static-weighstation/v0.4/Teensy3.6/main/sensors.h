#ifndef SENSORS
#define SENSORS

// External libraries
#include <Wire.h>
#include <HX711.h>
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
  static int8_t* construct_payload(uint8_t scaleID);
};

class WeighStation {
  // Scale states
  bool oneActive = false, twoActive = false, threeActive = false;
  uint16_t onePos = 0, twoPos = 0, threePos = 0; // Position in capture array

  // Number of readings to average (more is slower)
  const unsigned int SCALE_AVERAGES = 1;

  // Reset weight to zero if this threshold is crossed (prevents drifiting)
  const float ZERO_THRESHOLD = 0.5;

  // Minimum weight threshold
  const float triggerWeight = 10; // kg

  // Stop reading weight threshold
  const float stopWeight = 2; // kg

  // Timer for calculating time on scale
  unsigned long timer;

  public: 

  // Create scale objects
  static const uint8_t nScales = 3;

  // Arrays for holding weights and corresponding time
  static const int16_t maxArrSize = 2000;
  
  // Setup weighscales and zero their offsets (tare)
  void init();

  // Zero weighscales when over a certain threshold (ZERO_THRESHOLD)
  void calibrate(HX711 scale);
  
  // Scan each of the scales and capture any animal weights
  void scan();
  
};


class RealTimeClock{
  static const uint32_t unixTime = 1630466457; // Current UNIX time
  
  public:
  // Initalise RTC
  static void init();

  // Set RTC
  static void set_time();
    
};


#endif
