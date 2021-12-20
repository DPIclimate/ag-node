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
struct parameters_t {
  uint32_t unixTime;
  uint8_t scaleID;
  int16_t startWeight;
  int16_t middleWeight;
  int16_t endWeight;
  int16_t avWeight;
  int16_t stdevWeight;
  int16_t deltaWeight;
  int32_t timeOnScale;
};

class Sensors {
  public:
  // Sensors payload (Temperature, monitoring and additional sensors...)
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
  const float triggerWeight = 5; // kg

  // Holds packets until ready to send 
  int8_t payloads[50][WEIGH_PAYLOAD_SIZE];

  public:
  
  // Create scale objects
  static const uint8_t nScales = 3;

  // Arrays for holding weights and corresponding time
  static const int16_t maxArrSize = 20;

  // Current position in payloads array
  static uint8_t payloadPos;
  
  // Setup weighscales and zero their offsets (tare)
  void init();

  // Zero weighscales when over a certain threshold (ZERO_THRESHOLD)
  static void tare_scales();

  // Put the scales in low-power mode
  void sleep();

  // Wake up the scales from low-power mode
  void wakeup();
  
  // Scan each of the scales and capture any animal weights
  void scan();

  // Construct payload to send over LoRaWAN
  static int8_t* construct_payload(uint8_t scaleID);

  // Place payload in storage to send over LoRa
  void append_payload(int8_t* payload);

  // Send waiting payloads over LoRaWAN
  void forward_payload();

  // Check if scale is currently reading
  bool check_state();

};

class Monitoring {
  public:
    // Power monitoring addresses
    static const uint8_t batteryAddr = 0x40;
    static const uint8_t solarAddr = 0x44;

    static bool batteryConnected;
    static bool solarConnected;
    
    static void init();
    
    // Get voltage
    static int16_t voltage(char type); 
    
    // Get current
    static int16_t current(char type);
    
    // Get power
    static int16_t power(char type);
};

class Temperature {
  static int8_t const TEMPERATURE_RESOLUTION = 9; // bits
  public:
    static void init(DallasTemperature sensor);
    static int16_t measure();
};


class RealTimeClock {
  
  public:
  // Initalise RTC
  static void init();

  // Set RTC
  static void set_time();
    
};

#endif
