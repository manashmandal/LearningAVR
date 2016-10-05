#define BAUD 9600

void setup(){
  Serial.begin(BAUD);
  Serial1.begin(BAUD); //Connect AVR to these pair of RX TX
  Serial.println("Initialization");
}

void loop(){
  if (Serial1.available() > 0){
    Serial.println(Serial1.readStringUntil('\n'));
  }
}

