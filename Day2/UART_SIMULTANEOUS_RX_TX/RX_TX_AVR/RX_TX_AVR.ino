#define BAUD 9600

void setup(){
  Serial.begin(BAUD);
  Serial1.begin(BAUD); //Connect AVR to these pair of RX TX
  Serial.println("Initialization");
}

void loop(){
  Serial1.print('a');
  delay(500);
}


void serialEvent1(){
  String b;
  while (Serial1.available() > 0){
    b = Serial1.readStringUntil('\n');
    Serial.println(b);
  }
}

