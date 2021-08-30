#include "sensors.h"

// Struct of parameters to store on SD card and send over LoRaWAN 
parameters_t parameters;

// Holds payload to store on SD card and send over LoRaWAN 
int8_t Sensors::payload[PAYLOAD_SIZE];

// Arrays for holding weights and corresponding time
int16_t WeighStation::weights[(RESPONSE_SIZE / 2)];
int16_t WeighStation::timeStamps[(RESPONSE_SIZE / 2)]; 


void WeighStation::init(){
  Wire.begin();
}


void WeighStation::scale_one(){
  #ifdef DEBUG
    Serial.println("Device one requests I2C transmission...");
  #endif
  request_weights((uint8_t)1);
}


void WeighStation::scale_two(){
  #ifdef DEBUG
    Serial.println("Device two requests I2C transmission...");
  #endif
  request_weights((uint8_t)2);
}


void WeighStation::scale_three(){
  #ifdef DEBUG
    Serial.println("Device three requests I2C transmission...");
  #endif
  request_weights((uint8_t)3);
}



void WeighStation::request_weights(uint8_t scaleID){
  noInterrupts();
  Serial.println("==== Response ====");
  const uint8_t PACKET_SIZE = 20;
  uint8_t packet[RESPONSE_SIZE * 2];

  bool deviceFound = false;
  int16_t index = 0;
  for(unsigned int x = 0; x < (sizeof(packet) / PACKET_SIZE); x++){
    Wire.requestFrom(scaleID, PACKET_SIZE);
    while(Wire.available()) {
      packet[index++] = Wire.read(); // Append each byte to the response
      deviceFound = true;
    }
  }

  if (deviceFound){
      uint16_t weightsPosition = 0;
      uint16_t timeStampsPosition = 0;
      uint16_t shift = 0;
      for(uint16_t i = 0; i < (sizeof(packet) / 2); i++){
        shift = i + i;
        if(i < (RESPONSE_SIZE / 2)){
          weights[weightsPosition] = (packet[shift] << 8) | packet[shift + 1];
          weightsPosition++;
        } else{
          timeStamps[timeStampsPosition] = (packet[shift] << 8) | packet[shift + 1];
          timeStampsPosition++;
        }
      }
    } else {
      Serial.println("Device not found...");
  }

  // Construct payload for LoRaWAN and SD-Card
  Sensors::construct_payload(scaleID);

  // Save to SD-Card
  Memory::write_data(WeighStation::weights, WeighStation::timeStamps, Sensors::payload, parameters);
  
  #ifdef ENABLE_LORAWAN
    // Send payload over LoRaWAN and wait until its sent
    Lora::request_send(Sensors::payload);
    while(!Lora::check_state()){
      os_runloop_once();
    }
  #endif

  #ifdef DEBUG
    for(int i = 0; i < (RESPONSE_SIZE / 2); i++){
      Serial.print(timeStamps[i]);
      Serial.print("\t");
      Serial.println((float)weights[i] / 100.0);
      if(weights[i] == 0 && i > 5){
        break;
      }
    }
  #endif

  // Reset arrays with zeros
  memset(Sensors::payload, 0, sizeof(Sensors::payload));
  memset(WeighStation::weights, 0, sizeof(WeighStation::weights));
  memset(WeighStation::timeStamps, 0, sizeof(WeighStation::timeStamps));
  interrupts();
}


void Sensors::construct_payload(uint8_t scaleID){
  memset(Sensors::payload, 0, sizeof(Sensors::payload)); // Clear payload
  
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
  while((WeighStation::weights[pos] != 0 || pos <= 10) && pos <= (RESPONSE_SIZE / 2)){
    pos++;
  }

  // === Calculate parameters ===
  parameters.startWeight = WeighStation::weights[uint16_t(pos * 0.25)]; // 25 %
  parameters.middleWeight = WeighStation::weights[uint16_t(pos * 0.50)]; // 50 %
  parameters.endWeight = WeighStation::weights[uint16_t(pos * 0.75)]; // 75 %

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
    sumWeights += WeighStation::weights[i];
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
    Serial.print("\u0394V Weight:\t");
    Serial.print(parameters.deltaWeight / 100.0f);
    Serial.println(" kg");
  #endif
  payload[13] = parameters.deltaWeight;
  payload[14] = parameters.deltaWeight >> 8;

  // === Time on scale ===
  parameters.timeOnScale = WeighStation::timeStamps[pos]; // Gets the time at the last weight
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
    stdevSum += pow((WeighStation::weights[i] - parameters.avWeight), 2);
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
  #ifdef DEBUG
    Serial.println("Enter the current UNIX time (format = T1630237934)");
    while(!configuredTime){
      if(Serial.available()){
        String command = Serial.readStringUntil('\n');
        if(command.startsWith("T")){
          uint32_t userTime = command.substring(1, command.length()).toInt();
          RTC.set(userTime);
          setTime(userTime);
          Serial.print("UNIX time set to:\t");
          Serial.println(userTime);
          configuredTime = true;
        }
      }
      delay(100);
    }
  #else
    RTC.set(unixTime);
    setTime(unixTime);
  #endif
}
