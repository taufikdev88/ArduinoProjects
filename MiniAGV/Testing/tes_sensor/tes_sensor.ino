uint8_t pinsen[8] = { A0,A1,A2,A3,A4,A5,A6,A7 };
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<=7; i++){
    Serial.println((String) i + ". " + analogRead(pinsen[i]));
  }
  delay(1000);
}
