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

unsigned long timer; // Timer for timeStamps

HX711 hx711;

void Scale::init(){
  // Initalise and zero scale
  hx711.begin(DOUT, CLK);
//  if (hx711.is_ready()){
//    Serial.println("Scale found and initalised");
//  } else {
//    Serial.println("Unable to find HX711");
//  }
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
    Scale::weights[i] = 0.00;
    Scale::timeStamps[i] = 0.00;
  }
  
  calibrate(); // TODO: need to ensure this isn't interfering (its ment to prevent drifting)

  // Wait until slope is pos, indicating animal in on scale
  while(calculate_slope(Scale::weights[0], Scale::weights[X_RESOLUTION], Scale::timeStamps[0], Scale::timeStamps[X_RESOLUTION]) < POS_ANGLE){
    unsigned long startTime = timer;
    // Get a few values to check if there is a change in slope
    for(unsigned int i = 0; i <= X_RESOLUTION; i++){
      Scale::weights[i] = set_weight(SCALE_AVERAGES); // Change with desired averages
      Scale::timeStamps[i] = (millis() - startTime) / 100.0;
    }
  }
  capture(); // Animal is on scale begin capture
}


void Scale::capture(){
  // Capture the animals weight as an array of values
  // Also capture corresponding timestamps
  digitalWrite(13, HIGH);
  unsigned long startTime = timer; // Capture start time
  
  for(unsigned int i = X_RESOLUTION; i < WEIGHT_ARRAY_SIZE; i++){
    // Check if animal has stepped off the scale
    if(calculate_slope(Scale::weights[i - X_RESOLUTION], Scale::weights[i - 1], Scale::timeStamps[i - X_RESOLUTION], Scale::timeStamps[i - 1]) < NEG_ANGLE){
      break;
    }
    // Animal is still on the scale, keep reading
    else{
      Scale::weights[i] = set_weight(SCALE_AVERAGES); // Change with desired averages
      Scale::timeStamps[i] = (millis() - startTime) / 100.0; // TODO: Convert to milliseconds
      delay(100);
    }
  }
  digitalWrite(13, LOW);
}


float Scale::calculate_slope(float x1, float x2, float y1, float y2){
  // Calculate slope of recent capture
  float slope = ((float)x1 - (float)x2) / ((float)y1 - (float)y2);
  if(isnan(slope)){
    return 0.00;
  }
  else{
    return slope;
  }
}


void Scale::compile(){
  // Take array of floats and calc values for payload

  digitalWrite(13, HIGH);
  
  // ----- Convert weight transient to byte array ----- //

// Not used currently
//  byte plWeights[WEIGHT_ARRAY_SIZE * 2];
//  byte plTimes[WEIGHT_ARRAY_SIZE * 2];
//  int offset = 0;
//  for(unsigned int i = 0; i < WEIGHT_ARRAY_SIZE; i++){
//    offset = i + i;
//    int16_t value = weights[offset] * 100;
//    int16_t timestamp = timeStamps[offset] * 100;
//
//    plWeights[offset] = value >> 8;
//    plWeights[offset + 1] = value;
//
//    plTimes[offset] = timestamp >> 8;
//    plTimes[offset + 1] = timestamp;
//  }

  // ----- Calcualte parameters and convert to byte array ----- //

  // Put array in context (i.e. search for datapoints that are non-zero)
  // Need to do this to calc start and finishing weight
  unsigned int reading = 0;
  while(Scale::weights[reading] != 0){
    reading++;
    if(reading == Scale::WEIGHT_ARRAY_SIZE){
      break;
    }
  }

  // Get three positions in array
  unsigned int p1 = (float)reading * 0.25;
  unsigned int p2 = (float)reading * 0.50;
  unsigned int p3 = (float)reading * 0.75;

  // Get weights at three positions and convert to 16-bit int
  Scale::startWeight = (float)Scale::weights[p1] * 100;
  Scale::midWeight = (float)Scale::weights[p2] * 100;
  Scale::endWeight = (float)Scale::weights[p3] * 100;
  int sum = 0;
  Scale::nReadings = 0; // Number of values (indication of time on scale)
  for(int i = p1; i < p3; i++){
    sum += Scale::weights[i];
    Scale::nReadings++;
  }
  Scale::avWeight = (sum / Scale::nReadings) * 100;
  digitalWrite(13, LOW);

  Serial.print("Start: ");
  Serial.println(Scale::startWeight);
  Serial.print("Mid: ");
  Serial.println(Scale::midWeight);
  Serial.print("End: ");
  Serial.println(Scale::endWeight);
  Serial.print("Av: ");
  Serial.println(Scale::avWeight);
  Serial.print("N: ");
  Serial.println(Scale::nReadings);
  Serial.println();
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
	return weight;
}
