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

/* 
Struct that holds all desired variables from weight reading
*/
struct parameters_t {
  uint32_t unixTime; ///< Unix time the recording finished
  uint8_t scaleID; ///< The scale that was used
  int16_t startWeight; ///< The weight at 25% of timeOnScale
  int16_t middleWeight; ///< The weight at 50% of timeOnScale
  int16_t endWeight; ///< The weight at 75% of timeOnScale
  int16_t avWeight; ///< The average weight between 25% and 75% of timeOnScale
  int16_t stdevWeight; ///< The SD weight between 25% and 75% of timeOnScale
  int16_t deltaWeight; ///< Difference between endWeight and startWeight
  int16_t estimatedWeight; ///< Best estimate weight
  int16_t maxWeight; ///< Maxiumum reading (regardless of position)
  uint8_t estWeightFlag; ///< Flag used to describe which parameter is used for estimatedWeight
  int32_t timeOnScale; ///< The time a sheep spent on the scale
};

struct fit_t {
  int16_t maxStableWeight; ///< Maximum stable weight in time-series readings
  int32_t timeStable; ///< Time that MSW was obtained
  int16_t rmse; ///< RMSE of maximum stable weight
};

class Sensors {
  public:
  static int8_t* construct_payload(); ///< Sensors payload 
};

class WeighStation {

  const unsigned int SCALE_AVERAGES = 1; ///< Number of readings to average (more is slower)

  const float ZERO_THRESHOLD = 0.5; ///< Reset weight to zero if this threshold is crossed (prevents drifiting)

  const float triggerWeight = 5; ///< Minimum weight threshold (kg)

  int8_t payloads[STORED_PAYLOAD_SIZE][WEIGH_PAYLOAD_SIZE]; ///< Holds packets until ready to send 

  public:
  
  static constexpr uint8_t nScales = 3; ///< Create scale objects

  static uint8_t payloadPos; ///< Current position in payloads array

  static constexpr int16_t maxArrSize = 6000; ///< Arrays for holding weights and corresponding time

  void init(); ///< Setup weighscales and zero their offsets (tare)

  static void tare_scales(); ///< Zero weighscales 

  void sleep(); ///< Put the scales in low-power mode

  void wakeup(); ///< Wake up the scales from low-power mode
  
  void scan(); ///< Scan each of the scales and capture any animal weights

  static int8_t* construct_payload(uint8_t scaleID); ///< Construct payload to send over LoRaWAN

  void append_payload(int8_t* payload); ///< Place payload in storage to send over LoRa

  void forward_payload(); ///< Send waiting payloads over LoRaWAN

  bool check_state(); ///< Check if scale is currently reading

  static inline float findSlope(float x1, float x2, float y1, float y2) { return (y2 - y1) / (x2 - x1); };

  template <size_t SIZE>
  static float root_mean_sqare_error(int32_t (&ts)[SIZE], int16_t (&weights)[SIZE], float slope, float yIntercept);

  template <size_t SIZE>
  static fit_t find_plateau(int32_t (&ts)[SIZE], int16_t (&weights)[SIZE], int16_t inputLength);

};

class Monitoring {
  public:
    // Power monitoring addresses
    static const uint8_t batteryAddr = 0x40;
    static const uint8_t solarAddr = 0x44;

	// Monitoring connection states
    static bool batteryConnected;
    static bool solarConnected;
    
    static void init(); ///< Initialise monitoring if available
    
    static int16_t voltage(char type); ///< Get voltage
    
    static int16_t current(char type); ///< Get current
    
    static int16_t power(char type); ///< Get power
};

class Temperature {
  static int8_t const TEMPERATURE_RESOLUTION = 9; ///< bits
  public:
    static void init(DallasTemperature sensor); ///< Initialsise temperature recording
    static int16_t measure(); ///< Measure temperatrue 
};


class RealTimeClock {
  
  public:
  static void init(); ///< Initalise RTC

  static void set_time(); ///< Set RTC
};

#endif
