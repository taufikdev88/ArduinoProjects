#define mKa1 12
#define mKa2 14
#define mKi1 13
#define mKi2 15
const uint8_t pwmKa = 6,
pwmKi = 7;

uint8_t basePwm = 0;
void motorSetup(){
  pinMode(mKa1, OUTPUT);
  pinMode(mKa2, OUTPUT);
  pinMode(mKi1, OUTPUT);
  pinMode(mKi2, OUTPUT);
  pinMode(pwmKa, OUTPUT);
  pinMode(pwmKi, OUTPUT);
}
void mKaForward(){
  digitalWrite(mKa1, HIGH);
  digitalWrite(mKa2, LOW);
}
void mKaBackward(){
  digitalWrite(mKa1, LOW);
  digitalWrite(mKa2, HIGH);
}
void mKaStop(){
  digitalWrite(mKa1, LOW);
  digitalWrite(mKa2, LOW);
}
void mKiForward(){
  digitalWrite(mKi1, HIGH);
  digitalWrite(mKi2, LOW);
}
void mKiBackward(){
  digitalWrite(mKi1, LOW);
  digitalWrite(mKi2, HIGH);
}
void mKiStop(){
  digitalWrite(mKi1, LOW);
  digitalWrite(mKi2, LOW);
}
void mLiftUp(){
  
}
void mLiftDown(){
  
}
void mLiftStop(){
  
}

void setup(){
  motorSetup();
for(int i=2; i<=10; i++){
  pinMode(i, OUTPUT);
  analogWrite(i, 150);
}
  analogWrite(pwmKa, 0);
  analogWrite(pwmKi, 0);
  analogWrite(pwmKa, 55);
  analogWrite(pwmKi, 55);
}

void loop(){
  mKaForward();
  mKiForward();
  analogWrite(pwmKa, 85);
  analogWrite(pwmKi, 85);
  delay(3000);
//  mKaStop();
//  mKiStop();
//  delay(1000);
//  mKiBackward();
//  delay(3000);
//  mKaBackward();
//  delay(3000);
//  mKaStop();
//  mKiStop();
//  while(1);
}
