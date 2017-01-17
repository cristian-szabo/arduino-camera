#include "ov7670.hpp"

OV7670 camera;

void setup() {
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);

  camera.begin();
}

void loop() {

}
