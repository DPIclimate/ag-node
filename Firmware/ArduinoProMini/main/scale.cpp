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

const unsigned int WEIGHT_ARRAY_SIZE = 1000; // Sufficently sized array?
float weights[WEIGHT_ARRAY_SIZE]; // Array containing sheep weight profile
float timeStamps[WEIGHT_ARRAY_SIZE]; // Timestamps for sheep weight profile
unsigned long timer; // Timer for timeStamps
uint8_t* payload; // Payload to be send over lorawan

HX711 hx711;

void Scale::init(){
  // Initalise and zero scale
  hx711.begin(DOUT, CLK);

  if (hx711.is_ready()){
    Serial.println("Scale found and initalised");
  } else {
    Serial.println("Unable to find HX711");
  }
  
  hx711.set_scale(CALIBRATION_FACTOR);
  hx711.tare(); // Reset hx711 to zero
  timer = millis(); // Start timer
}


void Scale::calibrate(){
  // Reset scale to zero (preventing drifting)
  if(set_weight(SCALE_AVERAGES) > ZERO_THRESHOLD || set_weight(SCALE_AVERAGES) < -ZERO_THRESHOLD){
    hx711.tare(); // Reset to zero
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
}


float Scale::calculate_slope(float x1, float x2, float y1, float y2){
  // Calculate slope of recent capture
  float slope = ((float)x1 - (float)x2) / ((float)y1 - (float)y2);

  Serial.print("[SCALE]: Slope = ");
  if(isnan(slope)){
    Serial.println("0.00");
    return 0.00;
  }
  else{
    Serial.println(slope);
    return slope;
  }
}


void Scale::compile(float* weights, float* timeStamps){
  // Take array of floats and calc values for payload
  // Sore weights array in SD card with timestamp and ID
  // Return array of 8 byte ints for coms

  byte pl[PAYLOAD_SIZE];

  // ----- Convert weight transient to byte array ----- //

  byte plWeights[WEIGHT_ARRAY_SIZE * 2];
  byte plTimes[WEIGHT_ARRAY_SIZE * 2];
  int offset = 0;
  for(unsigned int i = 0; i < WEIGHT_ARRAY_SIZE; i++){
    offset = i + i;
    int16_t value = weights[offset] * 100;
    int16_t timestamp = timeStamps[offset] * 100;

    plWeights[offset] = value >> 8;
    plWeights[offset + 1] = value;

    plTimes[offset] = timestamp >> 8;
    plTimes[offset + 1] = timestamp;
  }

  // ----- Calcualte parameters and convert to byte array ----- //

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

  pl[0] = startWeight >> 8;
  pl[1] = startWeight;

  pl[2] = midWeight >> 8;
  pl[3] = midWeight;

  pl[4] = endWeight >> 8;
  pl[5] = endWeight;

  pl[6] = changeInWeight >> 8;
  pl[7] = changeInWeight;

  payload = pl;
}

byte* Scale::get_payload(){
  return payload;
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
  weight = hx711.get_units(times);
//  Serial.println(weight);
	return weight;
}
