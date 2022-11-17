#define LED_BUILTIN 2

int adc;
int UpperThreshold = 1500;
int LowerThreshold = 800;
int BPM = 0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;
unsigned long lastScan = 0;
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(16, INPUT); // Setup for leads off detection LO +
  pinMode(17, INPUT); // Setup for leads off detection LO -
}
 
void loop() {
  if(millis() - lastScan >= 20){
    lastScan = millis();
    adc = analogRead(15); 
    // Heart beat leading edge detected.
    if(adc > UpperThreshold && IgnoreReading == false){
      if(FirstPulseDetected == false){
        FirstPulseTime = millis();
        FirstPulseDetected = true;
      }
      else{
        SecondPulseTime = millis();
        PulseInterval = SecondPulseTime - FirstPulseTime;
        FirstPulseTime = SecondPulseTime;
      }
      IgnoreReading = true;
      digitalWrite(LED_BUILTIN, HIGH);
    }

    // Heart beat trailing edge detected.
    if(adc < LowerThreshold && IgnoreReading == true){
      IgnoreReading = false;
      digitalWrite(LED_BUILTIN, LOW);
    }  

    // Calculate Beats Per Minute.
    BPM = (1.0/PulseInterval) * 60.0 * 1000;
  }

  if(millis() - lastPrint > 200){
    lastPrint = millis();
    Serial.print(adc);
    Serial.print("\t");
    Serial.print(PulseInterval);
    Serial.print("\t");
    Serial.print(BPM);
    Serial.println(" BPM");
  }
}
