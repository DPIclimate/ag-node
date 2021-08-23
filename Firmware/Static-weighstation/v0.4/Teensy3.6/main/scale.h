#ifndef SCALE
#define SCALE

#include <HX711.h>
#include "Arduino.h"
#include "config.h"
#include "memory.h"
#include "lora.h"



class WeighStation {

  // Create scale objects
  static const uint8_t nScales = 3;
  HX711 scaleOne;
  HX711 scaleTwo;
  HX711 scaleThree;
  HX711 scales[nScales] = {scaleOne, scaleTwo, scaleThree};

  // Setup pin mapping for data and clock lines
  const uint8_t scaleData[nScales] = {1, 2, 3};
  const uint8_t scaleClock[nScales] = {4, 5, 6};

  // Set calibration factor for scale (see calibration script)
  const int16_t calibrationFactors[nScales] = {-1870, -1840, -1800};

  // Number of readings to average (more is slower)
  const unsigned int SCALE_AVERAGES = 1;

  // Reset weight to zero if this threshold is crossed (prevents drifiting)
  const float ZERO_THRESHOLD = 0.5;

  // Minimum weight threshold
  const int minWeight = 10; // kg

  // Timer for calculating time on scale
  unsigned long timer;

  // Arrays for holding weights and corresponding time
  static const uint16_t maxArrSize = 2000;
  // Scale One
  int scaleOneWeights[maxArrSize];
  long scaleOneTimes[maxArrSize];
  // Scale Two
  int scaleTwoWeights[maxArrSize];
  long scaleTwoTimes[maxArrSize];
  // Scale Three
  int scaleThreeWeights[maxArrSize];
  long scaleThreeTimes[maxArrSize];
  
  
  public: 
  // Setup weighscales and zero their offsets (tare)
  void init();

  // Zero weighscales when over a certain threshold (ZERO_THRESHOLD)
  void calibrate(HX711 scale);
  
  // Scan each of the scales and capture any animal weights
  void scan();
};


#endif
