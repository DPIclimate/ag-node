/* 
 * All sensor related functions, including:
 * Weighstation data, temperature monitoring, power monitoring and payload handling
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

// Set calibration factor for scale (see calibration script)
static constexpr int16_t calibrationFactors[WeighStation::nScales] = {-1770, -1760, -1770};

// Initalise HX711 library
static const HX711 scaleOne;
static const HX711 scaleTwo;
static const HX711 scaleThree;
// Store HX711 objects in array for easy indexing
static HX711 scales[WeighStation::nScales] = {scaleOne, scaleTwo, scaleThree};

// Initialse INA219 (power monitoring) library 
static Adafruit_INA219 battery(Monitoring::batteryAddr);
static Adafruit_INA219 solar(Monitoring::solarAddr);

// DallasTemperature (oneWire)
DallasTemperature sensor;

uint32_t unixTime = 1635036471; // Current UNIX time

// Current position in payloads array
uint8_t WeighStation::payloadPos = 0;


void WeighStation::init(){
  /*
   * Initialise the weighstation. 
   * Set their calibration factors.
   * Zero them (tare).
   */
  Serial.print("[WEIGH STATION]: Initialising");
  for(uint8_t i = 0; i < nScales; i++){
    scales[i].begin(scaleData[i], scaleClock[i]);
    scales[i].set_scale(calibrationFactors[i]);
    scales[i].tare();
    Serial.print(".");
  }
  Serial.println(" success");
}


void WeighStation::tare_scales(){
  /*
   * Reset all scales to zero (tare).
   * Call often as scales drift over time.
   */
  #ifdef DEBUG
    Serial.println("[WEIGH STATION]: Tare scales.");
  #endif
  for(uint8_t i = 0; i < nScales; i++){
    scales[i].tare(); // Reset to zero
  }
}


void WeighStation::sleep(){
  for(uint8_t i = 0; i < nScales; i++){
    scales[i].power_down();
  }
}


void WeighStation::wakeup(){
  for(uint8_t i = 0; i < nScales; i++){
    scales[i].power_up();
  }
}


void WeighStation::scan(){
  /*
   * Main weighstation function. Loops through HX711 objects and checks if they exceed a trigger weight,
   * signifying an animal is on the scale. If this is true the device will record the weight into two 2D
   * arrays (holding the scale id and weights / timestamps).
   * Once the animal has stepped of the particular scale (below trigger weight) some functions are called 
   * to calculate parameters, append packets to a LoRaWAN stack (to send in the future) and append to a SD-card.
   */
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
        weights[i][onePos] = weight * 100.0; // Convert float to int with decimal places
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
    else if(oneActive && i == 0){
      int8_t* payload = construct_payload(i);
      append_payload(payload); // Append payload to stack (to send over LoRaWAN)
      Memory::write_weigh_data(weights[i], timeStamps[i], payload, parameters);
      #ifdef DEBUG
        Serial.println("[WEIGH STATION]: Scale one finished.");
      #endif
      // Reset varaibles and arrays to initial values
      oneActive = false;
      onePos = 0;
      oneStartTime = 0;
      memset(weights[i], 0, sizeof(weights[i]));
      memset(timeStamps[i], 0, sizeof(timeStamps[i]));
    }
    else if(twoActive && i == 1){
      int8_t* payload = construct_payload(i);
      append_payload(payload);
      Memory::write_weigh_data(weights[i], timeStamps[i], payload, parameters);
      #ifdef DEBUG
        Serial.println("[WEIGH STATION]: Scale two finished.");
      #endif
      twoActive = false;
      twoPos = 0;
      twoStartTime = 0;
      memset(weights[i], 0, sizeof(weights[i]));
      memset(timeStamps[i], 0, sizeof(timeStamps[i]));
    }
    else if(threeActive && i == 2){
      int8_t* payload = construct_payload(i);
      append_payload(payload);
      Memory::write_weigh_data(weights[i], timeStamps[i], payload, parameters);
      #ifdef DEBUG
        Serial.println("[WEIGH STATION]: Scale three finished.");
      #endif
      threeActive = false;
      threePos = 0;
      threeStartTime = 0;
      memset(weights[i], 0, sizeof(weights[i]));
      memset(timeStamps[i], 0, sizeof(timeStamps[i]));
    }
  }
}


int8_t* WeighStation::construct_payload(uint8_t scaleID){
  /*
   * Construct payload to send over LoRaWAN and calculate weighscale reading parameters.
   * @param scaleID ID of scale which these reading correspond to.
   * @return payload A payload to send over LoRaWAN.
   */
  static int8_t payload[WEIGH_PAYLOAD_SIZE];

  /*
   * Loop through array to find first zero value. 
   * This puts the array in context by ignoring all following zeros and focusing on actual values.
   */

  Serial.println("[WEIGH STATION]: Creating payload...");
  
  for(int i=0; i < maxArrSize; i++){
    if(weights[scaleID][i] == 0) break;
  }

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
  payload[5] = parameters.scaleID;
  payload[6] = parameters.startWeight;
  payload[7] = parameters.startWeight >> 8;
  
  payload[8] = parameters.middleWeight;
  payload[9] = parameters.middleWeight >> 8;
  
  payload[10] = parameters.endWeight;
  payload[11] = parameters.endWeight >> 8;

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
  payload[20] = parameters.stdevWeight;
  payload[21] = parameters.stdevWeight >> 8;

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


void WeighStation::append_payload(int8_t* payload){
  /*
   * Add payload to the stack to be sent over LoRaWAN when the next window opens.
   * @param payload An array consiting of a payload to send.
   */
  #ifdef DEBUG
    Serial.println("[WEIGH STATION]: Appending payload to LoRaWAN stack...");
  #endif
  for(uint8_t i = 0; i < WEIGH_PAYLOAD_SIZE; i++){
    payloads[payloadPos][i] = payload[i];
  }
  payloadPos++;
}


void WeighStation::forward_payload(){
  /*
   * Pop a payload off the pending stack and send it over LoRaWAN.
   * Decrement the payload position to lower down the stack.
   */
  #ifdef DEBUG
    Serial.print("[WEIGH STATION]: Forwarding payload at position:\t");
    Serial.println(payloadPos - 1);
  #endif
  #ifdef ENABLE_LORAWAN
    Lora::request_send(payloads[payloadPos - 1], 1);
    while(!Lora::check_state()){
      os_runloop_once();
    }
  #endif
  // Decrement payload pos to next payload
  if(payloadPos != 0) payloadPos--;
}


bool WeighStation::check_state(){
  /*
   * Check if there is an animal on any one of the scales.
   * @return A bool indicating if this is true (there is an animal) or false (there isn't)
   */
  if(!oneActive && !twoActive && !threeActive){
    return false;
  } else {
    return true;
  }
}


void Monitoring::init(){
  /*
   * Initialise power monitoring for battery and solar power.
   */
  Serial.print("[MONITORING]: initialising battery... ");
  if(battery.begin()){
    battery.setCalibration_16V_400mA();
    Serial.println("success");
  } else Serial.println("failed");

  Serial.print("[MONITORING]: initialising solar... ");
  if(solar.begin()){
    solar.setCalibration_32V_1A();
    Serial.println("success");
  } else Serial.println("failed");
}


int16_t Monitoring::voltage(char type){
  /*
   * Get the voltage of either the battery or solar panel.
   * @param type Is the device type ('b' = battery, 's' = solar).
   * @return volts Voltage of device x100 for float conversion.
   */
  float volts = 0;
  if(type == 'b'){
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


int16_t Monitoring::current(char type){
  /*
   * Get the current of either the battery or solar panel.
   * @param type Is the device type ('b' = battery, 's' = solar).
   * @return curr Current of device x100 for float conversion.
   */
  float curr = 0;
  if(type == 'b'){
    curr = battery.getCurrent_mA();
  } 
  else if(type == 's'){
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


int16_t Monitoring::power(char type){
  /*
   * Get the power of either the battery or solar panel.
   * @param type Is the device type ('b' = battery, 's' = solar).
   * @return watts Power of device x100 for float conversion.
   */
  float watts = 0;
  if(type == 'b'){
    watts = battery.getPower_mW();
  }
  else if(type == 's'){
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


void Temperature::init(DallasTemperature sensors){
  /*
   * Initialise temperatrue sensors - OneWire interface.
   */
  sensor = sensors;
  sensor.begin();
  sensor.setResolution(TEMPERATURE_RESOLUTION);
}


int16_t Temperature::measure(){
  /*
   * Measure the current temperature.
   * @return tempC The current temperature using x100 float conversion.
   */
  sensor.requestTemperatures();
  float tempC = sensor.getTempCByIndex(0);
  #ifdef DEBUG 
    Serial.print("[TEMPERATURE]: Temperature:\t");
    Serial.print(tempC);
    Serial.println("\u02DAC");
  #endif
  return (int16_t)(tempC * 100.0f);
}


void RealTimeClock::init(){
  /*
   * Initialise real time clock (RTC).
   */
  if(timeStatus() != timeSet){
    Serial.println("[RTC]: Unable to sync.");
    set_time();
  } else {
    Serial.println("[RTC]: RTC has set the system time.");
  }
}


void RealTimeClock::set_time(){
  /*
   * Set the time of the RTC in UNIX epoch time.
   */
  
  #ifdef DEBUG
  bool configuredTime = false;
  Serial.println("[RTC]: User must set unix time (format = T1632191801):");
  while(!configuredTime){
    if(Serial.available()){
      String command = Serial.readStringUntil('\n');
      if(command.startsWith("T")){
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


int8_t* Sensors::construct_payload(){
  /*
   * Construct the sensor payload - sensors, monitoring, temperature and RTC.
   * @return payload A payload containing sensor information.
   */
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
  int16_t batteryV = Monitoring::voltage('b');
  payload[5] = batteryV;
  payload[6] = batteryV >> 8;
  int16_t batteryA = Monitoring::current('b');
  payload[7] = batteryA;
  payload[8] = batteryA >> 8;
  int16_t batteryW = Monitoring::power('b');
  payload[9] = batteryW;
  payload[10] = batteryW >> 8;

  // Solar
  int16_t solarV = Monitoring::voltage('s');
  payload[11] = solarV;
  payload[12] = solarV >> 8;
  int16_t solarA = Monitoring::current('s');
  payload[13] = solarA;
  payload[14] = solarA >> 8;
  int16_t solarW = Monitoring::power('s');
  payload[15] = solarW;
  payload[16] = solarW >> 8;

  // === Temperature monitoring ===
  int16_t tempC = Temperature::measure();
  payload[17] = tempC;
  payload[18] = tempC >> 8;

  return payload;
}
