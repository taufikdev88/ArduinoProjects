#include <TimerOne.h>

volatile int encoderKi, encoderKa;

unsigned long lastIntKi = 0;
void interruptKi(){
  if((unsigned long) millis() - lastIntKi > 1){
    encoderKi++;
    lastIntKi = millis();
  }
}

unsigned long lastIntKa = 0;
void interruptKa(){
  if((unsigned long) millis() - lastIntKa > 1){
    encoderKa++;
    lastIntKa = millis();
  }
}

int rpmKiAct, rpmKaAct;
void readRpm(){
  noInterrupts();
  rpmKiAct = (encoderKi/72.0) * 1000.0 / 100 * 60.0;
  rpmKaAct = (encoderKa/72.0) * 1000.0 / 100 * 60.0;
  encoderKi = 0;
  encoderKa = 0;
  interrupts();
}

#define eb 16
#define b1 4 //kiri
#define b2 6
//--- pin and var for MotorKanan
#define ea 9
#define a1 7 //kanan
#define a2 8

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(1, interruptKi, CHANGE);
  attachInterrupt(0, interruptKa, CHANGE);
  
  Timer1.initialize(100000);
  Timer1.attachInterrupt(readRpm);

  pinMode(eb, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);

  digitalWrite(eb, HIGH);
  analogWrite(b1,0);
  analogWrite(b2,0);
  
  pinMode(ea, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);

  digitalWrite(ea, HIGH);
  analogWrite(a1,0);
  analogWrite(a2,0);

  Serial.begin(9600);
}

int rpmKiRef = 0, rpmKaRef = 0;
float dErrorKi, iErrorKi, pErrorKi;
float dErrorKa, iErrorKa, pErrorKa;

float KpKi =0.03, KiKi = 0.015, KdKi = 0.05; // ini buat slowrespon
float KpKa =0.2, KiKa = 0.04, KdKa = 0.07;   // fast respon

int globalpwm = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String str = Serial.readStringUntil('\n');

    if(str.indexOf('s') >= 0) rpmKiRef = str.substring(1).toFloat();
    if(str.indexOf("pka") >= 0) KpKa = str.substring(3).toFloat();
    if(str.indexOf("ika") >= 0) KiKa = str.substring(3).toFloat();
    if(str.indexOf("dka") >= 0) KdKa = str.substring(3).toFloat();
    if(str.indexOf("pki") >= 0) KpKi = str.substring(3).toFloat();
    if(str.indexOf("iki") >= 0) KiKi = str.substring(3).toFloat();
    if(str.indexOf("dki") >= 0) KdKi = str.substring(3).toFloat();

    rpmKaRef = rpmKiRef;
  }
  
  Serial.print(rpmKaRef);
  Serial.print('\t');
  Serial.print(rpmKaAct);
  Serial.print('\t');
  Serial.print(rpmKiRef);
  Serial.print('\t');
  Serial.print(rpmKiAct);
  int pwm = PID(rpmKaRef, rpmKaAct, KpKa, KiKa, KdKa, &iErrorKa, &dErrorKa,0,100);
  Serial.print('\t');
  Serial.println(pwm);
  analogWrite(a1, pwm);
  analogWrite(b1, PID(rpmKiRef, rpmKiAct, KpKi, KiKi, KdKi, &iErrorKi, &dErrorKi,0,255));
}

int PID(int rpmRef, int rpmAct, float Kp, float Ki, float Kd, float *iError, float *dError,int outmin, int outmax){
  float _pError = rpmRef-rpmAct;
  float _iError = *iError;
  float _dError = *dError;
  
  int pwm = _pError*Kp + _iError*Ki + (_pError-_dError)*Kd;

  *dError = _pError;
  _iError += _pError;
  *iError = constrain(_iError, -5000, 5000);

  return constrain(pwm, outmin, outmax);  
}
