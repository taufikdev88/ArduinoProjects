#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Psx.h>

#define dataPin 15  //coklat==putih
#define cmndPin 10  //abang==abu-abu
#define attPin 12   //ijo==ijo
#define clockPin 14 //biru==kuning

#define mode1 26
#define mode2 27
#define mode3 28
#define mode4 29

#define pa 8
#define a1 6 //kanan
#define a2 7
#define pb 5
#define b1 4 //kiri
#define b2 9

#define us1e 32
#define us1t 33
#define us2e 34
#define us2t 35
#define us3e 36
#define us3t 37
#define us4e 38
#define us4t 39
#define us5e 40
#define us5t 41
#define us6e 42
#define us6t 43

///////////ARUS//////////
const int numReadings = 30;
float readings[numReadings];
int index = 0;
float total = 0;
float average = 0;
float currentValue = 0;
int arus = 0;
/////////////////////////
int kec = 40;
///////PID////////////
int Speed, Err, D_Err, Last_Err, Ster;
int kpe, kde;
int pwmka = 150;
int pwmki = 150;
int Sensor;
///////////////////////////

//-------------------- koding baru --------------------
#include "TimerThree.h"

//--- pin and var for MotorKiri
#define encKi 3
uint8_t pwmKi;                      // menyimpan pwm yang di dapat dari PID untuk dikeluarkan ke motor
float dErrKi, iErrKi;               // untuk menyimpan nilai delta error dan integral error
int rpmKiRef = 0;                   // menyimpan nilai rpm referensi yang diinginkan (dari program / dari user)
volatile int rpmKiAct;              // menyimpan nilai rpm sebenarnya yang didapat dari rumus pengolahan encoder
volatile unsigned int pulseKi;      // menyimpan nilai interrupt yang dibangkitkan dari encoder

//--- pin and var for MotorKanan
#define encKa 2
uint8_t pwmKa;
float dErrKa, iErrKa;
int rpmKaRef = 0;
volatile int rpmKaAct;
volatile unsigned int pulseKa;

//--- pid const
#define Kp 1
#define Ki 0.001
#define Kd 0.25

//--- PID void --- single void for multiple pid application
void PID(int *act, int ref,uint8_t *out, float *lastDError, float *lastIError){
  int pError = ref-*act;
  int iError = *lastIError;
  
  int pwm = (Kp*pError)+(Ki*iError)+(Kd*(pError - *lastDError));
  *lastIError += pError;
  
  *lastIError = constrain(*lastIError, -400, 400);

  pwm = map(pwm,-5000,5000,-25,25);  
  *out += pwm;
  *out = constrain(*out, 0, 255);
  
  *lastDError = pError;
}

//--- interrupt void
void interruptKi(){
  pulseKi++;
}

void interruptKa(){
  pulseKa++;
}

//--- read rpm void
#define pulsesperturn 360
unsigned long timeold = 0;
void readRPM(){
  rpmKiAct = (60 * 1000 / pulsesperturn )/ (millis() - timeold)* pulseKi;
  rpmKaAct = (60 * 1000 / pulsesperturn )/ (millis() - timeold)* pulseKa; 
  timeold = millis();
}

//--- handler untuk menjalankan fungsi motor dan pid
void handler(){
  noInterrupts();
  readRPM();

  Serial.println((String) "rpmKi: " + rpmKiAct + "\trpmKa: " + rpmKaAct);
  PID(&rpmKiAct, rpmKiRef, &pwmKi, &dErrKi, &iErrKi);
  PID(&rpmKaAct, rpmKaRef, &pwmKa, &dErrKa, &iErrKa);
  
  analogWrite(pa, pwmKa);
  analogWrite(pb, pwmKi);
  interrupts();
}

//--- get instruction from serial
String readInstruction(){
  if(Serial.available()){
    String command = Serial.readStringUntil('\n');

    if(command.indexOf("F") >= 0){  rpmKiRef=1000;  rpmKaRef=1000;  mKiForward();   mKaForward();   }
    if(command.indexOf("B") >= 0){  rpmKiRef=1000;  rpmKaRef=1000;  mKiBackward();  mKaBackward();  }
  }
}

//--- motor command and direction ---
void mKiForward(){
  digitalWrite(a1, 1);
  digitalWrite(a2, 0);
}

void mKiBackward(){
  digitalWrite(a1, 0);
  digitalWrite(a2, 1);
}

void mKaForward(){
  digitalWrite(b1, 1);
  digitalWrite(b2, 0);
}

void mKaBackward(){
  digitalWrite(b1, 0);
  digitalWrite(b2, 1);
}
//-------------------- batas koding baru --------------

int atas = 8, kanan = 4, kiri = 1, bawah = 2, X = 512, R1 = 4096;
int data_stick = 0;

int maximumRange = 150; //kebutuhan akan maksimal range
int minimumRange = 0;   //kebutuhan akan minimal range
long duration1, duration2, duration3, duration4, duration5, duration6;
long distance1, distance2, distance3, distance4, distance5, distance6; //waktu untuk kalkulasi jarak

char buff1[16], buff2[16], buff3[16], buff4[16], buff5[16], buff6[16], buff7[16];
//int ska=0,ski=0;
//int simpangan_ki=0;
int sen[8];
int jkiri = 0, jmirki = 0, jtengah = 0, jmirka = 0, jkanan = 0, hitung = 0, sensor = 0, selisih = 0;
int gap = 0;
int batasatas[16], batasbawah[16], tengah[16], x, s;
//kanandekat=125, kananjauh=315;
int usdekat = 20, usjauh = 70;
int i = 0, j = 0, k = 0;
//int usdekat=90, usjauh=150;

LiquidCrystal lcd(31, 30, 44, 45, 46, 47);
Psx Psx;
void setup()
{
//--------------------------- koding baru -----------------------------------
  pinMode(encKi, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encKi), interruptKi, RISING);

  pinMode(encKa, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encKa), interruptKa, RISING);

  Timer3.initialize(1000);
  Timer3.attachInterrupt(handler);
//------------------------- batas koding baru -------------------------------

  // put your setup code here, to run once:
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 0);
  Serial.begin(9600);

  pinMode(us1t, OUTPUT);
  pinMode(us2t, OUTPUT);
  pinMode(us3t, OUTPUT);
  pinMode(us4t, OUTPUT);
  pinMode(us5t, OUTPUT);
  pinMode(us6t, OUTPUT);

  pinMode(us1e, INPUT);
  pinMode(us2e, INPUT);
  pinMode(us3e, INPUT);
  pinMode(us4e, INPUT);
  pinMode(us5e, INPUT);
  pinMode(us6e, INPUT);

  pinMode(mode1, INPUT_PULLUP);
  pinMode(mode2, INPUT_PULLUP);
  pinMode(mode3, INPUT_PULLUP);
  pinMode(mode4, INPUT_PULLUP);
  pinMode(pa, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(pb, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  digitalWrite(pa, LOW);
  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
  digitalWrite(pb, LOW);
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
  kpe = 10;
  kde = 50;
  //konversi();
  lcd.begin(16, 2);
  lcd.print("BISMILLAH"); //iki gantien tulisan sembarang misal BISMILLAH!!
  delay(2000);
  lcd.clear();
}

void loop()
{
  // putyour main code here, to run repeatedly:
  // scan_jarak();
//  menu();
  //us6();
  //delay(50);
  //Scan1();
  readInstruction();
}

void RC()
{
  data_stick = Psx.read();
  Serial.println(data_stick);
  //delay(10);
  //sensor_arus();
  if (data_stick == 0)
  {
    lcd.setCursor(0, 0);
    berhenti();
    lcd.print("STOP     ");
    lcd.print("                ");
  }
  if (data_stick == atas)
  {
    lcd.setCursor(0, 0);
    maju();
    lcd.print("MAJU     ");
    lcd.print("                ");
  }
  if (data_stick == bawah)
  {
    lcd.setCursor(0, 0);
    mundur();
    lcd.print("MUNDUR   ");
    lcd.print("                ");
  }
  if (data_stick == kanan)
  {
    lcd.setCursor(0, 0);
    belka();
    lcd.print("BELKA    ");
    lcd.print("                ");
  }
  if (data_stick == kiri)
  {
    lcd.setCursor(0, 0);
    belki();
    lcd.print("BELKI    ");
    lcd.print("                ");
  }
  if (data_stick == atas + kanan)
  {
    lcd.setCursor(0, 0);
    serka();
    lcd.print("SERKA    ");
    lcd.print("                ");
  }
  if (data_stick == atas + kiri)
  {
    lcd.setCursor(0, 0);
    serki();
    lcd.print("SERKI    ");
    lcd.print("                ");
  }
}

void berhenti()
{
  digitalWrite(a1, 0);
  digitalWrite(a2, 0);
  digitalWrite(b1, 0);
  digitalWrite(b2, 0);
  analogWrite(pa, 0);
  analogWrite(pb, 0);
}

void majuscan(unsigned int ka, unsigned int ki)
{
  pwmka = ka;
  pwmki = ki;
  digitalWrite(a1, 1);
  digitalWrite(a2, 0);
  digitalWrite(b1, 1);
  digitalWrite(b2, 0);
  analogWrite(pa, pwmka);
  analogWrite(pb, pwmki);
}

void maju()
{
  digitalWrite(a1, 1);
  digitalWrite(a2, 0);
  digitalWrite(b1, 1);
  digitalWrite(b2, 0);
  analogWrite(pa, pwmka);
  analogWrite(pb, pwmki);
}

void mundur()
{
  digitalWrite(a1, 0);
  digitalWrite(a2, 1);
  digitalWrite(b1, 0);
  digitalWrite(b2, 1);
  analogWrite(pa, pwmka);
  analogWrite(pb, pwmki);
}

void belka()
{
  digitalWrite(a1, 0);
  digitalWrite(a2, 1);
  digitalWrite(b1, 1);
  digitalWrite(b2, 0);
  analogWrite(pa, pwmka);
  analogWrite(pb, pwmki);
}

void belki()
{
  digitalWrite(a1, 1);
  digitalWrite(a2, 0);
  digitalWrite(b1, 0);
  digitalWrite(b2, 1);
  analogWrite(pa, pwmka);
  analogWrite(pb, pwmki);
}

void serki()
{
  digitalWrite(a1, 1);
  digitalWrite(a2, 0);
  digitalWrite(b1, 0);
  digitalWrite(b2, 0);
  analogWrite(pa, pwmka);
  analogWrite(pb, pwmki);
}

void serka()
{
  digitalWrite(a1, 0);
  digitalWrite(a2, 0);
  digitalWrite(b1, 1);
  digitalWrite(b2, 0);
  analogWrite(pa, pwmka);
  analogWrite(pb, pwmki);
}

void sensor_arus()
{
  total = total - readings[index];
  readings[index] = analogRead(A3); //Raw data reading
                                    //Data processing:510-raw data from analogRead when the input is 0;
                                    // 5-5v; the first 0.04-0.04V/A(sensitivity); the second 0.04-offset val;
  readings[index] = (readings[index] - 512) * 5 / 1024 / 0.04 - 0.04;

  total = total + readings[index];
  index = index + 1;
  if (index >= numReadings)
    index = 0;
  average = total / numReadings; //Smoothing algorithm (http://www.arduino.cc/en/Tutorial/Smoothing)
  currentValue = average;
  lcd.setCursor(13, 1);
  lcd.print(currentValue);
  // Serial.println(currentValue);
  //delay(10);
}

void uskiri()
{
  digitalWrite(us1t, LOW);
  delayMicroseconds(2);
  digitalWrite(us1t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us1t, LOW);
  duration1 = pulseIn(us1e, HIGH);
  jkiri = duration1 / 58.2;
  if (jkiri >= maximumRange)
  {
    jkiri = 150;
  }
  //delay(50);
}

void usmirki()
{
  digitalWrite(us2t, LOW);
  delayMicroseconds(2);
  digitalWrite(us2t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us2t, LOW);
  duration2 = pulseIn(us2e, HIGH);
  jmirki = duration2 / 58.2;
  if (jmirki >= maximumRange)
  {
    jmirki = 150;
  }
  //delay(50);
}

void ustengah()
{
  //jtengah=0;
  digitalWrite(us3t, LOW);
  delayMicroseconds(2);
  digitalWrite(us3t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us3t, LOW);
  duration3 = pulseIn(us3e, HIGH);
  jtengah = duration3 / 58.2;
  if (jtengah >= maximumRange)
  {
    jtengah = 150;
  }
  else
    jtengah = jtengah;
  //delay(50);
}

void usmirka()
{
  //jmirka=0;
  digitalWrite(us4t, LOW);
  delayMicroseconds(2);
  digitalWrite(us4t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us4t, LOW);
  duration4 = pulseIn(us4e, HIGH);
  jmirka = duration4 / 58.2;
  if (jmirka >= maximumRange)
  {
    jmirka = 150;
  }
  else
    jmirka = jmirka;
  //delay(50);
}

void uskanan()
{
  //jkanan=0;
  digitalWrite(us5t, LOW);
  delayMicroseconds(2);
  digitalWrite(us5t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us5t, LOW);
  duration5 = pulseIn(us5e, HIGH);
  jkanan = duration5 / 58.2;
  if (jkanan >= maximumRange)
  {
    jkanan = 150;
  }
  else
    jkanan = jkanan;
  //delay(50);
}

void us6()
{
  digitalWrite(us6t, LOW);
  delayMicroseconds(2);
  digitalWrite(us6t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us6t, LOW);
  delayMicroseconds(2);
  duration6 = pulseIn(us6e, HIGH);
  distance6 = duration6 / 58.2;
  if (distance6 >= maximumRange)
  {
    distance6 = 150;
  }
  else
    distance6 = distance6;
  //    digitalWrite(us6t,HIGH);delay(15);digitalWrite(us6t,LOW);
  //    while(digitalRead(us6e)==0){}
  //    while(digitalRead(us6e)==1)
  //    {distance6++;if(distance6>10964){break;}}
  //    distance6=(distance6 * 0.629808565690441);
  //    PORTC.6=1; delay_us(15); PORTC.6=0;
  //    while(PINC.7==0){}
  //    while(PINC.7==1)
  //    { jkanan++; if(jkanan>10964){break;} }
  //    jkanan=(jkanan*0.629808565690441);

  //distance6=distance6;
  //  lcd.setCursor(0,0);
  //  lcd.print(distance6);
  //  lcd.print("      ");
  //  Serial.println(distance6);
  // delay(50);
}

// void scankanan(int kec)
// {
//   switch (sensor)
//   { //=========>mendekat dinding kanan
//   case 0b00000000:  maju(kec - 80, kec);  s = 1;  break; //0
//   case 0b00000001:  maju(kec - 50, kec);  s = 1;  break; //1
//   case 0b00000011:  maju(kec - 30, kec);  s = 1;  break; //2
//   case 0b00000111:  maju(kec, kec); break; //3
//   case 0b00001111:  maju(kec, kec - 20);  s = 0;  break; //4
//   case 0b00011111:  maju(kec, kec - 30);  s = 0;  break; //5
//   case 0b00111111:  maju(kec, kec - 50);  s = 0;  break; //6
//   case 0b01111111:  maju(kec, kec - 80);  s = 0;  break; //7
//   }
// }

void Scan1() //kiri
{
  int mka, mki;
  Speed = 40;
  bacasensorkanan();
  //usmirka();
  //sensor_arus();

  Sensor = sensor;
  switch (Sensor)
  {
    //                case 0b11111110: Err=21;           break;
    //                case 0b11111101: Err=18;           break;
    //                case 0b11111001: Err=15;           break;
    //                case 0b11111011: Err=12;           break;
    //                case 0b11110001: Err=9;            break;
    //                case 0b11110011: Err=5;           break;
    //                case 0b11110111: Err=1;           break;
    //                case 0b11100011: Err=1;           break;
    //                case 0b11100111: Err=0;           break;
    //                case 0b11000011: Err=0;           break;
    //                case 0b11000111: Err=-1;          break;
    //                case 0b11101111: Err=-1;          break;
    //                case 0b11001111: Err=-5;          break;
    //                case 0b10001111: Err=-9;          break;
    //                case 0b11011111: Err=-12;          break;
    //                case 0b10011111: Err=-15;          break;
    //                case 0b10111111: Err=-18;          break;
    //                case 0b01111111: Err=21;          break;
    // case 0b11111111: Err=0;          break;
    // case 0b111111: Err=-20;          break;
    //                  case 0b0000000000000000:    Err=20;             break;  //16
    //                  case 0b0000000000000001:    Err=16;             break;  //16
    //                  case 0b0000000000000011:    Err=12;             break;  //15
    //                  case 0b0000000000000111:    Err=8;             break;  //14
    //                  case 0b0000000000001111:    Err=6;             break;  //13
    //                  case 0b0000000000011111:    Err=4;              break;  //12
    //                  case 0b0000000000111111:    Err=2;              break;  //11
    //                  case 0b0000000001111111:    Err=0;              break;  //10
    //                  case 0b0000000011111111:    Err=0;              break;  //9
    //                  case 0b0000000111111111:    Err=-2;              break;  //8
    //                  case 0b0000001111111111:    Err=-4;             break;  //7
    //                  case 0b0000011111111111:    Err=-6;             break;  //6
    //                  case 0b0000111111111111:    Err=-8;             break;  //5
    //                  case 0b0001111111111111:    Err=-12;             break;  //4
    //                  case 0b0011111111111111:    Err=-16;            break;  //3
    //                  case 0b0111111111111111:    Err=-20;            break;  //2
    //                  case 0b1111111111111111:    Err=-25;            break;  //1

  case 0b0000000000:  Err = 15; break; //0
  case 0b0000000001:  Err = 5;  break; //1
  case 0b0000000011:  Err = 3;  break; //2
  case 0b0000000111:  Err = 2;  break; //3
  case 0b0000001111:  Err = 0;  break; //4
  case 0b0000011111:  Err = 0;  break; //5
  case 0b0000111111:  Err = -2; break; //6
  case 0b0001111111:  Err = -3; break; //7
  case 0b0011111111:  Err = -5; break; //7
  case 0b0111111111:  Err = -7; break; //7
  case 0b1111111111:  Err = -15; break; //7
    //
  }

  //     interupsi

  D_Err = Err - Last_Err;
  Ster = Err * kpe + D_Err * kde;

  mka = Speed - Ster;
  mki = Speed + Ster;

  if (mka < 0)
  {
    mka = mka * (-1);
    if (mka >= 30)
      mka = 40;
    digitalWrite(b1, 0);
    digitalWrite(b2, 1);
    analogWrite(pb, mka);
    lcd.setCursor(0, 1);
    lcd.print("mdr=");
    lcd.setCursor(4, 1);
    lcd.print(mka);
    lcd.print("   ");
  }

  else
  {
    if (mka >= Speed)
      mka = Speed;
    digitalWrite(b1, 1);
    digitalWrite(b2, 0);
    analogWrite(pb, mka);
    lcd.setCursor(0, 1);
    lcd.print("mju=");
    lcd.setCursor(4, 1);
    lcd.print(mka);
    lcd.print("   ");
  }

  if (mki < 0)
  {
    mki = mki * (-1);
    if (mki >= 30)
      mki = 40;
    digitalWrite(a1, 0);
    digitalWrite(a2, 1);
    analogWrite(pa, mki);
    lcd.setCursor(8, 1);
    lcd.print("mdr=");
    lcd.setCursor(12, 1);
    lcd.print(mki);
    lcd.print("   ");
  }
  else
  {
    if (mki >= Speed)
      mki = Speed;
    digitalWrite(a1, 1);
    digitalWrite(a2, 0);
    analogWrite(pa, mki);
    lcd.setCursor(8, 1);
    lcd.print("mju=");
    lcd.setCursor(12, 1);
    lcd.print(mki);
    lcd.print("   ");
  }

  Last_Err = Err;
  delay(100);
}

void konversi()
{
  selisih = usjauh - usdekat;
  gap = selisih / 24;
  batasbawah[0] = usdekat;
  for (x = 1; x <= 10; x++)
  {
    batasbawah[x] = batasbawah[x - 1] + gap;
  }
  for (x = 0; x <= 10; x++)
  {
    batasatas[x] = batasbawah[x] + 5;
  }
}
void bacasensorkanan()
{
  //us_all();
  konversi();
  uskanan();
  //ustengah();
  //delay(50);
  for (x = 0; x <= 10; x++)
  {
    if (jkanan >= batasbawah[x])
    {
      sen[x] = 1;
    }
    else if (jkanan <= batasbawah[x])
    {
      sen[x] = 0;
    }
  }

  sensor = (sen[10] * 1024) + (sen[9] * 512) + (sen[8] * 256) + (sen[7] * 128) + (sen[6] * 64) + (sen[5] * 32) + (sen[4] * 16) + (sen[3] * 8) + (sen[2] * 4) + (sen[1] * 2) + (sen[0] * 1);
  lcd.setCursor(0, 0);
  sprintf(buff1, "%d%d%d%d%d%d%d%d%d%d", sen[10], sen[9], sen[8], sen[7], sen[6], sen[5], sen[4], sen[3], sen[2], sen[1], sen[0]);
  lcd.print(buff1);
  //       lcd.setCursor(0,0);lcd.print(sen[7]);
  //       lcd.setCursor(1,0);lcd.print(sen[6]);
  //       lcd.setCursor(2,0);lcd.print(sen[5]);
  //       lcd.setCursor(3,0);lcd.print(sen[4]);
  //       lcd.setCursor(4,0);lcd.print(sen[3]);
  //       lcd.setCursor(5,0);lcd.print(sen[2]);
  //       lcd.setCursor(6,0);lcd.print(sen[1]);
  //       lcd.setCursor(7,0);lcd.print(sen[0]);
  //lcd.setCursor(0,1);  sprintf(buff1,"%3d  %3d  %3d",jtengah,jmirka,jkanan); lcd.print(buff1);
  //lcd.setCursor(14,0); sprintf(buff6,"%d",i); lcd.print(buff6);
  //lcd.setCursor(14,1); sprintf(buff7,"%d",j); lcd.print(buff7);
}

void Scan2(int kec)
{
  bacasensorkanan();
  switch (sensor)
  { //=========>mendekat dinding kanan
  case 0b0000000000:  majuscan(kec, kec - 40);  s = 1;  break; //0
  case 0b0000000001:  majuscan(kec, kec - 15);  s = 1;  break; //1
  case 0b0000000011:  majuscan(kec, kec - 10);  s = 1;  break; //2
  case 0b0000000111:  majuscan(kec, kec - 17);  break; //3
  case 0b0000001111:  majuscan(kec, kec - 2);   s = 0;  break; //4
  case 0b0000011111:  majuscan(kec, kec);       s = 0;  break; //5
  case 0b0000111111:  majuscan(kec - 2, kec);   s = 0;  break; //6
  case 0b0001111111:  majuscan(kec - 7, kec);   s = 0;  break; //7
  case 0b0011111111:  majuscan(kec - 10, kec);  s = 0;  break; //7
  case 0b0111111111:  majuscan(kec - 15, kec);  s = 0;  break; //7
  case 0b1111111111:  majuscan(kec - 40, kec);  s = 0;  break; //7
  }
  lcd.setCursor(0, 1);
  lcd.print(pwmka);
  lcd.print("   ");
  lcd.setCursor(8, 1);
  lcd.print(pwmki);
  lcd.print("   ");
  delay(50);
}

void scan_jarak()
{
  unsigned int i = 0;
  while (digitalRead(mode1) == 1)
  {
    //uskiri();usmirki();
    ustengah();
    usmirka();
    uskanan();
    //us6();
    //    lcd.setCursor(0,0);
    //    lcd.print(jkiri);
    //    lcd.print("   ");
    //    lcd.setCursor(4,0);
    //    lcd.print(jmirki);
    //    lcd.print("   ");
    //    lcd.setCursor(8,0);
    //    lcd.print(jtengah);
    //    lcd.print("   ");
    //    lcd.setCursor(12,0);
    //    lcd.print(jmirka);
    //    lcd.print("   ");
    //    lcd.setCursor(5,1);
    //    lcd.print(jkanan);
    //    lcd.print("   ");
    //    lcd.setCursor(9,1);
    //    lcd.print(distance6);
    //    lcd.print("   ");
    //Serial.println(distance6);

    bacasensorkanan();
    //delay(50);
  }

  for (i = 0; i <= 200; i++)
    while (digitalRead(mode1) == 0)
    {
    };
  lcd.clear();
  //delay(100);
  lcd.setCursor(0, 0);
  lcd.print("Selesai");
  delay(500);
  lcd.clear();
}

void menu()
{
  unsigned char i = 0;
menu_utama:
  //berhenti();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("pilih mode:");
  //lcd.setCursor(14,0);
  //lcd.print(arus);
  //lcd.setCursor(15,0);
  //lcd.print("A");
  if (digitalRead(mode1) == 0)
    while (digitalRead(mode1) == 0)
      ;
  while (1)
  {
    if (digitalRead(mode2) == 0)
    {
      delay(100);
      if (digitalRead(mode2) == 0)
        while (digitalRead(mode2) == 0)
        {
        }
      if (++i > 6)
        i = 0;
    }

    if (digitalRead(mode3) == 0)
    {
      delay(100);
      if (digitalRead(mode3) == 0)
        while (digitalRead(mode3) == 0)
        {
        }
      if (--i == 255)
        i = 6;
    }

    switch (i)
    {
    case 0:
      lcd.setCursor(0, 1);
      lcd.print("kalibrasi      ");
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("RC             ");
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.print("MODE auto 1    ");
      break;
    case 3:
      lcd.setCursor(0, 1);
      lcd.print("MODE auto 2    ");
      break;
    case 4:
      lcd.setCursor(0, 1);
      lcd.print("Setting Arus   ");
      break;
    case 5:
      lcd.setCursor(0, 1);
      lcd.print("RC DRONE       ");
      break;
    case 6:
      lcd.setCursor(0, 1);
      lcd.print("Auto Ka        ");
      break;
    }
    if (digitalRead(mode1) == 0)
    {
      delay(100);
      if (digitalRead(mode1) == 0)
        while (digitalRead(mode1) == 0)
          ;
      delay(100);
      lcd.clear();
      if (i == 0)
      {
        // kalibrasi();
        scan_jarak();
        //RC();
        //delay(50);
        lcd.clear();
        i = 0;
        goto menu_utama;
      }

      if (i == 1)
      {
        delay(300);
        lcd.setCursor(0, 1);
        lcd.print("Mode RC");
        delay(500);
        lcd.clear();
        Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 0);
        while (digitalRead(mode1) == 1)
        {
          ///////////////////////TULIS PROGRAM DIBAWAH SINI///////////////////////////////
          RC();
          sensor_arus();
          //scan_jarak();

          ////////////////////////////////SELESAI/////////////////////////////////////////
        }

        for (i = 0; i <= 200; i++)
          while (digitalRead(mode1) == 0)
          {
          }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" RC stop ");
        delay(1000);
        lcd.clear();
        i = 1;
        goto menu_utama;
      }
      if (i == 2)
      {
        delay(300);
        lcd.setCursor(0, 1);
        lcd.print("Mode Auto 1");
        delay(500);
        lcd.clear();
        while (digitalRead(mode1) == 1)
        {
          ustengah();
          //usmirka();
          if (jtengah <= 30)
          {
            belki();
            //lcd.setCursor(14,0);
            //lcd.print("1");
          }

          else if (jtengah >= 30)
          {
            Scan1();
            //lcd.setCursor(14,0);
            //lcd.print("0");
          }
        }

        for (i = 0; i <= 200; i++)
          while (digitalRead(mode1) == 0)
          {
          }
        berhenti();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Auto 1 STOP");
        delay(1000);
        lcd.clear();
        i = 2;
        goto menu_utama;
      }
      if (i == 3)
      {
        delay(300);
        lcd.setCursor(0, 1);
        lcd.print("Mode Auto 2");
        lcd.clear();
        //baca_sensor();

        while (digitalRead(mode1) == 1)
        {
          ustengah();
          lcd.setCursor(14, 0);
          lcd.print("0");
          Scan2(kec);
          //usmirka();
          while (jtengah <= 30)
          {
            ustengah();
            belki();
            lcd.setCursor(14, 0);
            lcd.print("1");
            if (jtengah > 30)
            {
              break;
            }
          }
        }

        for (i = 0; i <= 200; i++)
          while (digitalRead(mode1) == 0)
          {
          }
        berhenti();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Auto 2 STOP");
        delay(1000);
        lcd.clear();
        i = 3;
        goto menu_utama;
      }
      if (i == 4)
      {
        delay(300);
        lcd.setCursor(0, 0);
        lcd.print("Setting Arus");
        while (digitalRead(mode1) == 1)
        {
          ;
          if (digitalRead(mode3) == 0)
          {
            delay(100);
            arus += 1;
            if (arus >= 255)
              arus = 0;
          }
          if (digitalRead(mode2) == 0)
          {
            delay(100);
            arus -= 1;
            if (arus <= 0)
              arus = 255;
          }

          lcd.setCursor(0, 1);
          lcd.print(arus);
          EEPROM.write(5, arus);
        }

        for (i = 0; i <= 200; i++)
          while (digitalRead(mode1) == 0)
          {
          }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Setiing Arus Done");
        delay(1000);
        lcd.clear();
        i = 4;
        goto menu_utama;
      }
      if (i == 5)
      {
        delay(300);
        lcd.setCursor(0, 1);
        lcd.print("RC Drone");
        while (digitalRead(mode1) == 1)
        {
          lcd.print(" kosong4");
          //RC_drone();
        }

        for (i = 0; i <= 200; i++)
          while (digitalRead(mode1) == 0)
          {
          }
        //berhenti();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("kosong4 Stop");
        delay(1000);
        lcd.clear();
        i = 5;
        goto menu_utama;
      }
      if (i == 6)
      {
        delay(300);
        lcd.setCursor(0, 1);
        lcd.print("Mode semi Auto Ka");
        lcd.clear();
      }
      while (digitalRead(mode1) == 1)
      {
        lcd.setCursor(0, 0);
        lcd.print(" kosong5 ");
        //Scan1();
      }

      for (i = 0; i <= 200; i++)
        while (digitalRead(mode1) == 0)
        {
        }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("kosong5 Stop");
      delay(1000);
      lcd.clear();
      i = 6;
      goto menu_utama;
    }
  }
}
