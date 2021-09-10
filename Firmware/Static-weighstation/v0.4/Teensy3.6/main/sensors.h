#ifndef SENSORS
#define SENSORS

// External libraries
#include <Wire.h>
// Weigh scales
#include <HX711.h>
// Time keeping
#include <DS1307RTC.h>
#include <TimeLib.h>
// Power monitoring
#include <Adafruit_INA219.h>
#include "Arduino.h"
// Temperature
#include <DallasTemperature.h>

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

#define ONE_WIRE_BUS 38
#define TEMPERATURE_RESOLUTION 12

class Sensors {
  public:
  static int8_t* construct_payload();

};

class WeighStation {
  // Scale states
  bool oneActive = false, twoActive = false, threeActive = false;
  uint32_t oneStartTime = 0, twoStartTime = 0, threeStartTime = 0;
  uint16_t onePos = 0, twoPos = 0, threePos = 0; // Position in capture array

  // Number of readings to average (more is slower)
  const unsigned int SCALE_AVERAGES = 1;

  // Reset weight to zero if this threshold is crossed (prevents drifiting)
  const float ZERO_THRESHOLD = 0.5;

  // Minimum weight threshold
  const float triggerWeight = 10; // kg

  // Stop reading weight threshold
  const float stopWeight = 2; // kg

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

  // Construct payload to send over LoRaWAN
  static int8_t* construct_payload(uint8_t scaleID);

};

class Monitoring {
  public:
    // Power monitoring addresses
    static const uint8_t batteryAddr = 0x40;
    static const uint8_t solarAddr = 0x44;
    
    static void init();
    
    // Get voltage
    static int16_t voltage(Adafruit_INA219 ina219); 
    
    // Get current
    static int16_t current(Adafruit_INA219 ina219);
    
    // Get power
    static int16_t power(Adafruit_INA219 ina219);
};

class Temperature : public DallasTemperature{
  public:
    static DallasTemperature sensor;
    static void init(OneWire* oneWire);
    static int16_t measure(DallasTemperature sensor);
};


class RealTimeClock{
  static const uint32_t unixTime = 1631253902; // Current UNIX time
  
  public:
  // Initalise RTC
  static void init();

  // Set RTC
  static void set_time();
    
};


#endif
