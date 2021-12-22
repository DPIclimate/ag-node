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
  #ifdef DEBUG
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


void Scale::scan(){
  // Scan weights to detect when an animal steps on the scale
  // Reset arrays to zeros
  memset(Scale::weights, 0, sizeof(Scale::weights));
  memset(Scale::timeStamps, 0, sizeof(Scale::timeStamps));

  hx711.tare();

  // Read weights until animal is detected
  float weight = 0.0f;
  uint8_t resetWeight = 0; // Trigger tare
  while(read_weight(SCALE_AVERAGES) < TRIGGER_WEIGHT){
    weight = read_weight(SCALE_AVERAGES);
    #ifdef DEBUG
      Serial.print("Weight: ");
      Serial.println(weight);
    #endif
    Scale::timer = millis(); // Reset timer to zero
    int16_t startTime = Scale::timer;
    if(resetWeight == 100){
      #ifdef DEBUG
        Serial.println("Tare!");
      #endif
      hx711.tare();
      resetWeight = 0;
    } resetWeight++;
  }
  capture(); // Animal is on scale begin capture
}


void Scale::capture(){
  // Capture the animals weight and corresponding timestamps
  digitalWrite(13, HIGH);
  int16_t startTime = Scale::timer; // Capture start time
  #ifdef DEBUG
    Serial.println("==== Capture ====");
  #endif
  int8_t offWeights = 0; // Keep track of time off scale
  for(unsigned int i = 0; i < WEIGHT_ARRAY_SIZE; i++){
    // Check if animal has stepped off the scale
    float weight = read_weight(SCALE_AVERAGES);
    #ifdef DEBUG
      Serial.print("Weight: ");
      Serial.println(weight);
    #endif

    // Animal is off scale, take a few more readings just to be sure
    if(weight < TRIGGER_WEIGHT) offWeights++;
    else offWeights = 0;

    // Animal is definitely off scale, stop reading
    if(offWeights >= 10) break;

    // Animal is still on the scale, keep reading
    else{
      Scale::weights[i] = weight * 100.0;
      Scale::timeStamps[i] = millis() - startTime;
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
