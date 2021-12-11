#include <Psx.h>
#include <TimerOne.h>


int rpmKiRef = 0, rpmKaRef = 0;
float dErrorKi, iErrorKi, pErrorKi;
float dErrorKa, iErrorKa, pErrorKa;

float KpKiS =0.03, KiKiS = 0.015, KdKiS = 0.05; // ini buat slowrespon
float KpKaS =0.03, KiKaS = 0.015, KdKaS = 0.05;   
float KpKiF =0.2, KiKiF = 0.04, KdKiF = 0.07; // ini buat fastrespon
float KpKaF =0.2, KiKaF = 0.04, KdKaF = 0.07;   

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

#define data_stickPin 12//abu2//MISO//PB_4
#define cmndPin 8//ungu//pwm//MOSI//PB_5
#define attPin 10//biru//pwm//SS//PA_15
#define clockPin 13//ijo//pwm//SCK//PB_3

#define pwm1 5
#define pwm2 6
#define pwm3 9
#define pwm4 11

uint8_t kec = 0;
#define fst 150
#define sfst 255
#define slw 100

/*
 * offset untuk menyeimbangkan kecepatan motor kanan kiri, offset akan mengurangi dari pwm referensi
 */
#define rpmOffsetKa 5
#define rpmOffsetKi 5

Psx Psx;
int data_stick = 0;
int bunder =1024,atas=8,kanan=4,kiri=1,bawah=2,X=512,segitiga=2048,R1=4096,kotak=256;
const int R2 = 16384;
const int L2 = -32768;
int j=150;

void setup() {
  Psx.setupPins(data_stickPin, cmndPin, attPin, clockPin, 0);
  Serial.begin(115200);
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(pwm3,OUTPUT);
  pinMode(pwm4,OUTPUT);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), interruptKi, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), interruptKa, CHANGE);

  Timer1.initialize(100000);
  Timer1.attachInterrupt(readRpm);
}

void loop() {
  data_stick=Psx.read(); 
//  Serial.println(data_stick);
  delay(10);
  if(data_stick==0)                   {kec=slw;berhenti();}
  if(data_stick==X)                   {kec=fst;berhenti();}
  if(data_stick==atas)                {kec=slw;maju();}
  if(data_stick==bawah)               {kec=slw;mundur();}
  if(data_stick==kanan)               {kec=slw;belka();}
  if(data_stick==kiri)                {kec=slw;belki();}
  if(data_stick==kotak)               {kec=fst;putka();}
  if(data_stick==bunder)              {kec=fst;putki();}
  if(data_stick==kotak+R1)            {kec=sfst;putka();}
  if(data_stick==bunder+R1)           {kec=sfst;putki();}
  if(data_stick==atas+kiri)           {kec=slw;serki();}
  if(data_stick==atas+kanan)          {kec=slw;serka();}
  //if(data_stick==bawah+kiri)        {kec=slw;}
  //if(data_stick==bawah+kanan)       {kec=slw;}
  
  
  if(data_stick==atas+X)              {kec=fst;maju();}
  if(data_stick==bawah+X)             {kec=fst;mundur();}
  if(data_stick==kanan+X)             {kec=fst;belka();}
  if(data_stick==kiri+X)              {kec=fst;belki();}
  if(data_stick==atas+kiri+X)         {kec=fst;serki();}
  if(data_stick==atas+kanan+X)        {kec=fst;serka();}
  
  if(data_stick==0+R1)                {kec=sfst;berhenti();}
  if(data_stick==X+R1)                {kec=sfst;berhenti();}
  if(data_stick==atas+R1)             {kec=sfst;maju();}
  if(data_stick==bawah+R1)            {kec=sfst;mundur();}
  if(data_stick==kanan+R1)            {kec=sfst;belka();}
  if(data_stick==kiri+R1)             {kec=sfst;belki();}
  if(data_stick==atas+kiri+R1)        {kec=sfst;serki();}
  if(data_stick==atas+kanan+R1)       {kec=sfst;serka();}
  
  if(data_stick==R2)                  {kec=sfst;putkaback();}
  if(data_stick==R2+X)                {kec=sfst;putkaback();}
  if(data_stick==R2+R1)               {kec=sfst;putkaback();}
  
  if (data_stick==L2)                 {putkiback();}
  if (data_stick==L2+X)               {putkiback();}
  if (data_stick==L2+R1)              {putkiback();}
  
  if(data_stick==R2+R1+X)             {kec=sfst;putka();delay(j);berhenti();}
  //if(data_stick==bawah+kiri)          {kec=slw;}
  //if(data_stick==bawah+kanan)         {kec=slw;}
  
  
  if(data_stick==atas+X+R1)           {kec=sfst;maju();}
  if(data_stick==bawah+X+R1)          {kec=sfst;mundur();}
  if(data_stick==kanan+X+R1)          {kec=sfst;belka();}
  if(data_stick==kiri+X+R1)           {kec=sfst;belki();}
  if(data_stick==atas+kiri+X+R1)      {kec=sfst;serki();}
  if(data_stick==atas+kanan+X+R1)     {kec=sfst;serka();}

  if(data_stick==atas+bunder)         {kec=slw;putki();}
  if(data_stick==atas+bunder+X)       {kec=slw;putki();}
  if(data_stick==atas+bunder+R1)      {kec=slw;putki();}

  if(data_stick==atas+kotak)          {kec=slw;putka();}
  if(data_stick==atas+kotak+X)        {kec=slw;putka();}
  if(data_stick==atas+kotak+R1)       {kec=slw;putka();}
  
}

void berhenti(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,0);
  analogWrite(pwm3,0);
  analogWrite(pwm4,0);
}
void maju(){
  uint8_t pwmka = PID(kec, rpmKaAct, KpKaF, KiKaF, KdKaF, &iErrorKa, &dErrorKa, 0, 255) - rpmOffsetKa;
  uint8_t pwmki = PID(kec, rpmKiAct, KpKiF, KiKiF, KdKiF, &iErrorKi, &dErrorKi, 0, 255) - rpmOffsetKi;

  analogWrite(pwm1,pwmka);
  analogWrite(pwm3,pwmki);
  analogWrite(pwm2,0);
  analogWrite(pwm4,0);
}
void mundur(){
  uint8_t pwmka = PID(kec, rpmKaAct, KpKaF, KiKaF, KdKaF, &iErrorKa, &dErrorKa, 0, 255) - rpmOffsetKa;
  uint8_t pwmki = PID(kec, rpmKiAct, KpKiF, KiKiF, KdKiF, &iErrorKi, &dErrorKi, 0, 255) - rpmOffsetKi;

  analogWrite(pwm1,0);
  analogWrite(pwm2,pwmka);
  analogWrite(pwm3,0);
  analogWrite(pwm4,pwmki);
}
void belka(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,50);
  analogWrite(pwm3,kec);
  analogWrite(pwm4,0);
}
void belki(){
  analogWrite(pwm1,kec);
  analogWrite(pwm2,0);
  analogWrite(pwm3,0);
  analogWrite(pwm4,50);
}
void serka(){
  analogWrite(pwm1,30);
  analogWrite(pwm2,0);
  analogWrite(pwm3,kec);
  analogWrite(pwm4,0);
}
void serki(){
  analogWrite(pwm1,kec);
  analogWrite(pwm2,0);
  analogWrite(pwm3,30);
  analogWrite(pwm4,0);
}
void putki(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,kec);
  analogWrite(pwm3,kec);
  analogWrite(pwm4,0);
}
void putka(){
  analogWrite(pwm1,kec);
  analogWrite(pwm2,0);
  analogWrite(pwm3,0);
  analogWrite(pwm4,kec);
}
void putkiback(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,255);
  analogWrite(pwm3,0);
  analogWrite(pwm4,50);
}
void putkaback(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,50);
  analogWrite(pwm3,0);
  analogWrite(pwm4,255);
}
