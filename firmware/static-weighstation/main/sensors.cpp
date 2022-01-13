/**
All sensor related functions, including:
Weighstation data, temperature monitoring, power monitoring and payload handling
*/

#include "sensors.h"

// Struct of parameters to store on SD card and send over LoRaWAN 
static parameters_t parameters;

// Weights and timeStamps
static int16_t weights[WeighStation::nScales][WeighStation::maxArrSize];
static int32_t timeStamps[WeighStation::nScales][WeighStation::maxArrSize];

// Setup pin mapping for data and clock lines
static constexpr uint8_t scaleData[WeighStation::nScales] = {4, 21, 30};
static constexpr uint8_t scaleClock[WeighStation::nScales] = {3, 20, 29};

// Overtime check (animal has spent too long on the scales)
static bool overtime[WeighStation::nScales] = {false, false, false};

// Set calibration factor for scale (see calibration script)
static constexpr int16_t calibrationFactors[WeighStation::nScales] = {-1770, -1760, -1770};

// Initalise HX711 library
static HX711 scaleOne;
static HX711 scaleTwo;
static HX711 scaleThree;
// Store HX711 objects in array for easy indexing
static constexpr HX711* scales[WeighStation::nScales] = {&scaleOne, &scaleTwo, &scaleThree};

// Check active scale
static bool scaleActive[WeighStation::nScales] = {false, false, false};

// Note start time (using millis())
static uint32_t weighStartTime[WeighStation::nScales] = {0, 0, 0};

// Position in weights array
static uint16_t readingPos[WeighStation::nScales] = {0, 0, 0};

// Initialse INA219 (power monitoring) library 
static Adafruit_INA219 battery(Monitoring::batteryAddr);
static Adafruit_INA219 solar(Monitoring::solarAddr);

// DallasTemperature (oneWire)
DallasTemperature sensor;

uint32_t unixTime = 1640124900; // Current UNIX time

// Current position in payloads array
uint8_t WeighStation::payloadPos = 0;

// Solar and battery monitoring
bool Monitoring::batteryConnected = false;
bool Monitoring::solarConnected = false;


/**
Initialise the weighstation. 
Set their calibration factors.
Zero them (tare).
*/
void WeighStation::init() {
  Serial.print("[WEIGH STATION]: Initialising");
  for(uint8_t i = 0; i < nScales; i++) {
    scales[i]->begin(scaleData[i], scaleClock[i]);
    scales[i]->set_scale(calibrationFactors[i]);
    scales[i]->tare();
    Serial.print(".");
  }
  Serial.println(" success");
}


/**
Reset all scales to zero (tare weight).
Call often as scales drift over time.
*/
void WeighStation::tare_scales() {
  #ifdef DEBUG
    Serial.println("[WEIGH STATION]: Tare scales.");
  #endif
  for(uint8_t i = 0; i < nScales; i++) {
    scales[i]->tare(); // Reset to zero
  }
}

/**
Power down ADC's (HX711) when in sleep mode.
*/
void WeighStation::sleep() {
  for(uint8_t i = 0; i < nScales; i++) {
    scales[i]->power_down();
  }
}


/**
Power up ADC's (HX711) when coming out of sleep mode.
*/
void WeighStation::wakeup() {
  for(uint8_t i = 0; i < nScales; i++) {
    scales[i]->power_up();
  }
}

/**
Main function within WeighStation class.
Handles the functioning of each scale and keeps track of data.
*/
void WeighStation::scan(){
  for(uint8_t i = 0; i < nScales; i++){
    float weight = scales[i]->get_units(SCALE_AVERAGES);  
    if(weight >= triggerWeight){
      if(!scaleActive[i]){
        weighStartTime[i] = millis();
      }
      if(readingPos[i] > (WeighStation::maxArrSize - 1)){
        overtime[i] = true;
        #ifdef DEBUG
          Serial.println("Overtime");
        #endif
      }
      else {
        scaleActive[i] = true;
        weights[i][readingPos[i]] = weight * 100.0; // Convert float to int with decimal places
        timeStamps[i][readingPos[i]] = millis() - weighStartTime[i];
        #ifdef DEBUG
          Serial.print("Scale [");
          Serial.print(i);
          Serial.print("]: ");
          Serial.print(timeStamps[i][readingPos[i]] / 1000.0);
          Serial.print("\t");
          Serial.println(weight);
        #endif
        readingPos[i]++;
      }
    }
    else if(scaleActive[i]) {
      int8_t* payload = construct_payload(i);
      append_payload(payload); // Append payload to stack (to send over LoRaWAN)
      Memory::write_weigh_data(weights[i], timeStamps[i], payload, parameters);
      #ifdef DEBUG
        Serial.println("[WEIGH STATION]: Scale one finished.");
      #endif
      // Reset varaibles and arrays to initial values
      scaleActive[i] = false;
      overtime[i] = false;
      readingPos[i] = 0;
      weighStartTime[i] = 0;
      memset(weights[i], 0, sizeof(weights[i]));
      memset(timeStamps[i], 0, sizeof(timeStamps[i]));
    }
  }
}


/**
Construct payload to send over LoRaWAN and calculate weighscale reading parameters.
@param scaleID ID of scale which these reading correspond to.
@returns payload A payload to send over LoRaWAN.
*/
int8_t* WeighStation::construct_payload(uint8_t scaleID) {
  static int8_t payload[WEIGH_PAYLOAD_SIZE];

  Serial.println("[WEIGH STATION]: Creating payload...");

  /*
   * Loop through array to find first zero value. 
   * This puts the array in context by ignoring all following zeros and focusing on actual values.
   */

  // === Payload identifier ===
  int8_t payloadType = 0;
  payload[0] = payloadType;
  
  // === Get current time as a UNIX epoch format ===
  parameters.unixTime = now();
  #ifdef DEBUG
    Serial.print("UNIX Time:\t");
    Serial.println(parameters.unixTime);
  #endif
  payload[1] = parameters.unixTime;
  payload[2] = parameters.unixTime >> 8;
  payload[3] = parameters.unixTime >> 16;
  payload[4] = parameters.unixTime >> 24;
  
  // === Get weights from within array (drop zeros to improve averaging accuracy) ===
  uint16_t pos; // Location within weight array
  for(pos = 0; pos < (WeighStation::maxArrSize - 1); pos++){
    if(weights[scaleID][pos] == 0 and pos > 10) break;
  }

  // === Calculate parameters ===
  uint16_t p25 = (pos * 0.25); // 25 %
  uint16_t p50 = (pos * 0.50); // 50 %
  uint16_t p75 = (pos * 0.75); // 75 %
  parameters.startWeight = weights[scaleID][p25];
  parameters.middleWeight = weights[scaleID][p50];
  parameters.endWeight = weights[scaleID][p75];

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
  payload[5] = parameters.scaleID;
  payload[6] = parameters.startWeight;
  payload[7] = parameters.startWeight >> 8;
  
  payload[8] = parameters.middleWeight;
  payload[9] = parameters.middleWeight >> 8;
  
  payload[10] = parameters.endWeight;
  payload[11] = parameters.endWeight >> 8;

  // === Sum weights and calculate average ===
  uint32_t sumWeights = 0;
  for(uint16_t i = p25; i < p75; i++) {
    sumWeights += weights[scaleID][i];
  }
  parameters.avWeight = sumWeights / (p75 - p25);
  
  #ifdef DEBUG
    Serial.print("AV Weight:\t");
    Serial.print(parameters.avWeight / 100.0f);
    Serial.println(" kg");
  #endif
  payload[12] = parameters.avWeight;
  payload[13] = parameters.avWeight >> 8;

  // === Change in weight ===
  parameters.deltaWeight = parameters.endWeight - parameters.startWeight;
  #ifdef DEBUG
    Serial.print("\u0394 Weight:\t");
    Serial.print(parameters.deltaWeight / 100.0f);
    Serial.println(" kg");
  #endif
  payload[14] = parameters.deltaWeight;
  payload[15] = parameters.deltaWeight >> 8;

  // === Time on scale ===
  parameters.timeOnScale = timeStamps[scaleID][pos-1]; // Gets the time at the last weight
  #ifdef DEBUG
    Serial.print("Drink time:\t");
    Serial.print(parameters.timeOnScale / 1000.0f);
    Serial.println(" s");
  #endif
  payload[16] = parameters.timeOnScale;
  payload[17] = parameters.timeOnScale >> 8;
  payload[18] = parameters.timeOnScale >> 16;
  payload[19] = parameters.timeOnScale >> 24;

  // === Weight StDev ===
  uint32_t stdevSum = 0;
  int16_t lenSum = 0;
  for(uint16_t i = p25; i < p75; i++) {
    stdevSum += pow((weights[scaleID][i] - parameters.avWeight), 2);
    lenSum++;
  }

  parameters.stdevWeight = sqrt((1.0f/lenSum) * stdevSum);
  
  #ifdef DEBUG
    Serial.print("StDev Weight:\t");
    Serial.print(parameters.stdevWeight / 100.0f);
    Serial.println(" kg");
  #endif
  payload[20] = parameters.stdevWeight;
  payload[21] = parameters.stdevWeight >> 8;

  // === Estimated Weight ===
  // Most "accurate" weight reading
  parameters.estimatedWeight = 0;

  // Use average weight as stdev is low
  if((parameters.stdevWeight / 100.0f) < 2.0f){
    parameters.estimatedWeight = parameters.avWeight;
    #ifdef DEBUG
      Serial.print("Estimated weight (average weight):\t");
      Serial.print(parameters.estimatedWeight / 100.0f);
      Serial.println(" kg");
    #endif
  }
  else {
    // Use last weight
    if((parameters.deltaWeight / 100.0f) > 5.0f){
      parameters.estimatedWeight = weights[scaleID][p75];
      #ifdef DEBUG
        Serial.print("Estimated weight (p75 weight):\t");
        Serial.print(parameters.estimatedWeight / 100.0f);
        Serial.println(" kg");
      #endif
    }
    // Use first weight
    else if((parameters.deltaWeight / 100.0f) < 5.0f){
      parameters.estimatedWeight = weights[scaleID][p25];
      #ifdef DEBUG
        Serial.print("Estimated weight (p25 weight):\t");
        Serial.print(parameters.estimatedWeight / 100.0f);
        Serial.println(" kg");
      #endif
    }
  }

  payload[22] = parameters.estimatedWeight;
  payload[23] = parameters.estimatedWeight >> 8;

  #ifdef DEBUG
    Serial.print("Payload:\t");
    for(uint8_t i = 0; i < sizeof(payload); i++) {
      if (i != 0) Serial.print("-");
      payload[i] &= 0xff;
      if (payload[i] < 16)
          Serial.print('0');
      Serial.print(payload[i], HEX);
    } Serial.println("");
  #endif

  return payload;
}


/**
Add payload to the stack to be sent over LoRaWAN when the next window opens.
@param payload An array consiting of a payload to send.
*/
void WeighStation::append_payload(int8_t* payload) {

  if(payloadPos < STORED_PAYLOAD_SIZE){
    #ifdef DEBUG
      Serial.println("[WEIGH STATION]: Appending payload to LoRaWAN stack...");
    #endif
    for(uint8_t i = 0; i < WEIGH_PAYLOAD_SIZE; i++) {
      payloads[payloadPos][i] = payload[i];
    }
    payloadPos++;
  }
  else{
    #ifdef DEBUG
      Serial.print("[WEIGH STATION]: Too many payloads in queue.");
    #endif
  }
}


/**
Pop a payload off the pending stack and send it over LoRaWAN.
Decrement the payload position to lower down the stack.
*/
void WeighStation::forward_payload() {
  #ifdef DEBUG
    Serial.print("[WEIGH STATION]: Forwarding payload at position:\t");
    Serial.println(payloadPos - 1);
  #endif
  #ifdef ENABLE_LORAWAN
    Lora::request_send(payloads[payloadPos - 1], 1);
    while(!Lora::check_state()) {
      os_runloop_once();
    }
  #endif
  // Decrement payload pos to next payload
  if(payloadPos != 0) payloadPos--;
}


/**
Check if there is an animal on any one of the scales.
@returns A bool indicating if this is true (there is an animal) or false
*/
bool WeighStation::check_state() {
   for(int i = 0; i < nScales; i++){
    if(scaleActive[i]){
      return true;
    }
   }
   return false;
}


/**
Initialise power monitoring for battery and solar power.
*/
void Monitoring::init() {
  Serial.print("[MONITORING]: initialising battery... ");
  if(battery.begin()) {
    Monitoring::batteryConnected = true;
    battery.setCalibration_16V_400mA();
    Serial.println("success");
  } else Serial.println("failed");

  Serial.print("[MONITORING]: initialising solar... ");
  if(solar.begin()) {
    Monitoring::solarConnected = true;
    solar.setCalibration_32V_1A();
    Serial.println("success");
  } else Serial.println("failed");
}


/**
Get the voltage of either the battery or solar panel.
@param type Is the device type ('b' = battery, 's' = solar).
@returns volts Voltage of device x100 for float conversion.
*/
int16_t Monitoring::voltage(char type) {
  float volts = 0;
  if(type == 'b') {
    volts = battery.getBusVoltage_V() + (battery.getShuntVoltage_mV() / 1000.0f);
  } 
  else if(type == 's') {
    volts = solar.getBusVoltage_V() + (solar.getShuntVoltage_mV() / 1000.0f);
  } else{
    #ifdef DEBUG
      Serial.println("[MONITORING]: Unknown device type.");
    #endif
    return (int16_t)volts;
  }
  #ifdef DEBUG
    Serial.print("[MONITORING]: Millivolts:\t");
    Serial.println(volts);
  #endif
  return (int16_t)(volts * 100.0f);
}


/**
Get the current of either the battery or solar panel.
@param type Is the device type ('b' = battery, 's' = solar).
@returns curr Current of device x100 for float conversion.
*/
int16_t Monitoring::current(char type) {
  float curr = 0;
  if(type == 'b') {
    curr = battery.getCurrent_mA();
  } 
  else if(type == 's') {
    curr = solar.getCurrent_mA();
  } else {
    #ifdef DEBUG
      Serial.println("[MONITORING]: Unknown device type.");
    #endif
    return (int16_t)curr;
  }
  #ifdef DEBUG
    Serial.print("[MONITORING]: Milliamps:\t");
    Serial.println(curr);
  #endif
  return (int16_t)(curr * 100.0f);
}


/**
Get the power of either the battery or solar panel.
@param type Is the device type ('b' = battery, 's' = solar).
@returns watts Power of device x100 for float conversion.
*/
int16_t Monitoring::power(char type) {
  float watts = 0;
  if(type == 'b') {
    watts = battery.getPower_mW();
  }
  else if(type == 's') {
    watts = solar.getPower_mW();
  } else{
    #ifdef DEBUG
      Serial.println("[MONITORING]: Unknown device type.");
    #endif
    return (int16_t)watts;
  }
  #ifdef DEBUG
    Serial.print("[MONITORING]: Milliwatts:\t");
    Serial.println(watts);
  #endif
  return (int16_t)(watts * 100.0f);
}


/**
Initialise temperatrue sensors - OneWire interface.
*/
void Temperature::init(DallasTemperature sensors) {
  sensor = sensors;
  sensor.begin();
  sensor.setResolution(TEMPERATURE_RESOLUTION);
}


/*
Measure the current temperature.
@returns tempC The current temperature using x100 float conversion.
*/
int16_t Temperature::measure() {
  sensor.requestTemperatures();
  float tempC = sensor.getTempCByIndex(0);
  #ifdef DEBUG 
    Serial.print("[TEMPERATURE]: Temperature:\t");
    Serial.print(tempC);
    Serial.println("\u02DAC");
  #endif
  return (int16_t)(tempC * 100.0f);
}


/**
Initialise real time clock (RTC).
*/
void RealTimeClock::init() {
  if(timeStatus() != timeSet) {
    Serial.println("[RTC]: Unable to sync.");
    set_time();
  } else {
    Serial.println("[RTC]: RTC has set the system time.");
  }
}


/**
Set the time of the RTC in UNIX epoch time.
*/
void RealTimeClock::set_time() {
  
  #ifdef DEBUG
  bool configuredTime = false;
  Serial.println("[RTC]: User must set unix time (format = T1631254602):");
  while(!configuredTime) {
    if(Serial.available()) {
      String command = Serial.readStringUntil('\n');
      if(command.startsWith("T")) {
        uint32_t userTime = command.substring(1, command.length()).toInt();
        RTC.set(userTime);
        setTime(userTime);
        Serial.print("[RTC]: Time set to: ");
        Serial.println(now());
        configuredTime = true;
      }
    }
    delay(100);
  }
  #else
    RTC.set(unixTime);
    setTime(unixTime);
    Serial.print("[RTC]: Set time: ");
    Serial.println(now());
   #endif
}


/**
Construct the sensor payload - sensors, monitoring, temperature and RTC.
@returns payload A payload containing sensor information.
*/
int8_t* Sensors::construct_payload() {
  Serial.println("[SENSORS]: Creating payload...");
  
  static int8_t payload[SENSORS_PAYLOAD_SIZE];

  // === Payload identifier ===
  int8_t payloadType = 1;
  payload[0] = payloadType;

  // === Current time ===
  uint32_t unixTime = now();
  payload[1] = unixTime;
  payload[2] = unixTime >> 8;
  payload[3] = unixTime >> 16;
  payload[4] = unixTime >> 24;
  #ifdef DEBUG
    Serial.print("UNIX Time:\t");
    Serial.println(unixTime);
  #endif

  // === Power monitoring ===
  // Battery
  int16_t batteryV = 0;
  int16_t batteryA = 0;
  int16_t batteryW = 0;
  if(Monitoring::batteryConnected){
    batteryV = Monitoring::voltage('b');
    batteryA = Monitoring::current('b');
    batteryW = Monitoring::power('b');
  }
  
  payload[5] = batteryV;
  payload[6] = batteryV >> 8;
  payload[7] = batteryA;
  payload[8] = batteryA >> 8;
  payload[9] = batteryW;
  payload[10] = batteryW >> 8;

  // Solar
  int16_t solarV = 0;
  int16_t solarA = 0;
  int16_t solarW = 0;
  if(Monitoring::solarConnected){
    solarV = Monitoring::voltage('s');
    solarA = Monitoring::current('s');
    solarW = Monitoring::power('s');
  }

  payload[11] = solarV;
  payload[12] = solarV >> 8;
  payload[13] = solarA;
  payload[14] = solarA >> 8;
  payload[15] = solarW;
  payload[16] = solarW >> 8;

  // === Temperature monitoring ===
  int16_t tempC = Temperature::measure();
  payload[17] = tempC;
  payload[18] = tempC >> 8;

  return payload;
}
