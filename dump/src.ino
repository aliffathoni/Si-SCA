#include "config.h"

SiSCA sisca("sisca", 2, 16, 33, 15);

//Add task scheduler and rtos to improve speed ok!

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sisca.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // sisca.startSystem();
  Serial.println(sisca.getHR());
  delay(500);
}
