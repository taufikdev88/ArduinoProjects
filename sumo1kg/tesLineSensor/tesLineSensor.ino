#define rfLine A0
#define lfLine A1
#define cbLine A2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(rfLine, INPUT);
  pinMode(lfLine, INPUT);
  pinMode(cbLine, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(rfLine));
  Serial.print('\t');
  Serial.print(analogRead(lfLine));
  Serial.print('\t');
  Serial.print(analogRead(cbLine));
  Serial.println();
}
