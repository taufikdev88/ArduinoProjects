#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <EEPROM.h>

#define robot1
//#define robot2

//======================================================================================== SENSOR
#define senL 2
#define senR 3
#define isSenR() digitalRead(senR)
#define isSenL() digitalRead(senL)

//============== Filtering Left and Right Sensor =================
//bool isSenR(void){
//  int senVal=0;
//  for(int i=0; i<10; i++){
//    if(digitalRead(senR)) senVal++;
//    delayMicroseconds(10);
//  }
//  if(senVal>5) return true;
//  else return false;
//}
//bool isSenL(void){
//  int senVal=0;
//  for(int i=0; i<10; i++){
//    if(digitalRead(senL)) senVal++;
//    delayMicroseconds(10);
//  }
//  if(senVal>5) return true;
//  else return false;
//}

uint8_t pinsen[8] = { A0,A1,A2,A3,A4,A5,A6,A7 };
int trimsen[8] = { 0 };
uint8_t sensorDelay = 0;
#define lsPin1 27
#define lsPin2 29
#define lsPin3 31
#define lsPin4 33
#define isLs1 digitalRead(lsPin1)
#define isLs2 digitalRead(lsPin2)
#define isLs3 digitalRead(lsPin3)
#define isLs4 digitalRead(lsPin4)
void sensSetup(){
  pinMode(senL, INPUT); // 0
  pinMode(senR, INPUT); // 1
  pinMode(lsPin1, INPUT_PULLUP);
  pinMode(lsPin2, INPUT_PULLUP);
  pinMode(lsPin3, INPUT_PULLUP);
  pinMode(lsPin4, INPUT_PULLUP);
}

//======================================================================================== MOTOR
#define mKa1 12
#define mKa2 14
#define mKi1 13
#define mKi2 15
#define mLf1 22
#define mLf2 24
#define pwmKaPin 6
#define pwmKiPin 7
#define pwmLfPin 8

void motorSetup(){
  pinMode(mKa1, OUTPUT);
  pinMode(mKa2, OUTPUT);
  pinMode(mKi1, OUTPUT);
  pinMode(mKi2, OUTPUT);
  pinMode(mLf1, OUTPUT);
  pinMode(mLf2, OUTPUT);
  pinMode(pwmKaPin, OUTPUT);
  pinMode(pwmKiPin, OUTPUT);
  pinMode(pwmLfPin, OUTPUT);
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
  digitalWrite(mLf1, LOW);
  digitalWrite(mLf2, HIGH);
}
void mLiftDown(){
  digitalWrite(mLf1, HIGH);
  digitalWrite(mLf2, LOW);
}
void mLiftStop(){
  digitalWrite(mLf1, LOW);
  digitalWrite(mLf2, LOW);
}

//======================================================================================== NAVIGATION
bool isGripped = false;
bool flagInt = false;
bool flagVs = false;
bool flagTempatAmbil = false;
bool flagTempatLetak = false;
bool flagRTL = false;

uint8_t mulai = 1;
int8_t kondisix;
int8_t kondisiy;
int8_t tujuanx = 5;
int8_t tujuany = 0;
int8_t letakx;
int8_t letaky;
String arah;
int persimpangan = 0;
int simpang = 0;
uint8_t currentRack = 1;
void rack(uint8_t r){
  analogWrite(pwmLfPin, 100);
  if(r > currentRack){
    switch(r){
      case 2:
      while(isLs2){
        mLiftUp();
        if(!isLs3) break;
      }
      break;
      case 3:
      while(isLs3){
        mLiftUp();
      }
      break;
    }
  } else if(r < currentRack){
    switch(r){
      case 2:
      while(isLs2){
        mLiftDown();
      }
      mLiftStop();
      delay(500);
      while(isLs2){
        mLiftUp();
        if(!isLs3) break;
      }
      break;
      case 1:
      while(isLs1){
        mLiftDown();
      }
      mLiftStop();
      delay(500);
      while(isLs1){
        mLiftUp();
        if(!isLs2 || !isLs3) break;
      }
      break;
    }
  }
  currentRack = r;
  mLiftStop();
}

//======================================================================================== LCD 
// lcd bar
byte chr0[8]={32,32,32,32,32,32,32,31};
byte chr1[8]={32,32,32,32,32,32,31,31};
byte chr2[8]={32,32,32,32,32,31,31,31};
byte chr3[8]={32,32,32,32,31,31,31,31};
byte chr4[8]={32,32,32,31,31,31,31,31};
byte chr5[8]={32,32,31,31,31,31,31,31};
byte chr6[8]={32,31,31,31,31,31,31,31};
byte chr7[8]={31,31,31,31,31,31,31,31};

LiquidCrystal_I2C lcd(0x27, 16, 2);
void lcdSetup(){
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  
  lcd.createChar(1, chr0);
  lcd.createChar(2, chr1);
  lcd.createChar(3, chr2);
  lcd.createChar(4, chr3);
  lcd.createChar(5, chr4);
  lcd.createChar(6, chr5);
  lcd.createChar(7, chr6);
  lcd.createChar(8, chr7);
}
void lcdBar(uint8_t input, uint8_t x, uint8_t y){
  lcd.setCursor(x,y);
  if(input == 0){
    lcd.print(' ');
  } else {
    lcd.write(input);
  }
}
unsigned long delayUpdateLCD = millis();
void lcdLine0(char *message){
  lcd.setCursor(0,0);
  lcd.print(message);
}
void lcdLine1(char *message){
  if((unsigned long) millis() - delayUpdateLCD >= 500){
    lcd.setCursor(0,1);
    lcd.print(message);
    delayUpdateLCD = millis();
  }
}
//======================================================================================== BUTTON
#define btnLf 28
#define btnRt 34
#define btnUp 30
#define btnDw 32
#define btnCc 26
#define btnOk 36
#define readLf digitalRead(btnLf)
#define readRt digitalRead(btnRt)
#define readUp digitalRead(btnUp)
#define readDw digitalRead(btnDw)
#define readOk digitalRead(btnOk)
#define readCc digitalRead(btnCc)
void buttonSetup(){ pinMode(btnLf, INPUT_PULLUP); pinMode(btnRt, INPUT_PULLUP); pinMode(btnUp, INPUT_PULLUP); pinMode(btnDw, INPUT_PULLUP);   pinMode(btnCc, INPUT_PULLUP); pinMode(btnOk, INPUT_PULLUP); }
bool isLf(){ if(readLf == 0){ while(readLf == 0); return true; } else return false; }
bool isRt(){ if(readRt == 0){ while(readRt == 0); return true; } else return false; }
bool isUp(){ if(readUp == 0){ while(readUp == 0); return true; } else return false; }
bool isDw(){ if(readDw == 0){ while(readDw == 0); return true; } else return false; }
bool isOk(){ if(readOk == 0){ while(readOk == 0); return true; } else return false; }
bool isCc(){ if(readCc == 0){ while(readCc == 0); return true; } else return false; }

//======================================================================================== ULTRASONIC
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

//======================================================================================== TCS 
#define s0 A11
#define s1 A9
#define s2 A10
#define s3 A8
#define out A12

int colorFreq[4] = {0};
void tcsSetup(){
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
}
String scanWarna(){
  // merah
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  colorFreq[1] = pulseIn(out, LOW);

  // hijau
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  colorFreq[2] = pulseIn(out, LOW);

  // biru
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  colorFreq[3] = pulseIn(out, LOW);

  uint8_t color = 0;
  colorFreq[0] = 750;
  for(int j=1; j<=3; j++){
    Serial.print(colorFreq[j]);
    Serial.print('\t');
  
    if(colorFreq[0] > colorFreq[j]) {
      colorFreq[0] = colorFreq[j];
      color = j;
    }
  }

  switch(color){
    case 0: return "NONE"; break;
    case 1: Serial2.println("R"); letakx=1; letaky=2; return "R"; break;
    case 2: Serial2.println("G"); letakx=3; letaky=2; return "G"; break;
    case 3: Serial2.println("B"); letakx=2; letaky=3; return "B"; break;
  }
}

//======================================================================================== SERVO
#define gripperPin 10
#define engselPin 11

#if defined(robot1)
#define sudutGripperBuka 180
#define sudutGripperTutup 100
#define sudutEngselHor 0
#define sudutEngselVer 80
#elif defined(robot2)
#define sudutGripperBuka 100
#define sudutGripperTutup 180
#define sudutEngselHor 80
#define sudutEngselVer 0
#endif

Servo gripper;
Servo engsel;
void servoSetup(){
  gripper.attach(gripperPin);
  gripper.write(sudutGripperBuka);
  engsel.attach(engselPin);
  engsel.write(sudutEngselHor);
}

//======================================================================================== PID
float Kp = 0.1;
float Ki = 0.2;
float Kd = 0.3;
float iMax = 100;
float dErr = 0, iErr = 0;
volatile int8_t pv = 0;
uint8_t basePwm = 0;
bool inLine = false;
int8_t getPwm(){
  int8_t pErr = 0 - pv;
  int pwm = pErr*Kp + iErr*Ki + (pErr-dErr)*Kd;
  dErr = pErr;
  iErr += pErr;
  iErr = constrain(iErr, -iMax, iMax);
  return constrain(pwm, -basePwm, basePwm);
}

//======================================================================================== COMMUNICATION
char terimapesan[5];
int agv2x = 5;
int agv2y = 5;
int agv3x = 0;
int agv3y = 0;
int ambilBarang = 0;
int Rak = 0;
int Lt = 0;

bool isSafeRide(){
  Serial2.println("minta");
  delay(1);
  if(Serial2.available() > 1){
    String a = Serial2.readStringUntil('\n');
     Serial.println((String)a + "\t" + a.length());
    
    if(a.length() == 6 || a.length() == 7){
      if(a.toInt() != 0){
        String node = a.substring(0,1);
        String xx = a.substring(2,3);
        String yy = a.substring(4,5);
        Serial.println((String)"robot lain" + a + "\t" + node + "\t" + xx + "\t" + yy);
        
        #if defined(robot1)
        if(node == "2"){
        #elif defined(robot2)
        if(node == "1"){
        #endif
          agv2x = xx.toInt();
          agv2y = yy.toInt();
        } else {
          agv3x == xx.toInt();
          agv3y == yy.toInt();
        }
      }
    } else if(a.length() == 3){
      if(a.toInt() != 0){
        Serial.println("dapat kondisi rak");
        Rak = a.substring(0,1).toInt();
        Lt = a.substring(1,2).toInt();
        mKaStop();
        mKiStop();
        rack(Lt);
      }
    } else if(a.length() == 2){
      if(a.toInt() > 0 && a.toInt() < 10){
        ambilBarang = a.toInt();
        Serial.println("dapat status rak");
      }
    }
  }

  if(ambilBarang == 1){
    flagRTL = false;
    return true;
  } else if(ambilBarang == 9){
    flagRTL = true;
    return false;
  } else {
    return false;
  }
}
void sendLocation(){
  Serial2.println((String) mulai + "," + kondisix + "," + kondisiy);
}
bool berangkat(){
  Serial.println((String) agv2x + "," + agv2y);
  if(arah == "E"){
    if(agv2x == kondisix+1 && agv2y == kondisiy){
      return false;
    }
  } else
  if(arah == "N"){
    if(agv2y == kondisiy+1 && agv2x == kondisix){
      return false;
    }
  } else
  if(arah == "S"){
    if(agv2y == kondisiy-1 && agv2x == kondisix){
      return false;
    }
  } else
  if(arah == "W"){
    if(agv2x == kondisix-1 && agv2y == kondisiy){
      return false;
    }
  }
  return true;
}

//======================================================================================== MODES
enum _mode {
  homepage,
  running,
  setpoint,
  calibration,
  setting,
  tesleft,
  tesright,
  tesfull
} mode;
bool isNew = true;
int8_t iMenu = 0;
unsigned long debounceButton = millis();
#define delayButton 200

void vHomepage(){
  isNew = true;
  mKaStop();
  mKiStop();

  if(isUp() && millis()-debounceButton > delayButton){ if(--iMenu < 0) iMenu = 6; debounceButton = millis(); }
  if(isDw() && millis()-debounceButton > delayButton){ if(++iMenu > 6) iMenu = 0; debounceButton = millis(); }
  if(isOk() && millis()-debounceButton > delayButton){ 
    lcd.clear();
    switch(iMenu){
      case 0: mode=running; break;
      case 1: mode=setpoint; break;
      case 2: mode=calibration; break;
      case 3: mode=setting; break;
      case 4: mode=tesleft; break;
      case 5: mode=tesright; break;
      case 6: mode=tesfull; break;
      default: mode=homepage; break;
    }
    debounceButton = millis();
  }
  if(isCc() && millis()-debounceButton > delayButton){ iMenu = 0; debounceButton = millis(); }

  if((unsigned long) millis() - delayUpdateLCD >= 1000){
    lcdLine0("    MINI AGV    ");
    lcd.setCursor(0,1);
    lcd.print("Mode: ");
    switch(iMenu){
      case 0: lcd.print("Run       "); break;
      case 1: lcd.print("Setpoint  "); break;
      case 2: lcd.print("Calibrate "); break;
      case 3: lcd.print("Setting   "); break;
      case 4: lcd.print("Test Left "); break;
      case 5: lcd.print("Test Right"); break;
      case 6: lcd.print("Test Full "); break;
    }
    delayUpdateLCD = millis();
  }
}
//------------------------ vRunning
void vRunning(){
  lcd.clear();
  lcdLine0("    RUNNING     ");
  mKaForward();
  mKiForward();
  timerSetup();
  
  switch(mulai){
    case 1:
    kondisix = -2;
    kondisiy = 0;
    arah = "E";
    break;
    case 2:
    kondisix = -2;
    kondisiy= 2;
    arah = "E";
    break;
    case 3:
    kondisix = 2;
    kondisiy = -2;
    arah = "N";
    break;
  }
  isGripped = false;
  flagTempatAmbil = false;
  flagTempatLetak = false;
  flagRTL = false;
  
  while(readCc && (mode == running || mode == tesleft || mode == tesright || mode == tesfull)){
    if((unsigned long) millis()-delayUpdateLCD >= 500){
      delayUpdateLCD = millis();
      
      lcd.setCursor(0,1); 
      lcd.print(kondisix);
      lcd.print(",");
      lcd.print(kondisiy);
      lcd.print("             ");
      lcd.setCursor(4,1);
      lcd.print(usDist());
      for(int i=0; i<=7; i++){
        uint16_t adcIn = 0;
        adcIn = analogRead(pinsen[i]);
        delayMicroseconds(sensorDelay);
        if(adcIn > trimsen[i]){
          lcdBar(8,15-i,1);
        } else {
          lcdBar(1,15-i,1);
        }
      }
    }

    if(mode != running) goto skipcek;
    // ------ rak
    if(!isSafeRide()){
      mKaStop();
      mKiStop();
      Serial.println("rak penuh");
      delay(100);
      continue;
    }
    // cek robot lain
    if(!berangkat()){
      mKaStop();
      mKiStop();
      Serial.println("ada robot lain");
      delay(100);
      Serial.println("stop");
      continue;
    }
    
    if(flagTempatAmbil){
      getAndMeasure();
    }
    if(flagTempatLetak){
      placeToRack();
    }
    if(usDist() < 10 && !isGripped){
      mKaStop();
      mKiStop();
      continue;
    }
    skipcek:

    readSens();
    int8_t pwm = getPwm();
    uint8_t pwmKa = basePwm + pwm;
    uint8_t pwmKi = basePwm - pwm;
    
    mKaForward();
    mKiForward();
    if(inLine){
      analogWrite(pwmKaPin, pwmKa);
      analogWrite(pwmKiPin, pwmKi); 
    } else {
      analogWrite(pwmKaPin, 30);
      analogWrite(pwmKiPin, 30);
    }
  }
  mode = homepage;
  timerShutdown(); 
}

void getAndMeasure(){
  timerShutdown();
  gripper.write(sudutGripperBuka);
  engsel.write(sudutEngselHor);
  rack(1);
  
  while(inLine){
    readSens();
    int8_t pwm = getPwm();
    uint8_t pwmKa = basePwm + pwm;
    uint8_t pwmKi = basePwm - pwm;
    mKaForward();
    mKiForward();
    analogWrite(pwmKaPin, pwmKa);
    analogWrite(pwmKiPin, pwmKi); 
  }
  mKaStop();
  mKiStop();
  
  isGripped = true;
  flagTempatAmbil = false;
  arah = "N";
  
  delay(2000);

  scanWarna();
  isSafeRide(); // cek feedback
  // mekanisme dimensi di skip, mekanik tidak sesuai
  gripper.write(sudutGripperTutup);

  uint8_t countL = 0;
  bool flagL = false;
  
  readSens();
  while(!inLine || countL < 2){
    readSens();
    analogWrite(pwmKaPin, 35);
    analogWrite(pwmKiPin, 30);
    mKiBackward();
    mKaBackward();
    if(isSenL() && flagL == false){
      countL++;
      flagL = true;
      delay(50);
    }
    if(!isSenL() && flagL == true){
      flagL = false;
      delay(50);
    }
  }

  analogWrite(pwmKaPin, 40);
  mKaForward();
  while(!isSenR());
  delay(500);
  kondisix = 5;
  kondisiy = -1;
  iErr = 0;
  timerSetup();
}

void placeToRack(){
  timerShutdown();
  delayUpdateLCD = millis();
  while((unsigned long) millis()-delayUpdateLCD < 500){
    readSens();
    int8_t pwm = getPwm();
    uint8_t pwmKa = basePwm + pwm;
    uint8_t pwmKi = basePwm - pwm;
    mKaForward();
    mKiForward();
    analogWrite(pwmKaPin, pwmKa);
    analogWrite(pwmKiPin, pwmKi); 
  }
  mKaStop();
  mKiStop();
  analogWrite(pwmKaPin,35);
  analogWrite(pwmKiPin,30);

  gripper.write(sudutGripperBuka);
  delay(2000);
  
  uint8_t countL = 0;
  bool flagL = false;
  
  mKaBackward();
  mKiBackward();
  bool f1 = false, f2 = false;
  while(!f1 || !f2){ 
    if(isSenL()) f1 = true;
    if(isSenR()) f2 = true;
   }
  delay(100);
  mKaForward();
  while(isSenL() || isSenR());
  delay(1000);
  readSens();
  while(!isSenR() || !inLine){
    readSens();
  }
  delay(1000);
  readSens();
  while(pv != 0 && pv != 15 && pv != 15){
    readSens();
  }

  iErr = 0;
  readSens();
  f1 = false; f2 = false;
  while(!f1 || !f2){
    readSens();
    int8_t pwm = getPwm();
    int8_t pwmKa = 30 + pwm;
    int8_t pwmKi = 30 - pwm;
    pwmKa = constrain(pwmKa, 0, 60);
    pwmKi = constrain(pwmKi, 0, 60);
    mKaForward();
    mKiForward();
    analogWrite(pwmKaPin, pwmKa);
    analogWrite(pwmKiPin, pwmKi); 
    if(isSenL()) f1 = true;
    if(isSenR()) f2 = true;
  }
  
  readSens();
  flagInt = false;
  turnLeft();

  iErr = 0;
  kondisix = letakx;
  kondisiy = letaky;
  arah = "S";
  flagTempatLetak = false;
  isGripped = false;
  timerSetup();
}

//------------------------ vSetpoint
#define s_mulai 14
int8_t iSetpoint = s_mulai;

void vSetpoint(){
  if(isNew){
    lcdLine0("    SETPOINT    ");
    isNew = false;
    displaySet();
  }
  if(isCc() && (unsigned long) millis()-debounceButton > delayButton){
    mode = homepage;
    debounceButton = millis();
  }
  if(isOk() && (unsigned long) millis()-debounceButton > delayButton){
    writeEEPROM(iSetpoint); 
    debounceButton = millis();
    displaySet();
    lcd.setCursor(14,1);
    lcd.print("ok");
  }
  if(isRt() && (unsigned long) millis()-debounceButton > delayButton){
    debounceButton = millis();
    switch(iSetpoint){
      case s_mulai: if(++mulai > 3) mulai = 1; break;
    }
    displaySet();
  }
  if(isLf() && (unsigned long) millis()-debounceButton > delayButton){
    debounceButton = millis();
    switch(iSetpoint){
      case s_mulai: if(--mulai < 1) mulai = 3; break;
    }
    displaySet();
  }
}
void displaySet(){
  lcd.setCursor(0,1);
  switch(iSetpoint){
    case s_mulai:
    lcd.print("robot st:       ");
    lcd.setCursor(10, 1);
    lcd.print(mulai);
    lcd.setCursor(13,0);
    lcd.print("1-3");
    break;
  }
}

//------------------------ vCalibrate
bool isDigital = false;
unsigned long lcdCal = millis();
int trimtemp[8][2];
void vCalibration(){
  if(isNew){
    lcdLine0("  CALIBRATION   "); 
    isNew = false;
    for(int i=0; i<=7; i++){
      trimtemp[i][0] = 1024;
      trimtemp[i][1] = 0;
    }
  }
  if(isCc() && (unsigned long) millis()-debounceButton > delayButton){
    mode = homepage;
    debounceButton = millis();
  }
  if((isLf() || isRt() || isUp() || isDw()) && (unsigned long) millis()-debounceButton > delayButton){
    isDigital = !isDigital;
    debounceButton = millis();
  }
  if(isOk() && (unsigned long) millis()-debounceButton > delayButton){
    saveTrim();
    lcd.setCursor(0,1);
    lcd.print("    ok saved    ");
    delay(2000);
    debounceButton = millis();
  }
  
  uint16_t adcIn = 0;
  lcd.setCursor(0,1);
  if((unsigned long) millis() - lcdCal >= 500){
    lcdCal = millis();
    
    if(isDigital){  
      lcd.print("logic  :");
      for(int i=0; i<=7; i++){
        adcIn = analogRead(pinsen[i]);
        delayMicroseconds(sensorDelay);
        if(adcIn > trimsen[i]){
          lcdBar(8,15-i,1);
        } else {
          lcdBar(1,15-i,1);
        }
      }
    } else {
      lcd.print("analog :");
      for(int i=0; i<=7; i++){
        adcIn = analogRead(pinsen[i]);
        delayMicroseconds(sensorDelay);
        lcdBar(adcIn/128,15-i,1);

        if(trimtemp[i][0] > adcIn) trimtemp[i][0] = adcIn;
        if(trimtemp[i][1] < adcIn) trimtemp[i][1] = adcIn;
        trimsen[i] = (double)(trimtemp[i][1] - trimtemp[i][0]) / 2.0 + trimtemp[i][0];
        Serial.println((String) i + ". " + trimtemp[i][0] + ", " + trimtemp[i][1] + ", real: " + trimsen[i] + ", adcIn: " + adcIn);
      }
    }
  }
}

//------------------------ vSetting
#define s_speed 9
#define s_kp 10
#define s_ki 11
#define s_kd 12
#define s_sensordelay 13
int8_t iSetting = s_speed;
void vSetting(){
  if(isNew){
    displayVal();
    isNew = false;
  }
  if(isCc() && (unsigned long) millis()-debounceButton > delayButton){
    mode = homepage;
    debounceButton = millis();
  }
  if(isOk() && (unsigned long) millis()-debounceButton > delayButton){
    writeEEPROM(iSetting); 
    debounceButton = millis();
    displayVal();
    lcd.setCursor(14,1);
    lcd.print("ok");
  }
  if(isUp() && (unsigned long) millis()-debounceButton > delayButton){
    if(--iSetting < s_speed) iSetting = s_sensordelay;
    debounceButton = millis();
    displayVal();
  }
  if(isDw() && (unsigned long) millis()-debounceButton > delayButton){
    if(++iSetting > s_sensordelay) iSetting = s_speed;
    debounceButton = millis();
    displayVal();
  }
  if(readLf == 0 && (unsigned long) millis()-debounceButton > delayButton){
    switch(iSetting){
      case s_speed: if(basePwm > 0) basePwm -= 1; break;
      case s_kp: Kp -= 1; break;
      case s_ki: Ki -= 0.01; break;
      case s_kd: Kd -= 1; break;
      case s_sensordelay: sensorDelay -= 1; break;
    }
    debounceButton = millis();
    displayVal();
  }
  if(readRt == 0 && (unsigned long) millis()-debounceButton > delayButton){
    switch(iSetting){
      case s_speed: if(basePwm < 127) basePwm += 1; break;
      case s_kp: Kp += 1; break;
      case s_ki: Ki += 0.01; break;
      case s_kd: Kd += 1; break;
      case s_sensordelay: sensorDelay += 1; break;
    }
    debounceButton = millis();
    displayVal();
  }
}
void displayVal(){
    lcdLine0("    SETTING     ");
    lcd.setCursor(0,1);
    switch(iSetting){
      case s_speed:
      lcd.print("Speed:          ");
      lcd.setCursor(8,1);
      lcd.print(basePwm);
      break;
      case s_kp:
      lcd.print("Kp:             ");
      lcd.setCursor(4,1);
      lcd.print(Kp);
      break;
      case s_ki:
      lcd.print("Ki:             ");
      lcd.setCursor(4,1);
      char buff[5];
      dtostrf(Ki,5,3,buff);
      lcd.print(buff);
      break;
      case s_kd:
      lcd.print("Kd:             ");
      lcd.setCursor(4,1);
      lcd.print(Kd);
      break;
      case s_sensordelay:
      lcd.print("Sensor D:       ");
      lcd.setCursor(10,1);
      lcd.print(sensorDelay);
      break;
    }
}

//======================================================================================== TIMER
void timerSetup(){
  attachInterrupt(digitalPinToInterrupt(senL), interruptKi, CHANGE);
  attachInterrupt(digitalPinToInterrupt(senR), interruptKa, CHANGE);
}
void timerShutdown(){
  detachInterrupt(digitalPinToInterrupt(senR));
  detachInterrupt(digitalPinToInterrupt(senL));
}
/*
 * tergantung mana yg terbaca dahulu, lihat apakan perempatan atau pertigaan
 */
#define per4 0
#define per3l 1
#define per3r 2
#define d 10
volatile bool lock = false;
volatile unsigned long delayInt = millis();
void interruptKi(){
  if(mode != running && mode != tesleft && mode != tesright && mode != tesfull) return;
  if(lock == false && (unsigned long) millis()-delayInt > 500){
    delayInt = millis();
    timerShutdown();
    lock = true; 
    flagVs = false;
    delay(100);
    while(isSenL()){
      delay(d);
      if(isSenR()) flagVs = true;
    }
    if(flagVs){
      prosesNavigasi(per4);
    } else {
      prosesNavigasi(per3l);
    }
    lock = false;
    if(!flagTempatAmbil) timerSetup();
  }
}
void interruptKa(){
  if(mode != running && mode != tesleft && mode != tesright && mode != tesfull) return;
  if(lock == false && (unsigned long) millis()-delayInt > 500){
    delayInt = millis();     
    timerShutdown();
    lock = true;
    flagVs = false;
    delay(100);
    while(isSenR()){
      delay(d);
      if(isSenL()) flagVs = true;
    }
    if(flagVs){
      prosesNavigasi(per4);
    } else {
      prosesNavigasi(per3r);
    }
    lock = false;
    if(!flagTempatAmbil) timerSetup();
  }
}

#define berhenti 0
#define lurus 1
#define belkanan 2
#define belkiri 3
#define ulang 4
int counter = 0;
int arahNav[10] = {0};

void kontrolAmbil(bool count){
  if(arah == "N"){
    if(count) kondisiy += 1;
    if(kondisiy == tujuany){
      if(kondisix < tujuanx){
        turnRight();
        arah = "E";
      } else if(kondisix > tujuanx){
        turnLeft();
        arah = "W";
      } else {
        mKaStop();
        mKiStop();
      }
    }
    flagTempatAmbil = false;
  } else
  if(arah == "S"){
    if(count) kondisiy -= 1;
    if(kondisiy == tujuany){
      if(kondisix < tujuanx){
        turnLeft();
        arah = "E";
      } else if(kondisix > tujuanx){
        turnRight();
        arah = "W";
      } else {
        // di tempat pengambilan barang , dari atas / arahnya menuju S
        turnLeft();
        // beri flag berada ditempat pengambilan 
        flagTempatAmbil = true;
      }
    }
  } else
  if(arah == "E"){
    if(count) kondisix += 1;
    if(kondisix == tujuanx){
      if(kondisiy < tujuany){
        turnLeft();
        arah = "N";
      } else if(kondisiy > tujuany){
        turnRight();
        arah = "S";
      } else {
        // di tempat pengambilan barang , dari kiri / arahnya menuju E
        // beri flag di tempat pengambilan
        flagTempatAmbil = true;
      }
    } else if(kondisix == 0 && kondisiy == 2){
      turnRight();
      arah = "S";
    }
  } else
  if(arah == "W"){
    if(count) kondisix -= 1;
    if(kondisix == tujuanx){
      if(kondisiy < tujuany){
        turnRight();
        arah = "N";
      } else if(kondisiy > tujuany){
        turnLeft();
        arah = "S";
      } else {
        mKaStop();
        mKiStop();
      }
    }
    flagTempatAmbil = false;
  }
}
void kontrolLetak(bool count){
  if(arah == "W" && kondisix-1 == letakx && kondisiy == letaky) count = true;
  
  if(arah == "N"){
    if(count) kondisiy += 1;
    if(kondisiy == letaky){
      if(kondisix < letakx){
        turnRight();
        arah = "E";
      } else if(kondisix > letakx){
        turnLeft();
        arah = "W";
      } else {
        mKaStop();
        mKiStop();
        while(1);
      }
    }
  } else
  if(arah == "S"){
    if(count) kondisiy-=1;
    if(kondisiy == letaky){
      if(kondisix < letakx){
        turnLeft();
        arah = "E";
      } else if(kondisix > letakx){
        turnRight();
        arah = "W";
      } else {
        mKaStop();
        mKiStop();
        while(1);
      }
    }
  } else 
  if(arah == "E"){
    if(count) kondisix += 1;
    if(kondisix == letakx){
      if(kondisiy < letaky){
        turnLeft();
        arah = "N";
      } else if(kondisiy > letaky){
        turnRight();
        arah = "S";
      } else {
        flagTempatLetak = true;
      }
    } 
  } else 
  if(arah == "W"){
    if(count) kondisix -= 1;
    if(kondisix == letakx){
      if(kondisiy < letaky){
        turnRight();
        arah = "N";
      } else if(kondisiy > letaky){
        turnLeft();
        arah = "S";
      } else {
        turnRight();
        flagTempatLetak = true;
      }
    }
  }
}
void prosesNavigasi(int nav){
  readSens();
  flagInt = false;
  delay(1000);

  if(mode != running) goto testmode;
  switch(nav){
    case per3l:
    case per3r:
    if(!isGripped){
      // belum ada barang
      if((kondisix > 0 && kondisix < 5) && (kondisiy == 2 || kondisiy == 3)){
        kontrolAmbil(false);
      } else {
        kontrolAmbil(true);
      }
    } else {
      // kene
      if((kondisix > 0 && kondisix < 5) && (kondisiy == 2 || kondisiy == 3)){
        kontrolLetak(false);
      } else {
        kontrolLetak(true);
      }
    }
    break;
    case per4:
    if(!isGripped){
      kontrolAmbil(true);
    } else {
      kontrolLetak(true);
    }
    break;
  }
  if(kondisix == letakx && kondisiy == letaky) return;
  sendLocation();
  sendLocation();
  sendLocation();
  return;

  testmode:
  if(mode == tesfull){
    arahNav[0] = belkiri;
    arahNav[1] = belkiri;
    arahNav[2] = belkanan; 
    arahNav[3] = belkanan; 
    arahNav[4] = belkanan; 
    arahNav[5] = belkanan; 
    arahNav[6] = belkiri; 
    arahNav[7] = belkiri; 
    arahNav[8] = ulang; 
    arahNav[9] = ulang;
  } else if(mode == tesleft){
    arahNav[0] = belkiri; 
    arahNav[1] = belkiri; 
    arahNav[2] = belkiri; 
    arahNav[3] = belkiri; 
    arahNav[4] = belkiri; 
    arahNav[5] = belkiri; 
    arahNav[6] = belkiri; 
    arahNav[7] = belkiri; 
    arahNav[8] = ulang; 
    arahNav[9] = ulang;
  } else if(mode == tesright){
    arahNav[0] = belkanan; 
    arahNav[1] = belkanan;
    arahNav[2] = belkanan; 
    arahNav[3] = belkanan; 
    arahNav[4] = belkanan; 
    arahNav[5] = belkanan; 
    arahNav[6] = belkanan; 
    arahNav[7] = belkanan; 
    arahNav[8] = ulang; 
    arahNav[9] = ulang;
  }
  readSens();
  proses:
  readSens();
  flagInt = false;
  delay(1000);
  switch(arahNav[counter]){
    case belkanan:
    turnRight();
    break;
    case belkiri:
    turnLeft();
    break;
    case berhenti:
    mKaStop();
    mKiStop();
    break;
    case ulang:
    counter = 0;
    goto proses;
    break;
  }
  counter++;
}

bool flagTurn = false;
bool flagSenR = false;
void turnRight(){
  flagInt = false;
  flagTurn = false;
  flagSenR = false;
  while(!inLine || pv != 0 || !flagInt || !flagTurn || !flagSenR){
    readSens();
    delay(100);
    if(isSenR() && !flagInt){
      flagInt = true;
      delay(200);
    }
    if(!isSenR() && flagInt && !flagTurn){
      flagTurn = true;
      delay(200);
    }
    
    if(kondisiy == 2){
      if(isSenL()){
        flagSenR = true;
      }
    } else {
      flagSenR = true;
    }
    
    mKaBackward();
    mKiForward();
    #if defined(robot1)
    analogWrite(pwmKaPin, 30);
    analogWrite(pwmKiPin, 30);
    #elif defined(robot2)
    analogWrite(pwmKaPin, 30);
    analogWrite(pwmKiPin, 30);
    #endif
  }
}
void turnLeft(){
  flagInt = false;
  flagTurn = false;
  flagSenR = false;
  
  while(!inLine || pv != 0 || !flagInt || !flagTurn || !flagSenR){
    readSens();
    delay(100);
    if(isSenL() && !flagInt){
      flagInt = true;
      delay(200);
    }
    if(!isSenL() && flagInt && !flagTurn){
      flagTurn = true;
      delay(200);
    }

    if(kondisiy == 2){
      if(isSenR()){
        flagSenR = true;
      }
    } else {
      flagSenR = true;
    }
    
    if(kondisiy == letaky && kondisix == letakx){
      if((pv == -15 || pv == 15) && flagTempatLetak == false) pv = 0;
    }
      
    mKaForward();
    mKiBackward();
    #if defined(robot1)
    analogWrite(pwmKaPin, 35);
    analogWrite(pwmKiPin, 30);
    #elif defined(robot2)
    analogWrite(pwmKaPin, 30);
    analogWrite(pwmKiPin, 30);
    #endif
  }
}

void readSens(){
  int sens = 0;
  uint16_t adcIn = 0;
  
  for(int i=0; i<=7; i++){
    adcIn = analogRead(pinsen[i]);
    delayMicroseconds(sensorDelay);
    
//    //=========== Filtering Sensor ================
//    int sensVal = 0;
//    for(int i=0; i<10; i++){
//      sensVal += analogRead(pinsen[i]);
//      delayMicroseconds(10);
//    }
//    adcIn = (double) sensVal/10;
    
    if(adcIn > trimsen[i]){
      switch(i){
        case 0: sens += 1; break;
        case 1: sens += 2; break;
        case 2: sens += 4; break;
        case 3: sens += 8; break;
        case 4: sens += 16; break;
        case 5: sens += 32; break;
        case 6: sens += 64; break;
        case 7: sens += 128; break;
      }
    }
  }

  inLine = true;
  switch(sens){
    case 0b11100000: pv=-120; break;
    case 0b11000000: pv=-120; break;
    case 0b10000000: pv=-120; break;
    
    case 0b01110000: pv=-50; break;
    case 0b01100000: pv=-50; break;
    case 0b01000000: pv=-50; break;
    
    case 0b00111000: pv=-15; break;
    case 0b00110000: pv=-15; break;
    case 0b00100000: pv=-15; break;
    case 0b00010000: pv=-15; break;
    
    case 0b00011100: pv=15; break;
    case 0b00001100: pv=15; break;
    case 0b00001000: pv=15; break;
    case 0b00000100: pv=15; break;
    
    case 0b00001110: pv=50; break;
    case 0b00000110: pv=50; break;
    case 0b00000010: pv=50; break;
    
    case 0b00000111: pv=120; break;
    case 0b00000011: pv=120; break;
    case 0b00000001: pv=120; break;

    #ifdef robot2
//    case 0b10011000: pv=0; break;
//    case 0b11011000:
//    case 0b11001000: pv=0; break;
//    case 0b11001100:
//    case 0b11001101: pv =15; break;
    #endif
    case 0b11111111:
    case 0b00011000: pv=0; break;    
    case 0b00000000: pv=0; inLine = false; break;
//    case 0b11100111: 
//    case 0b11000011:
//    case 0b00111100: pv=0; break;
//    case 0b01111110:
//    case 0b10000001: pv=0; break;
  }
}

//======================================================================================== EEPROM 
void readEEPROM(){
  basePwm = EEPROM.read(s_speed);
  Kp = EEPROM.read(s_kp);
  Ki = EEPROM.read(s_ki)/100.0;
  Kd = EEPROM.read(s_kd);
  sensorDelay = EEPROM.read(s_sensordelay);
  mulai = EEPROM.read(s_mulai);
}
void writeEEPROM(uint8_t i){
  switch(i){
    case s_speed: EEPROM.write(s_speed, basePwm); break;
    case s_kp: EEPROM.write(s_kp, Kp); break;
    case s_ki: EEPROM.write(s_ki, Ki*100.0); break;
    case s_kd: EEPROM.write(s_kd, Kd); break;
    case s_sensordelay: EEPROM.write(s_sensordelay, sensorDelay); break;
    case s_mulai: EEPROM.write(s_mulai, mulai); break;
  }
}
void saveTrim(){
  Serial.println("Saving trim value");
  for(int i=0; i<=7; i++){
    EEPROM.write(i+20, ((trimsen[i]+1)/4)-1);
    Serial.print(trimsen[i]);
    Serial.print('\t');
    Serial.println(((trimsen[i]+1)/4)-1);
  }
}
void readTrim(){
  Serial.println("Read trim value");
  for(int i=0; i<=7; i++){
    trimsen[i] = (EEPROM.read(i+20)+1)*4+1;
    Serial.println(trimsen[i]);
  }
}

//========================================================================================//========================================================================================
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  readEEPROM();
  readTrim();
  lcdSetup();
  buttonSetup();
  motorSetup();
  servoSetup();
  sensSetup();
  usSetup();
  tcsSetup();
//  rack(3);
//  delay(1000);
//  rack(1);
} 

void loop() {
  if(Serial.available() > 0){
    String a = Serial.readStringUntil('\n');
    if(a.indexOf("hor") >= 0) {
      engsel.write(sudutEngselHor);
      return;
    }
    if(a.indexOf("ver") >= 0){
      engsel.write(sudutEngselVer);
      return;
    }
    if(a.indexOf("clo") >= 0){
      gripper.write(sudutGripperTutup);
      return;
    }
    if(a.indexOf("ope") >= 0){
      gripper.write(sudutGripperBuka);
      return;
    }
    if(a.indexOf("col") >= 0){
      Serial.println(scanWarna());
      return;
    }
    if(a.indexOf("dis") >= 0){
      Serial.println(usDist());
      return;
    }
    if(a.toInt() < 4)
      rack(a.toInt());
    else
      gripper.write(a.toInt());
    Serial.println(a);
  }
  switch(mode){
    case homepage: vHomepage(); break;
    case running: vRunning(); break;
    case setpoint: vSetpoint(); break;
    case calibration: vCalibration(); break;
    case setting: vSetting(); break;
    case tesleft: vRunning(); break;
    case tesright: vRunning(); break;
    case tesfull: vRunning(); break;
    default: vHomepage(); break;
  }
}
