void setup() {
  Serial.begin(115200);
  pinMode(16, INPUT); // Setup for leads off detection LO +
  pinMode(17, INPUT); // Setup for leads off detection LO -
}
 
void loop() {
  
  if((digitalRead(16) == 1)||(digitalRead(17) == 1)){
    Serial.println('!'); //make graph draw nothing
  }
  else{
    // send the value of analog input 0:
      int adc = analogRead(34);
      Serial.println(adc);
  }
  //Wait for a bit to keep serial data from saturating
  delay(20);
}
