/*
 * Functions for detemining the weight of livestock.
 * 
 * Initalise -> Calibrate -> Scan -> Capture -> Compile
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

HX711 hx711;

void Scale::init(){
  // Initalise and zero scale
  hx711.begin(DOUT, CLK);
  #if DEBUG == 1
    if (hx711.is_ready()){
      Serial.println("Scale found and initalised");
    } else {
      Serial.println("Unable to find HX711");
    }
  #endif
  hx711.set_scale(CALIBRATION_FACTOR);
  hx711.tare(); // Reset hx711 to zero
  Scale::timer = millis(); // Start timer
}


void Scale::calibrate(){
  // Reset scale to zero (preventing drifting)
  if(read_weight(SCALE_AVERAGES) > ZERO_THRESHOLD || read_weight(SCALE_AVERAGES) < -ZERO_THRESHOLD){
    hx711.tare(); // Reset to zero
  }
}


void Scale::scan(){
  // Scan weights to detect when an animal steps on the scale
  // Reset arrays to zeros
  memset(Scale::weights, 0, sizeof(Scale::weights));
  memset(Scale::timeStamps, 0, sizeof(Scale::timeStamps));
  
  Scale::calibrate(); // TODO: need to ensure this isn't interfering (its meant to prevent drifting)

  // Wait until slope is positive, indicating animal in on scale
  while(calculate_slope(Scale::weights[0], Scale::weights[X_RESOLUTION], Scale::timeStamps[0], Scale::timeStamps[X_RESOLUTION]) < POS_ANGLE){
    Scale::timer = millis(); // Reset timer to zero
    int16_t startTime = Scale::timer;
    // Get a few values to check if there is a change in slope
    for(uint16_t i = 0; i <= X_RESOLUTION; i++){
      #ifdef HIGH_PRECISION
        Scale::weights[i] = (int16_t)(read_weight(SCALE_AVERAGES) * 100); // Change with desired averages
      #else
        Scale::weights[i] = (int16_t)read_weight(SCALE_AVERAGES); // Change with desired averages
      #endif
      Scale::timeStamps[i] = millis() - startTime;

      #if DEBUG == 1
        Serial.print(Scale::timeStamps[i]);
        Serial.print("\t");
        #ifdef HIGH_PRECISION
          Serial.println((float)Scale::weights[i] / 100.0);
        #else
          Serial.println(Scale::weights[i]);
        #endif
      #endif
    }
  }
  capture(); // Animal is on scale begin capture
}


void Scale::capture(){
  // Capture the animals weight and corresponding timestamps
  digitalWrite(13, HIGH);
  int16_t startTime = Scale::timer; // Capture start time
  #if DEBUG == 1
    Serial.println("==== Capture ====");
  #endif
  for(unsigned int i = X_RESOLUTION; i < WEIGHT_ARRAY_SIZE; i++){
    // Check if animal has stepped off the scale
    if(calculate_slope(Scale::weights[i - X_RESOLUTION], Scale::weights[i - 1], Scale::timeStamps[i - X_RESOLUTION], Scale::timeStamps[i - 1]) < NEG_ANGLE
    && i >= 20){
      break;
    }
    // Animal is still on the scale, keep reading
    else{
      #ifdef HIGH_PRECISION
        Scale::weights[i] = (int16_t)(read_weight(SCALE_AVERAGES) * 100); // Change with desired averages
      #else
        Scale::weights[i] = (int16_t)read_weight(SCALE_AVERAGES); // Change with desired averages
      #endif
      Scale::timeStamps[i] = millis() - startTime;
      #if DEBUG == 1
        Serial.print(Scale::timeStamps[i]);
        Serial.print("\t");
        #ifdef HIGH_PRECISION
          Serial.println((float)Scale::weights[i] / 100.0);
        #else
          Serial.println(Scale::weights[i]);
        #endif
      #endif
      delay(100);
    }
  }
  digitalWrite(13, LOW);
}


float Scale::calculate_slope(float x1, float x2, float y1, float y2){
  // Finds out of an animal has steps on (or off) the weigh scale
  float slope = (x1 - x2) / (y1 - y2);
  if(isnan(slope)){
    return 0.00;
  }
  else{
    return slope;
  }
}


float Scale::read_weight(byte times){
  // Read and return weight
	return hx711.get_units(times);
}
