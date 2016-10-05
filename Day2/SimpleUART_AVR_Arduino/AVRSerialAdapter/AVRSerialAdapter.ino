#define BAUD 9600
#define LED 13

char receivedChar;

void setup(){
  Serial.begin(BAUD);
  Serial1.begin(BAUD); //Connect AVR to these pair of RX TX
  Serial.println("Initialization");
  pinMode(LED, OUTPUT);
}

void loop(){
  if ((receivedChar = Serial1.read()) > 0){
    Serial.println(receivedChar);
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

