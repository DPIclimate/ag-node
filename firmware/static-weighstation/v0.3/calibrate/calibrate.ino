// To be compiled on ATmega328 wiht connected HX711 load cell amplifier

#include "HX711.h"

#define DOUT  2 // SDA
#define CLK  3 // SCL

HX711 scale;

int CALIBRATION_FACTOR = -1760;

void setup() {
  Serial.begin(57600);

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
