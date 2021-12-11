#define use 18
#define ust 44
void usSetup(){
  pinMode(ust, OUTPUT);
  pinMode(use, INPUT);
}
#define maxRange 150
#define usTimeout 200000
int usDist(){
  digitalWrite(ust, LOW);
  delayMicroseconds(2);
  digitalWrite(ust, HIGH);
  delayMicroseconds(10);
  digitalWrite(ust, LOW);
  int jarak = pulseIn(use, HIGH, usTimeout) / 29 / 2;
  if(jarak >= maxRange) return maxRange;
  return jarak;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  usSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(usDist());
  delay(500);
}
