/*
 * HX711 extended functions for detemining the weight of livestock. 
 */
 
#ifndef SCALE
#define SCALE

#include "Arduino.h"
#include "HX711.h"


class Scale {
  inline static int CALIBRATION_FACTOR = -1760;
	inline static unsigned int DOUT = 2; // SDA
	inline static unsigned int CLK = 3; // SCL
  inline static float weight = 0.00;

  inline static const float POS_ANGLE = 4.0; // Positive angle of slope (i.e. when animal steps on scale)
  inline static const float NEG_ANGLE = -4.0; // Negitive angle of slope (i.e. when animal steps off scale)

  inline static const float ZERO_THRESHOLD = 0.5; // Reset to zero when weight is over or under this

  inline static const unsigned int X_RESOLUTION = 4; // Number of readings between slope calculations

  inline static const unsigned int SCALE_AVERAGES = 1; // Number of readings to create an average 

  inline static unsigned long captureTime = 0;
  inline static const unsigned int PAYLOAD_SIZE = 8;


  
public: 
  static void init();
	static void set_cal_factor(const int cf);
  static int get_cal_factor();
  static float set_weight(byte times = 1);
	static float get_weight();
  static void scan();
  static void calibrate();
  static float calculate_slope(float x1, float x2, float y1, float y2);
  static byte* get_payload();
  static int get_payload_size();
  static void capture();
  static float* get_weights();
  static float* get_time_stamps();
  static unsigned int get_size();
  static void compile(float* weights, float* timeStamps);
};

#endif
