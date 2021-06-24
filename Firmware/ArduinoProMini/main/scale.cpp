/*
 * HX711 extended functions for detemining the weight of livestock.
 * 
 * Initalise -> Calibrate -> Scan -> Capture -> Compile -> *send
 * 
 * *Handled by coms
 * 
 * Method:
 *  1. Initalise (configure HX711 library)
 *  2. Calibrate (reset scale to zero)
 *  3. Scan (read weight until positive slope is detected (indicating sheep is on scale))
 *    3b. Reset scale to zero to prevent drifting after each scan event 
 *  4. Capture (record weight into two arrays (weights, timeStamps)
 *  5. Compile (compile data into LoRaWAN payload to sending to gateway)
 */
 
#include "scale.h"

static const unsigned int WEIGHT_ARRAY_SIZE = 1000; // Sufficently sized array?
static float weights[WEIGHT_ARRAY_SIZE]; 
static float timeStamps[WEIGHT_ARRAY_SIZE];

void Scale::init(){
  // Initalise and zero scale
  begin(DOUT, CLK);
  set_scale(CALIBRATION_FACTOR);
  tare(); // Reset hx711 to zero
  timer = millis(); // Start timer
}


void Scale::calibrate(){
  // Reset scale to zero (preventing drifting)
  if(set_weight(SCALE_AVERAGES) > ZERO_THRESHOLD || set_weight(SCALE_AVERAGES) < -ZERO_THRESHOLD){
    tare(); // Reset to zero
  }
}


void Scale::scan(){
  // Scan weights until animal steps on scale

  // Reset arrays to zeros
  for(unsigned int i = 0; i < WEIGHT_ARRAY_SIZE; i++){
    weights[i] = 0.00;
    timeStamps[i] = 0.00;
  }
  
  calibrate(); // TODO: need to ensure this isn't interfering (its ment to prevent drifting)

  // Wait until slope is pos, indicating animal in on scale
  while(calculate_slope(weights[0], weights[X_RESOLUTION], timeStamps[0], timeStamps[X_RESOLUTION]) < POS_ANGLE){
    Serial.println("[SCALE]: Animal OFF");

    unsigned long startTime = timer;

    // Get a few values to check if there is a change in slope
    for(unsigned int i = 0; i <= X_RESOLUTION; i++){
      weights[i] = set_weight(SCALE_AVERAGES); // Change with desired averages
      timeStamps[i] = (millis() - startTime) / 100.0;
      Serial.println(weights[i]);
    }

    Serial.print("[SCALE]: Reading = ");
    Serial.println(weights[X_RESOLUTION]);
  }
  capture(); // Animal is on scale begin capture
}


void Scale::capture(){
  // Capture the animals weight as an array of values
  // Also capture corresponding timestamps

  Serial.println("[SCALE]: Animal ON (Capturing Data)");
  
  unsigned long startTime = timer; // Capture start time
  
  for(unsigned int i = X_RESOLUTION; i < WEIGHT_ARRAY_SIZE; i++){
    if(calculate_slope(weights[i - X_RESOLUTION], weights[i - 1], timeStamps[i - X_RESOLUTION], timeStamps[i - 1]) < NEG_ANGLE){
      break;
    }
    else{
      weights[i] = set_weight(SCALE_AVERAGES); // Change with desired averages
      timeStamps[i] = (millis() - startTime) / 100.0; // TODO: Convert to milliseconds
      Serial.print("[SCALE]: Weight = ");
      Serial.println(weights[i]);
      Serial.print("[SCALE]: Time = ");
      Serial.println(timeStamps[i]);
    }
  }
  captureTime = timeStamps[0];
}


float Scale::calculate_slope(float x1, float x2, float y1, float y2){
  // Calculate slope of recent capture
  float slope = ((float)x1 - (float)x2) / ((float)y1 - (float)y2);

  Serial.print("[SCALE]: Slope = ");
  if(isnan(slope)){
    Serial.println(slope);
    return 0.00;
  }
  else{
    Serial.println(slope);
    return slope;
  }
}


void Scale::compile(float* weights, float* timeStamps, unsigned long captureTime){
  // Take array of floats and calc values for payload
  // Sore weights array in SD card with timestamp and ID
  // Return array of 8 byte ints for coms
  const unsigned int PAYLOAD_SIZE = 12;
  static uint8_t payload[PAYLOAD_SIZE];

  // Put array in context (i.e. search for datapoints that are non-zero)
  // Need to do this to calc start and finishing weight
  unsigned int reading = 0;
  while(weights[reading] != 0){
    reading++;
    if(reading == WEIGHT_ARRAY_SIZE){
      break;
    }
  }


  // Get three positions in array
  unsigned int p1 = (float)reading * 0.25;
  unsigned int p2 = (float)reading * 0.50;
  unsigned int p3 = (float)reading * 0.75;
  
  int16_t startWeight = (float)weights[p1] * 100;
  int16_t midWeight = (float)weights[p2] * 100;
  int16_t endWeight = (float)weights[p3] * 100;
  int16_t changeInWeight = (float)(weights[p3] - weights[p1]) * 100;

  payload[0] = startWeight >> 8;
  payload[1] = startWeight;

  payload[2] = midWeight >> 8;
  payload[3] = midWeight;

  payload[4] = endWeight >> 8;
  payload[5] = endWeight;

  payload[6] = changeInWeight >> 8;
  payload[7] = changeInWeight;

  payload[8] = captureTime >> 24;
  payload[9] = captureTime >> 16; 
  payload[10] = captureTime >> 8;
  payload[11] = captureTime;

  pl = payload; // Send this to lorawan outgoing
}

uint8_t* Scale::get_payload(){
  return pl;
}


float* Scale::get_weights(){
  return weights;
}


float* Scale::get_time_stamps(){
  return timeStamps;
}

unsigned long Scale::get_capture_time(){
  return captureTime;
}


void Scale::set_cal_factor(const int cf){
  // Set calibration factor
	CALIBRATION_FACTOR = cf;
}


int Scale::get_cal_factor(){
  // Get the calibration factor
  return CALIBRATION_FACTOR;
}


float Scale::set_weight(byte times){
  // Read and return weight
  weight = get_units(times);
//  Serial.println(weight);
	return weight;
}

unsigned int Scale::get_size(){
  return WEIGHT_ARRAY_SIZE;
}


float Scale::get_weight(){
  // Returns last reading
  return weight;
}
