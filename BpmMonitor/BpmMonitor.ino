#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// pin button
#define btnUp 2
#define btnDw 3
#define btnSl 4
// pin buzzer
#define buzzer 5
// pin sensor bpm
#define output A0
#define lo_min 11
#define lo_plus 10

uint8_t bpmThreshold = 90;
uint8_t threshold = 50;
uint8_t bpm = 0;
bool flagTh  = true;
bool flagTuning = 0;
bool flagTuning2 = 0;
uint8_t menu = 0;
bool buzzState = false;

unsigned long lcdTiming = 0;
unsigned long btnTiming = 0;

void setup() {
  Serial.begin(115200);

  Serial.println(F("Inisialisasi Pinout"));
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDw, INPUT_PULLUP);
  pinMode(btnSl, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(lo_min, INPUT);
  pinMode(lo_plus, INPUT);

  Serial.println(F("Inisialisasi LCD"));
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  Serial.println(F("Membaca nilai threshold yang tersimpan"));
  threshold = EEPROM.read(0);
  threshold = constrain(threshold, 0, 100);
  bpmThreshold = EEPROM.read(1);

  Serial.println(F("Program dimulai"));
}

void loop() {
  if(digitalRead(lo_min) == 1 || digitalRead(lo_plus) == 1) {
    bpm = 0;
  } else {
    uint8_t sensorValue = bacaSensor();
    if(sensorValue > threshold && flagTh){
      hitungBPM();
      flagTh = false;
    } else if(sensorValue < threshold){
      flagTh = true;
    } 
  }
 
  if(isBtnUp() && (unsigned long) millis()-btnTiming >= 500){
    switch(menu){
      case 1: threshold += 1; flagTuning = 1; break;
      case 2: bpmThreshold += 1; flagTuning2 = 1; break;
    }
    btnTiming = millis();
  }
  if(isBtnDw() && (unsigned long) millis()-btnTiming >= 500){
    switch(menu){
      case 1: threshold -= 1; flagTuning = 1; break;
      case 2: bpmThreshold -= 1; flagTuning2 = 1; break;
    }
    btnTiming = millis();
  }
  if(isBtnSl() && (unsigned long) millis()-btnTiming >= 500){
    if(++menu > 2) menu = 0;
    btnTiming = millis();
  }
  if(flagTuning && (unsigned long) millis()-btnTiming >= 5000){
    flagTuning = 0;
    simpanThreshold();
  }
  if(flagTuning2 && (unsigned long) millis()-btnTiming >= 5000){
    flagTuning2 = 0;
    simpanBpmThreshold();
  }
  
  if((unsigned long) millis()-lcdTiming >= 500){
    switch(menu){
      case 0: vMonitor(); break;
      case 1: vTuning(); break;
      case 2: vTuning2(); break;
    }
    
    if(bpm >= bpmThreshold){
      digitalWrite(buzzer, buzzState);
      buzzState = !buzzState;
    }
  }
}

void vMonitor(){
  char buff[16] = {0};
  char f1[7] = {0};
  dtostrf(bpm,3,2,f1);
  sprintf(buff,"BPM: %s",f1);
  lcdLine(1, "   HEART RATE   ");
  lcdLine(2, buff);
}

void vTuning(){
  char buff[16] = {0};
  sprintf(buff, "Th(0-100): %d", threshold);
  lcdLine(1, "Threshold Tuning");
  lcdLine(2, buff);
}

void vTuning2(){
  char buff[16] = {0};
  sprintf(buff, "Bpm Th: &d", bpmThreshold);
  lcdLine(1, " BPM Threshold  ");
  lcdLine(2, buff);
}

// output skala 0-100
uint8_t bacaSensor(){
  uint16_t value = analogRead(output);
  return (value / 1024.0) * 100;
}

unsigned long dBeat = millis();
float beats[500] = {0};
uint16_t index = 0;
void hitungBPM(){
  unsigned long beat = millis();
  int diff = beat - dBeat;
  float currentBpm = 60000.0 / diff;
  beats[index] = currentBpm;
  float total = 0.0;
  for(uint8_t i=0; i<500; i++){
    total += beats[i];
  }
  bpm = total / 500;
  dBeat = beat;
  if(++index >= 500) index = 0;
}

void lcdLine(uint8_t line, String text){
  lcd.setCursor(0, line-1);
  lcd.print("                ");
  lcd.setCursor(0, line-1);
  lcd.print(text);
}

void simpanThreshold(){
  EEPROM.write(0, threshold);
}

void simpanBpmThreshold(){
  EEPROM.write(1, bpmThreshold);
}

#define MAX_DETECT 20
#define MIN_DETECT 15
bool isBtnUp(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnUp) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
bool isBtnDw(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnDw) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
bool isBtnSl(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnSl) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
