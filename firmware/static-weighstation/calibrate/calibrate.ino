// To be compiled for Teensy with connected HX711 load cell amplifier

#include "HX711.h"

// Scale 1 -1750
//#define DOUT  4 // SDA
//#define CLK  3 // SCL

//// Scale 2 -1730
//#define DOUT  21 // SDA
//#define CLK  20 // SCL

//// Scale 3 -1730
#define DOUT  30 // SDA
#define CLK  29 // SCL

HX711 scale;

int CALIBRATION_FACTOR = -1730;

void setup() {
  Serial.begin(9600);

  // Scale setup
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare();
}

void loop() {

  scale.set_scale(CALIBRATION_FACTOR);

  Serial.print("Weigh: ");
  Serial.print(scale.get_units(1), 1);
  Serial.print(" kg");
  Serial.print(" -> CF: ");
  Serial.println(CALIBRATION_FACTOR);

  if(Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    CALIBRATION_FACTOR = command.toInt();
  }

  delay(1000);
}
