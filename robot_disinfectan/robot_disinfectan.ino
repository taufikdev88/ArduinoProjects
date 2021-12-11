/*
 * Robot Disinfectan Gresik -- Gudang Robot ------------------------------------------------------------------ HEADER ----------------------------------------------------------------------
 */
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <TimerOne.h>

/*
 * HOW TO USE DEBUG MODE           ----------------------------------------------------------------------------- DEBUG ---------------------------------------------------------------------
 * 
 * 1. debug kalibrasi   #define debug_kalibrasi
 * 2. debug rcoffline   #define debug_rcoffline
 * 3. debug rconline    #define debug_rconline
 * 4. debug autokanan   #define debug_autokanan
 * 5. debug autokiri    #define debug_autokiri
 * 6. debug setting     #define debug_setting
 * 
 * HOW TO NOT USE DEBUG MODE ? EASY ! Comment it
 */
//#define debug_rcoffline
//#define debug_autokiri
//#define debug_kalibrasi
//#define debug_setting

#if defined(debug_rcoffline)
#include <Psx.h>

#define dataPin 15  //coklat==putih
#define cmndPin 10  //abang==abu-abu
#define attPin 12   //ijo==ijo
#define clockPin 14 //biru==kuning

Psx Psx;
#endif

/*
 * ultrasonic -------------------------------------------------------------------------------------------- ULTRASONIC -----------------------------------------------------------------------
 */
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

void setupUs(){
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
}

#define maximumRange 150
volatile int usLeftVal, usLeftFrontVal, usFrontVal, usRightFrontVal, usRightVal;
unsigned int maxdist = 50;
unsigned int mindist = 30;

#define usTimeout 200000
int usLeft(){
  digitalWrite(us1t, LOW);
  delayMicroseconds(2);
  digitalWrite(us1t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us1t, LOW);
  int jarak = pulseIn(us1e, HIGH, usTimeout) / 29 / 2;
  if(jarak == 0) return usLeftVal;
  if(jarak >= maximumRange) return maximumRange;
  return jarak;
}
int usLeftFront(){
  digitalWrite(us2t, LOW);
  delayMicroseconds(2);
  digitalWrite(us2t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us2t, LOW);
  int jarak = pulseIn(us2e, HIGH, usTimeout) / 29 / 2;
  if(jarak == 0) return usLeftFrontVal;
  if(jarak >= maximumRange) return maximumRange;
  return jarak;
}
int usFront(){
  digitalWrite(us3t, LOW);
  delayMicroseconds(2);
  digitalWrite(us3t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us3t, LOW);
  int jarak = pulseIn(us3e, HIGH, usTimeout) / 29 / 2;
  if(jarak == 0) return usFrontVal;
  if(jarak >= maximumRange) return maximumRange;
  return jarak;
}
int usRightFront(){
  digitalWrite(us4t, LOW);
  delayMicroseconds(2);
  digitalWrite(us4t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us4t, LOW);
  int jarak = pulseIn(us4e, HIGH, usTimeout) / 29 / 2;
  if(jarak == 0) return usRightFrontVal;
  if(jarak >= maximumRange) return maximumRange;
  return jarak;
}
int usRight(){
  digitalWrite(us5t, LOW);
  delayMicroseconds(2);
  digitalWrite(us5t, HIGH);
  delayMicroseconds(10);
  digitalWrite(us5t, LOW);
  int jarak = pulseIn(us5e, HIGH, usTimeout) / 29 / 2;
  if(jarak == 0) return usRightVal;
  if(jarak >= maximumRange) return maximumRange;
  return jarak;
}

/*
 * LCD Setup                                      --------------------------------------------------------------------- LCD ----------------------------------------------------------------
 */
LiquidCrystal lcd(31, 30, 44, 45, 46, 47);
void setupLCD(){
  lcd.begin(16, 2);
  lcd.print("BISMILLAH!!!");
  delay(2000);
  lcd.clear();
}
unsigned long delayUpdateLCD = millis();
void lcdLine0(char *message){
  if(millis() - delayUpdateLCD >= 10){
    lcd.setCursor(0,0);
    lcd.print(message);
    delayUpdateLCD = millis();
  }
}
void lcdLine1(char *message){
  if(millis() - delayUpdateLCD >= 1000){
    lcd.setCursor(0,1);
    lcd.print(message);
    delayUpdateLCD = millis();
  }
}

/*
 * Button setup and read                     ------------------------------------------------------------------- BUTTON -----------------------------------------------------------------
 */
#define up 27
#define dw 28
#define ok 26
#define cc 29
#define readUp digitalRead(up)
#define readDw digitalRead(dw)
#define readOk digitalRead(ok)
#define readCc digitalRead(cc)
void setupButton(){
  pinMode(up, INPUT_PULLUP);
  pinMode(dw, INPUT_PULLUP);
  pinMode(ok, INPUT_PULLUP);
  pinMode(cc, INPUT_PULLUP);
}
bool isUp(){
  if(readUp == 0){
    while(readUp == 0);
    return true;
  } else return false;
}
bool isDw(){
  if(readDw == 0) {
    while(readDw == 0);
    return true;
  } else return false;
}
bool isOk(){
  if(readOk == 0){
    while(readOk == 0);
    return true;
  } else return false;
}
bool isCc(){
  if(readCc == 0){
    while(readCc == 0);
    return true;
  } else return false;
}

/*
 * MOTOR                                      ----------------------------------------------------------------------- MOTOR -------------------------------------------------------------
 */
//--- pin and var for MotorKiri
#define pb 5
#define b1 4 //kiri
#define b2 9
int pwmKi;                          // menyimpan pwm yang di dapat dari PID untuk dikeluarkan ke motor
float iErrKi, dErrKi;                       // untuk menyimpan nilai delta error
int rpmKiRef = 0;                   // menyimpan nilai rpm referensi yang diinginkan (dari program / dari user)
volatile int rpmKiAct;              // menyimpan nilai rpm sebenarnya yang didapat dari rumus pengolahan encoder
volatile unsigned int pulseKi;      // menyimpan nilai interrupt yang dibangkitkan dari encoder

//--- pin and var for MotorKanan
#define pa 8
#define a1 6 //kanan
#define a2 7
int pwmKa;
float iErrKa, dErrKa;
int rpmKaRef = 0;
volatile int rpmKaAct;
volatile unsigned int pulseKa;

unsigned int maxRpm = 40;
unsigned int turnRpm = 20;

/*
 * fungsi default untuk setting arah motor
 */
void setupMotor(){
  pinMode(pa, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(pb, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT); 
  mKiForward();
  mKaForward();
  analogWrite(pa, 0);
  analogWrite(pb, 0);
}
void mKaForward(){
  digitalWrite(a1, 1);
  digitalWrite(a2, 0);
}
void mKaBackward(){
  digitalWrite(a1, 0);
  digitalWrite(a2, 1);
}
void mKiForward(){
  digitalWrite(b1, 1);
  digitalWrite(b2, 0);
}
void mKiBackward(){
  digitalWrite(b1, 0);
  digitalWrite(b2, 1);
}
void rcF()    { lcdLine1("    FORWARD     "); mKiForward();   mKaForward();   rpmKiRef=maxRpm;    rpmKaRef=maxRpm;     }
void rcRF()   { lcdLine1(" RIGHT FORWARD  "); mKiForward();   mKaForward();   rpmKiRef=maxRpm;    rpmKaRef=turnRpm;    }
void rcR()    { lcdLine1("     RIGHT      "); mKiForward();                   rpmKiRef=maxRpm;    rpmKaRef=0;          }
void rcRB()   { lcdLine1(" RIGHT BACKWARD "); mKiBackward();  mKaBackward();  rpmKiRef=maxRpm;    rpmKaRef=turnRpm;    }
void rcB()    { lcdLine1("    BACKWARD    "); mKiBackward();  mKaBackward();  rpmKiRef=maxRpm;    rpmKaRef=maxRpm;     }
void rcLB()   { lcdLine1(" LEFT BACKWARD  "); mKiBackward();  mKaBackward();  rpmKiRef=turnRpm;   rpmKaRef=maxRpm;     }
void rcL()    { lcdLine1("      LEFT      ");                 mKaForward();   rpmKiRef=0;         rpmKaRef=maxRpm;     }
void rcLF()   { lcdLine1("  LEFT FORWARD  "); mKiForward();   mKaForward();   rpmKiRef=turnRpm;   rpmKaRef=maxRpm;     }
void rcSTOP() { lcdLine1("      STOP      "); pwmKi=0;        pwmKa=0;        rpmKiRef=0;         rpmKaRef=0;          }
void rcCW()   { lcdLine1("   ROTATE CW    "); mKiForward();   mKaBackward();  rpmKiRef=turnRpm;   rpmKaRef=turnRpm;     }
void rcCCW()  { lcdLine1("   ROTATE CCW   "); mKiBackward();  mKaForward();   rpmKiRef=turnRpm;   rpmKaRef=turnRpm;     }

/*
 * PID                                   ----------------------------------------------------------------------- PD Controller ----------------------------------------------------------
 */
//--- pid const
#define slowKpKi 0.03
#define slowKiKi 0.015
#define slowKdKi 0.05
#define slowKpKa 0.03
#define slowKiKa 0.015
#define slowKdKa 0.05

#define fastKpKi 0.2
#define fastKiKi 0.04
#define fastKdKi 0.07
#define fastKpKa 0.2
#define fastKiKa 0.04
#define fastKdKa 0.07

float KpKi, KiKi, KdKi;
float KpKa, KiKa, KdKa;

//--- PID void --- single void for multiple pid application
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

/*
 * fungsi fungsi routine yang dijalankan di void loop -------------------------------------------------------- MENU TAMPILAN -----------------------------------------------------------
 */
//--- menu tampilan
enum _mode {
  homepage,
  kalibrasi,
  rc,
  autokanan,
  autokiri,
  setting
} mode;
int8_t indexMenu = 0;

unsigned long debounceButton = millis();
#define delayButton 500

// view homepage ------------------------------------------------------------------------------------ VIEW HOME PAGE ------------------------------------------------------
void vhomepage(){
  if(isUp() && millis()-debounceButton > delayButton){
    if(--indexMenu < 0) indexMenu = 4;
    debounceButton = millis();
  }
  if(isDw() && millis()-debounceButton > delayButton){
    if(++indexMenu > 4) indexMenu = 0;
    debounceButton = millis();
  }
  if(isOk() && millis()-debounceButton > delayButton){
    lcd.clear();
    switch(indexMenu){
      case 0: mode=autokanan; break;
      case 1: mode=autokiri; break;
      case 2: mode=rc; break;
      case 3: mode=kalibrasi; break;
      case 4: mode=setting; break;
      default: mode=homepage; break;
    }
    debounceButton = millis();
  }
  if(isCc() && millis()-debounceButton > delayButton){
    indexMenu = 0;
    debounceButton = millis();
  }
  if(millis() - delayUpdateLCD >= 1000){
    lcd.setCursor(0,0);
    lcd.print("Pilih mode:     ");
    
    lcd.setCursor(0, 1);
    switch(indexMenu){
      case 0: lcd.print("MODE Auto Kanan "); break;
      case 1: lcd.print("MODE Auto Kiri  "); break;
      case 2: lcd.print("MODE RC         "); break;
      case 3: lcd.print("Cek Sensor      "); break;
      case 4: lcd.print("MODE Setting    "); break;
    }
    delayUpdateLCD = millis();
  }
}

// view kalibrasi ----------------------------------------------------------------------------------------- VIEW KALIBRASI ---------------------------------------------------------------
void vkalibrasi(){
  unsigned long delayViewKalibrasi = millis();
  while(readCc){
//    if(millis() - delayViewKalibrasi >= 500){
//      noInterrupts();
//      lcd.setCursor(0,1);
//      lcd.print("     Sensor      ");
//      lcd.setCursor(0,0);
//      lcd.print("                ");
//      lcd.setCursor(0,1);
//      lcd.print(usLeftVal);
//      lcd.setCursor(0,0);
//      lcd.print(usLeftFrontVal);
//      lcd.setCursor(6,0);
//      lcd.print(usFrontVal);
//      lcd.setCursor(13,0);
//      lcd.print(usRightFrontVal);
//      lcd.setCursor(13,1);
//      lcd.print(usRightVal);
//      delayViewKalibrasi = millis();
//      interrupts();
//    }
  }
  mode=homepage;
}

// view rc ------------------------------------------------------------------------------------------------------ VIEW RC --------------------------------------------------------
enum _dirRc {
  F,
  RF,
  R,
  RB,
  B,
  LB,
  L,
  LF,
  STOP,
  CW,
  CCW
} dirRc;

void vrc(){
  lcdLine0("    Mode RC     ");
  dirRc = STOP;
  KpKa = slowKpKa;
  KiKa = slowKiKa;
  KdKa = slowKdKa;
  KpKi = slowKpKi;
  KiKi = slowKiKi;
  KdKi = slowKdKi;
  
  while(readCc){
//    if(!isSafeRide()) continue; // cek jarak
    
    #ifndef debug_rcoffline
    if(Serial.available()){
      String command = Serial.readStringUntil('\n');
  
      if(command.indexOf("F\r") >= 0) { rcF();    dirRc=F; }
      if(command.indexOf("RF") >= 0)	{	rcRF();   dirRc=RF; }
      if(command.indexOf("R\r") >= 0)	{	rcR();    dirRc=R; }
      if(command.indexOf("RB") >= 0)	{	rcRB();   dirRc=RB; }
      if(command.indexOf("B\r") >= 0) { rcB();    dirRc=B; }
      if(command.indexOf("LB") >= 0)	{	rcLB();   dirRc=LB; }
      if(command.indexOf("L\r") >= 0)	{	rcL();    dirRc=L; }
      if(command.indexOf("LF") >= 0)	{	rcLF();   dirRc=LF; }
      if(command.indexOf("STOP") >= 0){ rcSTOP(); dirRc=STOP; }
      if(command.indexOf("CW\r") >= 0){	rcCW();   dirRc=CW; }
      if(command.indexOf("CCW") >= 0)	{	rcCCW();  dirRc=CCW; }
      if(command.indexOf("cw\r") >= 0){	rcSTOP(); dirRc=STOP; }
      if(command.indexOf("ccw") >= 0)	{ rcSTOP(); dirRc=STOP; }
    }
    #endif
    
    #if defined(debug_rcoffline)
    #define atas 8
    #define kanan 4
    #define kiri 1
    #define bawah 2
    #define X 512
    #define R1 4096
    
    static int last_stick = 23213;
    static int data_stick = 0;
    data_stick = Psx.read();
    if(data_stick == last_stick) goto skipStick;
    last_stick = data_stick;
    switch(data_stick){
      case 0:             rcSTOP(); dirRc=STOP; break;
      case atas:          rcF();    dirRc=F; break;
      case bawah:         rcB();    dirRc=B; break;
      case kanan:         rcR();    dirRc=R; break;
      case kiri:          rcL();    dirRc=L; break;
      case (atas+kanan):  rcRF();   dirRc=RF; break;
      case (atas+kiri):   rcLF();   dirRc=LF; break;
      case R1:            rcCCW();  dirRc=CCW; break;
    }
    skipStick:
    #endif

    pwmKa = PID(rpmKaRef, rpmKaAct, KpKa, KiKa, KdKa, &iErrKa, &dErrKa, 0, 255);
    pwmKi = PID(rpmKiRef, rpmKiAct, KpKi, KiKi, KdKi, &iErrKi, &dErrKi, 0, 255);
    
    Serial.print(rpmKiRef);
    Serial.print('\t');
    Serial.print(rpmKiAct);
    Serial.print('\t');
    Serial.print(iErrKa);
    Serial.print('\t');
    Serial.print(dErrKa);
    Serial.print('\t');
    Serial.print(pwmKi);
    Serial.print('\t');
    Serial.print(pwmKa);
    Serial.println();
    if(pwmKa < 10) pwmKa = 0;
    if(pwmKi < 10) pwmKi = 0;
    analogWrite(pa, pwmKa);
    analogWrite(pa, pwmKi);
  }
  mode=homepage;
}

// RC Mode   ----------------------------------------------------------------------------------------------------  RC Mode Safe Ride ----------------------------------------------------
// run in timer routine
unsigned long delayReadUs = millis();
bool isSafeRide(){ 
  if(millis() - delayReadUs > 100){
    #if defined(debug_rcoffline)
    Serial.print(usLeftVal);
    Serial.print('\t');
    Serial.print(usLeftFrontVal);
    Serial.print('\t');
    Serial.print(usFrontVal);
    Serial.print('\t');
    Serial.print(usRightFrontVal);
    Serial.print('\t');
    Serial.print(usRightVal);
    Serial.println();
    #endif

    if(usRightVal == 0 || usRightFrontVal == 0 || usFrontVal == 0 || usLeftFrontVal == 0 || usLeftVal == 0){
       lcd.setCursor(0,1);
       lcd.print("   Not Ready    "); 
       return false;
    }
    
    if(usRightVal < maxdist)     { rcLF();   return false; }
    if(usRightFrontVal < maxdist){ rcCCW();  return false; }
    if(usFrontVal < maxdist)     { rcB();    return false; }
    if(usLeftFrontVal < maxdist) { rcCW();   return false; }
    if(usLeftVal < maxdist)      { rcRF();   return false; }

    delayReadUs = millis();
    return true;
  }
  return true;
}

// view auto kiri           ----------------------------------------------------------------------------------------- VIEW AUTO KIRI -----------------------------------------------------
unsigned int autoTimer = 0;
unsigned long startTime = millis();
void vautokiri(){
  startTime = millis();
  rcSTOP();
  lcdLine0(" Mode Auto Kiri ");
  while(readCc && (unsigned long) millis()-startTime < autoTimer*60000){
    // run trace tembok
  }
  mode=homepage;
}

// view autokanan               ------------------------------------------------------------------------------------ VIEW AUTO KANAN ------------------------------------------------------
void vautokanan(){
  startTime = millis();
  rcSTOP();
  lcdLine0("Mode Auto Kanan ");
  while(readCc && (unsigned long) millis()-startTime < autoTimer*60000){
    // run trace tembok
  }
  mode=homepage;
}

// view setting                   ------------------------------------------------------------------------------------- VIEW SETTING ------------------------------------------------------
#define s_maxrpm 0
#define s_turnrpm 1
#define s_maxdistance 2
#define s_safedistance 3
#define s_autotimer 4
int8_t iSetting = s_maxrpm;
bool inSetting = false;
unsigned int *valueNeedToChange;

void vsetting(){
  if(isCc() && millis()-debounceButton > delayButton){
    if(!inSetting) mode = homepage;
    else inSetting = false;
    debounceButton = millis();
  }
  if(isOk() && millis()-debounceButton > delayButton){
    if(inSetting) writeEEPROM();
    if(!inSetting){
      switch(iSetting){
        case s_maxrpm:        valueNeedToChange = &maxRpm; break;
        case s_turnrpm:       valueNeedToChange = &turnRpm; break;
        case s_maxdistance:   valueNeedToChange = &maxdist; break;
        case s_safedistance:  valueNeedToChange = &mindist; break;
        case s_autotimer:     valueNeedToChange = &autoTimer; break;
      }      
    }
    inSetting = !inSetting;
  }

  switch(inSetting){
    case true:
    if(readUp == 0 && (millis()-debounceButton) > 500){
      *valueNeedToChange = *valueNeedToChange + 1;
      debounceButton = millis();
    }
    if(readDw == 0 && (millis()-debounceButton) > 500){
      *valueNeedToChange = *valueNeedToChange - 1;
      debounceButton = millis();
    }
    break;
    case false:
    if(isUp() && millis()-debounceButton > delayButton){
      if(--iSetting < 0) iSetting = s_autotimer;
      debounceButton = millis();
    }
    if(isDw() && millis()-debounceButton > delayButton){
      if(++iSetting > 4) iSetting = s_maxrpm;
      debounceButton = millis();
    }
    break;
  }
  lcdLine0("  Mode Setting  ");
  
  static unsigned long delayUpdateLCDSetting = millis();
  if(millis() - delayUpdateLCDSetting >= 100){
    lcd.setCursor(15,1);
    if(inSetting) lcd.print("_");
    else lcd.print(" ");
    
    switch(iSetting){
      case s_maxrpm:
      lcd.setCursor(0,1);
      lcd.print("maxrpm:         ");
      lcd.setCursor(8,1);
      lcd.print(maxRpm);
      break;
      case s_turnrpm:
      lcd.setCursor(0,1);
      lcd.print("turnrpm:        ");
      lcd.setCursor(9,1);
      lcd.print(turnRpm);
      break;
      case s_maxdistance:
      lcd.setCursor(0,1);
      lcd.print("maxdis:         ");
      lcd.setCursor(8,1);
      lcd.print(maxdist);
      break;
      case s_safedistance:
      lcd.setCursor(0,1);
      lcd.print("safedis:        ");
      lcd.setCursor(9,1);
      lcd.print(mindist);
      break;
      case s_autotimer:
      lcd.setCursor(0,1);
      lcd.print("auto Timer:     ");
      lcd.setCursor(12,1);
      lcd.print(autoTimer);
      break;
    }
  }
}

/*
 * EEPROM                          --------------------------------------------------------------------------------- EEPROM -----------------------------------------------------------------
 */
void readEEPROM(){
  maxRpm = EEPROM.read(0);
  turnRpm = EEPROM.read(1);
  maxdist = EEPROM.read(2);
  mindist = EEPROM.read(3);
  autoTimer = EEPROM.read(4);
}

void writeEEPROM(){
  EEPROM.write(0, maxRpm);
  EEPROM.write(1, turnRpm);
  EEPROM.write(2, maxdist);
  EEPROM.write(3, mindist);
  EEPROM.write(4, autoTimer);
}

//--- handler untuk menjalankan fungsi motor dan pid  ---------------------------------------------------------------- HANDLER RPM & PD Control --------------------------------------
void handler(){
  noInterrupts();  
  readRPM();
//  if(mode == homepage || mode == setting) goto skipupdatejarak;
//  updateUS();
//  skipupdatejarak:
  interrupts();
}

//--- read rpm void   ----------------------------------------------------------------------------------------------- READ RPM --------------------------------------------------------
#define pulsesperturn 72.0
unsigned long timeold = 0;
void readRPM(){
  rpmKaAct = (pulseKa/pulsesperturn) * 1000.0 / ((unsigned long)millis()-timeold) * 60.0;
  rpmKiAct = (pulseKi/pulsesperturn) * 1000.0 / ((unsigned long)millis()-timeold) * 60.0;
  timeold = millis();
  Serial.print(rpmKiAct);
  Serial.print('\t');
  Serial.print(rpmKaAct);
  Serial.println();
  pulseKa = 0;
  pulseKi = 0;
}
/// update us void -----------------------------------------------------------------
uint8_t i=0;
void updateUS(){
  switch(i){
    case 0: usRightVal = usRight(); break;
    case 1: usRightFrontVal = usRightFront(); break;
    case 2: usFrontVal = usFront(); break;
    case 3: usLeftFrontVal = usLeftFront(); break;
    case 4: usLeftVal = usLeft(); break;
  }
  if(++i > 4) i = 0;    
}

unsigned long lastIntKi; 
void interruptKi(){
  if((unsigned long) millis() - lastIntKi > 1){
    pulseKi++; 
    lastIntKi = millis();
    Serial.println(pulseKi);
  }
}

unsigned long lastIntKa;
void interruptKa(){
  if((unsigned) millis() - lastIntKa > 1){
    pulseKa++;
    Serial.println(pulseKa);
    lastIntKa = millis();
  }
}

void setupTimer(){
  attachInterrupt(1, interruptKi, CHANGE);
  attachInterrupt(0, interruptKa, CHANGE); // INT 0
  
  Timer1.initialize(100000); // micro min 40000 microSc()
  Timer1.attachInterrupt(handler);
}

//***************************************************************************************************************** MAIN **************************************************************
void setup()
{
  mode = homepage;
  #if defined(debug_rcoffline)
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 0);
  mode = rc;
  #endif
  
  #if defined(debug_kalibrasi)
  mode = kalibrasi;
  #endif
  #if defined(debug_rconline)
  mode = rc;
  #endif
  #if defined(debug_autokanan)
  mode = autokanan;
  #endif
  #if defined(debug_autokiri)
  mode = autokiri;
  #endif
  #if defined(debug_setting)
  mode = setting;
  #endif

  Serial.begin(9600);
//  readEEPROM();
//  setupButton();
//  setupMotor();
//  setupTimer();
//  setupUs();
//  setupLCD();
  mKaForward();
  mKiForward();
  analogWrite(pa, 70);
  analogWrite(pb, 70);
}

// *********************************************************************************************************************** LOOP *********************************************************
void loop(){
  return;
//  #if defined(debug_kalibrasi)
//  rpmKiRef = 0;
//  mKaForward(); 
//  
//  while(1){
//    if(Serial.available()){
//      String a = Serial.readStringUntil('\n');
//
//      if(a.indexOf("kpki") >= 0) KpKi = a.substring(4).toFloat();
//      if(a.indexOf("kpka") >= 0) KpKa = a.substring(4).toFloat();
//      if(a.indexOf("kdki") >= 0) KdKi = a.substring(4).toFloat();
//      if(a.indexOf("kdka") >= 0) KdKa = a.substring(4).toFloat();
//      if(a.indexOf("rpm") >= 0) rpmKaRef = a.substring(3).toInt();
//    }
//
//    static int i = 0;
//    static unsigned long a = millis();
//    if(millis() - a >= 5000){
//      a = millis();
//
//      switch(i){
//        case 0: rpmKaRef = 30; break;
//        case 1: rpmKaRef = 90; break;
//        case 2: rpmKaRef = 40; break;
//        case 3: rpmKaRef = 50; break;
//        case 4: rpmKaRef = 60; break;
//        case 5: rpmKaRef = 0; break;
//      }
//      if(++i > 5) i = 0;
//    }
//  }
//  #endif

  KpKa = slowKpKa;
  KiKa = slowKiKa;
  KdKa = slowKdKa;
  KpKi = slowKpKi;
  KiKi = slowKiKi;
  KdKi = slowKdKi;

  rpmKaRef = 30;
  pwmKa = PID(rpmKaRef, rpmKaAct, KpKa, KiKa, KdKa, &iErrKa, &dErrKa, 0, 255);
  Serial.println(pwmKa);
  analogWrite(pa, pwmKa);
  return;
//  switch(mode){
//    case homepage: vhomepage(); break;
//    case kalibrasi: vkalibrasi(); break;
//    case rc: vrc(); break;
//    case autokanan: vautokanan(); break;
//    case autokiri: vautokiri(); break;
//    case setting: vsetting(); break;
//    default: vhomepage(); break;
//  }
}
