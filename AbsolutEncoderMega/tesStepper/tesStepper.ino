#define stepPin 7
#define dirPin 9

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(stepPin, 0);
  digitalWrite(dirPin, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(F("kiri"));
  digitalWrite(dirPin, 0);
  for(int i=0; i<8000; i++){
    digitalWrite(stepPin, 1);
    delayMicroseconds(100);
    digitalWrite(stepPin, 0);
    delayMicroseconds(100);
  }
  delay(1000);
  Serial.println(F("kanan"));
  digitalWrite(dirPin, 1);
  for(int i=0; i<8000; i++){
    digitalWrite(stepPin, 1);
    delayMicroseconds(100);
    digitalWrite(stepPin, 0);
    delayMicroseconds(100);
  }
  delay(1000);
}
