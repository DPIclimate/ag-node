/*
 * HX711 extended functions for detemining the weight of livestock. 
 */
 
#ifndef SCALE
#define SCALE

#include "Arduino.h"
#include "HX711.h"
#include "config.h"


class Scale {
  #if DEVICE_ID == 1
    inline static int16_t CALIBRATION_FACTOR = -1870;
  #elif DEVICE_ID == 20
    inline static int16_t CALIBRATION_FACTOR = -1840;
  #else
    inline static int16_t CALIBRATION_FACTOR = -1800;
  #endif
    
	inline static unsigned int DOUT = 2; // SDA
	inline static unsigned int CLK = 3; // SCL

  inline static const uint8_t POS_ANGLE = 4; // Default 4 - positive angle of slope (i.e. when animal steps on scale)
  inline static const int8_t NEG_ANGLE = -4; // Default -4 - negitive angle of slope (i.e. when animal steps off scale)

  inline static const float ZERO_THRESHOLD = 0.5; // Reset to zero when weight is over or under this

  inline static const unsigned int X_RESOLUTION = 4; // Number of readings between slope calculations

  inline static const unsigned int SCALE_AVERAGES = 1; // Number of readings to create an average 

public: 
  inline static int16_t timer; // Timer for timeStamps
  inline static const unsigned int WEIGHT_ARRAY_SIZE = 200; // Sufficently sized array?
  inline static int16_t weights[WEIGHT_ARRAY_SIZE]; // Array containing weight profile
  inline static int16_t timeStamps[WEIGHT_ARRAY_SIZE]; // Timestamps for weight profile
  
  static void init();
  static float read_weight(byte times = 1);
  static void scan();
  static void calibrate();
  static float calculate_slope(float x1, float x2, float y1, float y2);
  static void capture();
};

#endif
