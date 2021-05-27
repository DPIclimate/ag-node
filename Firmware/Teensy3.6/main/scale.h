/*
 * HX711 extended functions for detemining the weight of livestock. 
 */
 
#ifndef SCALE
#define SCALE

#include "Arduino.h"
#include "HX711.h"


class Scale : public HX711 {
  int CALIBRATION_FACTOR = -1760;
	unsigned int DOUT = 18; // SDA
	unsigned int CLK = 19; // SCL
  float weight = 0.00;

  const float POS_ANGLE = 4.0; // Positive angle of slope (i.e. when animal steps on scale)
  const float NEG_ANGLE = -4.0; // Negitive angle of slope (i.e. when animal steps off scale)

  const float ZERO_THRESHOLD = 0.5; // Reset to zero when weight is over or under this

  unsigned long timer; // Timer for timeStamps

  const unsigned int X_RESOLUTION = 4; // Number of readings between slope calculations

  const unsigned int SCALE_AVERAGES = 1; // Number of readings to create an average 

  uint8_t* pl; // Payload to be send over lorawan

  unsigned long captureTime = 0;
  
public: 
  void init();
	void set_cal_factor(const int cf);
  int get_cal_factor();
  float set_weight(byte times = 1);
	float get_weight();
  void scan();
  void calibrate();
  float calculate_slope(float x1, float x2, float y1, float y2);
  uint8_t* get_payload();
  unsigned long get_capture_time();
  void capture();
  float* get_weights();
  float* get_time_stamps();
  unsigned int get_size();
  void compile(float* weights, float* timeStamps, unsigned long captureTime);
};

#endif
