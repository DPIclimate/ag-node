#include "scale.h"


void WeighStation::init(){
  for(uint8_t i = 0; i < nScales; i++){
    scales[i].begin(scaleData[i], scaleClock[i]);
    #ifdef DEBUG
      Serial.print("Scale ");
      Serial.print(i + 1);
      Serial.print(": ");
      if (scales[i].is_ready()){
        Serial.println("found and initalised.");
      } else {
        Serial.println("unable to find HX711.");
      }
    #endif
    scales[i].set_scale(calibrationFactors[i]);
    scales[i].tare();
  }
  timer = millis(); // Start timer
}


void WeighStation::calibrate(HX711 scale){
  if(scale.get_units(SCALE_AVERAGES) > ZERO_THRESHOLD || scale.get_units(SCALE_AVERAGES) < -ZERO_THRESHOLD){
    scale.tare(); // Reset to zero
  }
}


void WeighStation::scan(){
  uint16_t onePos = 0, twoPos = 0, threePos = 0; // Position in capture array
  bool oneActive = false, twoActive = false, threeActive = false;
  
  for(uint8_t i = 0; i < nScales; i++){
    float weight = scales[i].get_units(SCALE_AVERAGES);
    // Check if animal is on scale
    if(weight >= minWeight){
      // Identify which scale is active and append weight (with timestamp)
      if(i == 0){
        oneActive = true;
        scaleOneWeights[onePos] = weight * 100.0; // Convert float to int
        scaleOneTimes[onePos++] = millis() - timer;
        #ifdef DEBUG
          Serial.print("Scale one active: ");
          Serial.println(weight);
        #endif
      }
      else if(i == 1){
        twoActive = true;
        scaleTwoWeights[twoPos] = weight * 100.0;
        scaleTwoTimes[twoPos++] = millis() - timer;
        #ifdef DEBUG
          Serial.print("Scale two active: ");
          Serial.println(weight);
        #endif

      }
      else if(i == 2){
        threeActive = true;
        scaleThreeWeights[threePos] = weight * 100.0;
        scaleThreeTimes[threePos++] = millis() - timer;
        #ifdef DEBUG
          Serial.print("Scale ahree active: ");
          Serial.println(weight);
        #endif
      }
    }
  }


  
}
