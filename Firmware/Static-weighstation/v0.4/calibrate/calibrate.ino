// To be compiled for Teensy with connected HX711 load cell amplifier

#include "HX711.h"

#define DOUT  30 // SDA
#define CLK  29 // SCL

HX711 scale;

int CALIBRATION_FACTOR = -1770;

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
}
