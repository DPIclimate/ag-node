#include "sensors.h"

// Struct of parameters to store on SD card and send over LoRaWAN 
parameters_t parameters;

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
  uint32_t 
  for(uint8_t i = 0; i < nScales; i++){
    float weight = scales[i].get_units(SCALE_AVERAGES);
    // Check if animal is on scale
    if(weight >= minWeight){
      // Identify which scale is active and append weight (with timestamp)
      if(i == 0){
        oneActive = true;
        weights[i][onePos] = weight * 100.0; // Convert float to int with decimal places (i.e. *100)
        timeStamps[i][onePos++] = millis() - timer;
        #ifdef DEBUG
          Serial.print("Scale one active: ");
          Serial.println(weight);
        #endif
      }
      else if(i == 1){
        twoActive = true;
        weights[i][twoPos] = weight * 100.0;
        timeStamps[i][twoPos++] = millis() - timer;
        #ifdef DEBUG
          Serial.print("Scale two active: ");
          Serial.println(weight);
        #endif

      }
      else if(i == 2){
        threeActive = true;
        weights[i][threePos] = weight * 100.0;
        timeStamps[i][threePos++] = millis() - timer;
        #ifdef DEBUG
          Serial.print("Scale three active: ");
          Serial.println(weight);
        #endif
      }
      
    }
    else if(oneActive && i == 0 && weight <= stopWeight){
      oneActive = false;
      Sensors::construct_payload(i);
      Memory::write_data(weights[i], timeStamps[i], Sensors::payload, parameters);
      #ifdef DEBUG
        Serial.println("Scale one finished.");
      #endif
    }
    else if(twoActive && i == 1 && weight <= stopWeight){
      twoActive = false;
      Sensors::construct_payload(i);
      Memory::write_data(weights[i], timeStamps[i], Sensors::payload, parameters);
      #ifdef DEBUG
        Serial.println("Scale two finished.");
      #endif
    }
    else if(threeActive && i == 2 && weight <= stopWeight){
      threeActive = false;
      Sensors::construct_payload(i);
      Memory::write_data(weights[i], timeStamps[i], Sensors::payload, parameters);
      #ifdef DEBUG
        Serial.println("Scale three finished.");
      #endif
    }
  }
}


void Sensors::construct_payload(uint8_t scaleId){
  memset(Sensors::payload, 0, sizeof(Sensors::payload)); // Clear payload
  
  // === Get current time as a UNIX epoch format ===
  parameters.unixTime = now();
  payload[0] = parameters.unixTime;
  payload[1] = parameters.unixTime >> 8;
  payload[2] = parameters.unixTime >> 16;
  payload[3] = parameters.unixTime >> 24;
  
  // === Get weights from within array (drop zeros to improve averaging accuracy) ===
  uint16_t pos = 0; // Location within weight array
  while((WeighStation::weights[scaleId][pos] != 0 || pos <= 5) && pos <= WeighStation::maxArrSize){
    pos++;
  }

  // === Calculate parameters ===
  parameters.startWeight = WeighStation::weights[scaleId][uint16_t(pos * 0.25)]; // 25 %
  parameters.middleWeight = WeighStation::weights[scaleId][uint16_t(pos * 0.50)]; // 50 %
  parameters.endWeight = WeighStation::weights[scaleId][uint16_t(pos * 0.75)]; // 75 %

  parameters.scaleId = scaleId;
  payload[4] = parameters.scaleId;
  payload[5] = parameters.startWeight;
  payload[6] = parameters.startWeight >> 8;
  
  payload[7] = parameters.middleWeight;
  payload[8] = parameters.middleWeight >> 8;
  
  payload[9] = parameters.endWeight;
  payload[10] = parameters.endWeight >> 8;

  // === Sum weights and calculate average ===
  uint32_t sumWeights = 0;
  for(uint16_t i = uint16_t(pos * 0.25); i < uint16_t(pos*0.75); i++){
    sumWeights += WeighStation::weights[scaleId][i];
  }
  parameters.avWeight = sumWeights / ((pos * 0.75) - (pos * 0.25));

  payload[11] = parameters.avWeight;
  payload[12] = parameters.avWeight >> 8;

  // === Change in weight ===
  parameters.deltaWeight = parameters.endWeight - parameters.startWeight;

  payload[13] = parameters.deltaWeight;
  payload[14] = parameters.deltaWeight >> 8;

  // === Time on scale ===
  pos = 0; // Location within timestamps array
  while(WeighStation::timeStamps[scaleId][pos] != 0 && pos <= WeighStation::maxArrSize){
    pos++;
  }
  parameters.timeOnScale = WeighStation::timeStamps[scaleId][pos];

  payload[15] = parameters.timeOnScale;
  payload[16] = parameters.timeOnScale >> 8;
}


void RealTimeClock::init(){
  setSyncProvider(RTC.get); // Get time from RTC
  #ifdef DEBUG
    if(timeStatus() != timeSet){
      Serial.println("Unable to sync with RTC");
    } else {
      Serial.println("RTC has set the system time");
    }
  #endif
}

void RealTimeClock::set_time(){
  bool configuredTime = false;
  while(!configuredTime){
    if(Serial.available()){
      String command = Serial.readStringUntil('\n');
      if(command.startsWith("T")){
        uint32_t userTime = command.substring(1, command.length()).toInt();
        RTC.set(userTime);
        setTime(userTime);
        configuredTime = true;
      }
    }
    delay(100);
  }
}
