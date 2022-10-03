#include "config.h"

SiSCA sisca;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sisca.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  sisca.startSystem();
}
