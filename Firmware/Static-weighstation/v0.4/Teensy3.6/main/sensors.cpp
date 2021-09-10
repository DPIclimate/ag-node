#include "sensors.h"

// Struct of parameters to store on SD card and send over LoRaWAN 
static parameters_t parameters;

// Weights and timeStamps
static int16_t weights[WeighStation::nScales][WeighStation::maxArrSize];
static int16_t timeStamps[WeighStation::nScales][WeighStation::maxArrSize];

// Setup pin mapping for data and clock lines
static constexpr uint8_t scaleData[WeighStation::nScales] = {4, 21, 30};
static constexpr uint8_t scaleClock[WeighStation::nScales] = {3, 20, 29};

// Set calibration factor for scale (see calibration script)
static constexpr int16_t calibrationFactors[WeighStation::nScales] = {-1870, -1840, -1800};

// Initalise HX711 library
static const HX711 scaleOne;
static const HX711 scaleTwo;
static const HX711 scaleThree;
static HX711 scales[WeighStation::nScales] = {scaleOne, scaleTwo, scaleThree};

// Initialse INA219 library
Adafruit_INA219 battery(Monitoring::batteryAddr);
Adafruit_INA219 solar(Monitoring::solarAddr);

void WeighStation::init(){
  for(uint8_t i = 0; i < nScales; i++){
    scales[i].begin(scaleData[i], scaleClock[i]);
    scales[i].set_scale(calibrationFactors[i]);
    scales[i].tare();
  }
  #ifdef DEBUG
    Serial.println("Scales ready...");
  #endif
}


void WeighStation::calibrate(HX711 scale){
  if(scale.get_units(SCALE_AVERAGES) > ZERO_THRESHOLD || scale.get_units(SCALE_AVERAGES) < -ZERO_THRESHOLD){
    scale.tare(); // Reset to zero
  }
}


void WeighStation::scan(){
  for(uint8_t i = 0; i < nScales; i++){
    float weight = scales[i].get_units(SCALE_AVERAGES);
    // Check if animal is on scale
    if(weight >= triggerWeight){
      // Identify which scale is active and append weight (with timestamp)
      if(i == 0){
        if(!oneActive){
          oneStartTime = millis();
        }
        oneActive = true;
        weights[i][onePos] = weight * 100.0; // Convert float to int with decimal places (i.e. *100)
        timeStamps[i][onePos++] = millis() - oneStartTime;
        #ifdef DEBUG
          Serial.print("Scale [0]: ");
          Serial.print(timeStamps[i][onePos - 1] / 1000.0);
          Serial.print("\t");
          Serial.println(weight);
        #endif
      }
      else if(i == 1){
        if(!twoActive){
          twoStartTime = millis();
        }
        twoActive = true;
        weights[i][twoPos] = weight * 100.0;
        timeStamps[i][twoPos++] = millis() - twoStartTime;
        #ifdef DEBUG
          Serial.print("Scale [1]: ");
          Serial.print(timeStamps[i][twoPos - 1] / 1000.0);
          Serial.print("\t");
          Serial.println(weight);
        #endif

      }
      else if(i == 2){
        if(!threeActive){
          threeStartTime = millis();
        }
        threeActive = true;
        weights[i][threePos] = weight * 100.0;
        timeStamps[i][threePos++] = millis() - threeStartTime;
        #ifdef DEBUG
          Serial.print("Scale [2]: ");
          Serial.print(timeStamps[i][threePos - 1] / 1000.0);
          Serial.print("\t");
          Serial.println(weight);
        #endif
      }
      
    }
    else if(oneActive && i == 0 && weight <= stopWeight){
      int8_t* payload = construct_payload(i);
//      Memory::write_data(weights[i], timeStamps[i], payload, parameters);
      #ifdef DEBUG
        Serial.println("Scale one finished.");
      #endif
      oneActive = false;
      onePos = 0;
      oneStartTime = 0;
    }
    else if(twoActive && i == 1 && weight <= stopWeight){
      int8_t* payload = construct_payload(i);
//      Memory::write_data(weights[i], timeStamps[i], payload, parameters);
      #ifdef DEBUG
        Serial.println("Scale two finished.");
      #endif
      twoActive = false;
      twoPos = 0;
      twoStartTime = 0;
    }
    else if(threeActive && i == 2 && weight <= stopWeight){
      int8_t* payload = construct_payload(i);
//      Memory::write_data(weights[i], timeStamps[i], payload, parameters);
      #ifdef DEBUG
        Serial.println("Scale three finished.");
      #endif
      threeActive = false;
      threePos = 0;
      threeStartTime = 0;
    }
  }
}


int8_t* WeighStation::construct_payload(uint8_t scaleID){
  static int8_t payload[WEIGH_PAYLOAD_SIZE];

  for(int i=0; i < maxArrSize; i++){
    Serial.println(weights[scaleID][i]);
    if(weights[scaleID][i] == 0) break;
  }
  
    // === Get current time as a UNIX epoch format ===
  parameters.unixTime = now();
  #ifdef DEBUG
    Serial.print("UNIX Time:\t");
    Serial.println(parameters.unixTime);
  #endif
  payload[0] = parameters.unixTime;
  payload[1] = parameters.unixTime >> 8;
  payload[2] = parameters.unixTime >> 16;
  payload[3] = parameters.unixTime >> 24;
  
  // === Get weights from within array (drop zeros to improve averaging accuracy) ===
  uint16_t pos = 0; // Location within weight array
  while((weights[scaleID][pos] != 0 || pos <= 10) && pos <= maxArrSize){
    pos++;
  }

  // === Calculate parameters ===
  parameters.startWeight = weights[scaleID][uint16_t(pos * 0.25)]; // 25 %
  parameters.middleWeight = weights[scaleID][uint16_t(pos * 0.50)]; // 50 %
  parameters.endWeight = weights[scaleID][uint16_t(pos * 0.75)]; // 75 %

  #ifdef DEBUG
    Serial.print("Start Weight:\t");
    Serial.print(parameters.startWeight / 100.0f);
    Serial.println(" kg");
    Serial.print("Middle Weight:\t");
    Serial.print(parameters.middleWeight / 100.0f);
    Serial.println(" kg");
    Serial.print("End Weight:\t");
    Serial.print(parameters.endWeight / 100.0f);
    Serial.println(" kg");
  #endif

  parameters.scaleID = scaleID;
  #ifdef DEBUG
    Serial.print("Scale ID:\t");
    Serial.println(parameters.scaleID);
  #endif
  payload[4] = parameters.scaleID;
  payload[5] = parameters.startWeight;
  payload[6] = parameters.startWeight >> 8;
  
  payload[7] = parameters.middleWeight;
  payload[8] = parameters.middleWeight >> 8;
  
  payload[9] = parameters.endWeight;
  payload[10] = parameters.endWeight >> 8;

  // === Sum weights and calculate average ===
  uint32_t sumWeights = 0;
  for(uint16_t i = uint16_t(pos * 0.25); i < uint16_t(pos*0.75); i++){
    sumWeights += weights[scaleID][i];
  }
  parameters.avWeight = sumWeights / ((pos * 0.75) - (pos * 0.25));
  #ifdef DEBUG
    Serial.print("AV Weight:\t");
    Serial.print(parameters.avWeight / 100.0f);
    Serial.println(" kg");
  #endif
  payload[11] = parameters.avWeight;
  payload[12] = parameters.avWeight >> 8;

  // === Change in weight ===
  parameters.deltaWeight = parameters.endWeight - parameters.startWeight;
  #ifdef DEBUG
    Serial.print("\u0394 Weight:\t");
    Serial.print(parameters.deltaWeight / 100.0f);
    Serial.println(" kg");
  #endif
  payload[13] = parameters.deltaWeight;
  payload[14] = parameters.deltaWeight >> 8;

  // === Time on scale ===
  parameters.timeOnScale = timeStamps[scaleID][pos-1]; // Gets the time at the last weight
  #ifdef DEBUG
    Serial.print("Drink time:\t");
    Serial.print(parameters.timeOnScale / 1000.0f);
    Serial.println(" s");
  #endif
  payload[15] = parameters.timeOnScale;
  payload[16] = parameters.timeOnScale >> 8;

  // === Weight StDev ===
  int16_t stdevSum = 0;
  for(uint16_t i = uint16_t(pos * 0.25); i < uint16_t(pos*0.75); i++){
    stdevSum += pow((weights[scaleID][i] - parameters.avWeight), 2);
  }
  parameters.stdevWeight = sqrt(stdevSum/pos);
  #ifdef DEBUG
    Serial.print("StDev Weight:\t");
    Serial.print(parameters.stdevWeight / 100.0f);
    Serial.println(" kg");
  #endif
  payload[17] = parameters.stdevWeight;
  payload[18] = parameters.stdevWeight >> 8;

  #ifdef DEBUG
    Serial.print("Payload:\t");
    for(uint8_t i = 0; i < sizeof(payload); i++){
      if (i != 0) Serial.print("-");
      payload[i] &= 0xff;
      if (payload[i] < 16)
          Serial.print('0');
      Serial.print(payload[i], HEX);
    } Serial.println("");
  #endif

  return payload;
}


void Monitoring::init(){
  #ifdef DEBUG
    if(battery.begin()){
      Serial.println("Battery monitoring initalisaed");
    } else Serial.println("Battery monitoring error");
    if(solar.begin()){
      Serial.println("Solar monitoring initalised");
    } else Serial.println("Solar monitoring error");
  #endif
}


int16_t Monitoring::voltage(Adafruit_INA219 ina219){
  float volts = ina219.getBusVoltage_V() + (ina219.getShuntVoltage_mV() / 1000);
  return (int16_t)volts * 100;
}


int16_t Monitoring::current(Adafruit_INA219 ina219){
  float curr = ina219.getCurrent_mA();
  return (int16_t)curr * 100;
}


int16_t Monitoring::power(Adafruit_INA219 ina219){
  float watts = ina219.getPower_mW();
  return (int16_t)watts * 100;
}


void Temperature::init(OneWire* oneWire){
  sensor(oneWire);
  sensor.begin();
  sensor.setResolution(TEMPERATURE_RESOLUTION);
}


int16_t Temperature::measure(DallasTemperature sensor){
  sensor.requestTemperatures();
  float tempC = sensor.getTempCByIndex(0);
  return (int16_t)tempC * 100;
}


void RealTimeClock::init(){
  #ifdef DEBUG
    if(timeStatus() != timeSet){
      Serial.print("Unable to sync with RTC...");
      set_time();
    } else {
      Serial.println("RTC has set the system time");
    }
  #endif
}


void RealTimeClock::set_time(){
  bool configuredTime = false;
  Serial.println("Set unix time (format = T1631254602):");
  while(!configuredTime){
    if(Serial.available()){
      String command = Serial.readStringUntil('\n');
      if(command.startsWith("T")){
        uint32_t userTime = command.substring(1, command.length()).toInt();
        RTC.set(userTime);
        setTime(userTime);
        Serial.print("Time set to: ");
        Serial.println(now());
        configuredTime = true;
      }
    }
    delay(100);
  }
}

int8_t* Sensors::construct_payload(){
  static int8_t payload[14];

  // === Power monitoring ===
  // Battery
  int16_t batteryV = Monitoring::voltage(battery);
  payload[0] = batteryV;
  payload[1] = batteryV >> 8;
  int16_t batteryA = Monitoring::current(battery);
  payload[2] = batteryA;
  payload[3] = batteryA >> 8;
  int16_t batteryW = Monitoring::power(battery);
  payload[4] = batteryW;
  payload[5] = batteryW >> 8;

  // Solar
  int16_t solarV = Monitoring::voltage(solar);
  payload[6] = solarV;
  payload[7] = solarV >> 8;
  int16_t solarA = Monitoring::current(solar);
  payload[8] = solarA;
  payload[9] = solarA >> 8;
  int16_t solarW = Monitoring::power(solar);
  payload[10] = solarW;
  payload[11] = solarW >> 8;

  // === Temperature monitoring ===
  int16_t tempC = Temperature::measure(Temperature::sensor);
  payload[12] = tempC;
  payload[13] = tempC >> 8;

  return payload;
}
