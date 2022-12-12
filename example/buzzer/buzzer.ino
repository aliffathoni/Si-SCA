void setup(){
  // configure LED PWM functionalitites
  ledcSetup(0, 10000, 12);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(14, 0);
}
 
void loop(){
  ledcWriteTone(0, 4096);
  delay(100);
  ledcWriteTone(0, 0);   
  delay(100);
  ledcWriteTone(0, 4096);
  delay(100);
  ledcWriteTone(0, 0);
  delay(100);
  ledcWriteTone(0, 4096);
  delay(100);
  ledcWriteTone(0, 0);   
  delay(2500);
}