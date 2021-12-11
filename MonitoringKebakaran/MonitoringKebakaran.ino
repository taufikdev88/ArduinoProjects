#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
//#include "mq2sensing.h"
#include <max6675.h>
#include <Servo.h>

char auth[] = "4xHZJZASCsMZTnEh14PmLyKMW5WDoiMj";
char ssid[] = "RumahNikita";
char pass[] = "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd";

//***************************** INPUT
#define mq2 A0
#define thermoD0 19
#define thermoCS 23
#define thermoCLK 5

#define btnUp 14
#define btnDown 15
#define btnSelect 16

//***************************** OUTPUT
#define kipasPin 27
#define pompaPin 32
#define servoPin 13

#define RELAY_ON 0
#define RELAY_OFF !RELAY_ON
#define OPEN_ANGLE 90
#define CLOSE_ANGLE 0

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 20, 4);
MAX6675 thermo(thermoCLK, thermoCS, thermoD0);
Servo servo;

//int PPM_LPG = 0;
//int PPM_CO = 0;
//int PPM_Smoke = 0;
int TEMP = 0;
bool GAS = false;

//int PPM_LPG_REF = 500;
//int PPM_Smoke_REF = 500;
int TEMP_REF = 50;

bool isDanger1 = false;
bool isDanger2 = false;

/*
 * 0 = standby
 * 1 = ppm lpg ref
 * 2 = ppm smoke ref
 * 3 = temp ref
 */
int8_t menu = 0;
unsigned long lcdUpdate = 0;
unsigned long btnUpdate = 0;

void updateData(){
//  PPM_LPG = MQGetGasPercentage(MQRead(mq2)/Ro,GAS_LPG);
//  PPM_CO = MQGetGasPercentage(MQRead(mq2)/Ro,GAS_CO);
//  PPM_Smoke = MQGetGasPercentage(MQRead(mq2)/Ro,GAS_SMOKE);

  GAS = (digitalRead(mq2) == 0 ? true : false);
  TEMP = thermo.readCelsius();

//  if(PPM_LPG >= PPM_LPG_REF || PPM_Smoke >= PPM_Smoke_REF){
//    servo.write(OPEN_ANGLE);
//    digitalWrite(kipasPin, RELAY_ON);
//    isDanger1 = true;
//  } else if(PPM_LPG < PPM_LPG_REF && PPM_Smoke < PPM_Smoke_REF){
//    isDanger1 = false;
//    servo.write(CLOSE_ANGLE);
//    digitalWrite(kipasPin, RELAY_OFF);
//  }

  if(GAS){
    servo.write(OPEN_ANGLE);
    digitalWrite(kipasPin, RELAY_ON);
    isDanger1 = true;
  } else {
    servo.write(CLOSE_ANGLE);
    digitalWrite(kipasPin, RELAY_OFF);
    isDanger1 = false;
  }
  
  if(TEMP >= TEMP_REF){
    isDanger2 = true;
    digitalWrite(pompaPin, RELAY_ON);
  } else {
    isDanger2 = false;
    digitalWrite(pompaPin, RELAY_OFF);
  }
  
//  Blynk.virtualWrite(V6, PPM_LPG);
//  Blynk.virtualWrite(V7, PPM_CO);
//  Blynk.virtualWrite(V8, PPM_Smoke);
  Blynk.virtualWrite(V8, (GAS ? 1 : 0));
  Blynk.virtualWrite(V9, TEMP);
  Blynk.virtualWrite(V10, (isDanger1 || isDanger2 ? 1:0));
}

//BLYNK_WRITE(V1){
//  PPM_LPG_REF = param.asInt();
//  Serial.print("Mendapatkan referensi PPM LPG: ");
//  Serial.println(PPM_LPG_REF);
//}
//
//BLYNK_WRITE(V2){
//  PPM_Smoke_REF = param.asInt();
//  Serial.print("Mendapatkan referensi PPM Asap: ");
//  Serial.println(PPM_Smoke_REF);
//}

BLYNK_WRITE(V3){
  TEMP_REF = param.asInt();
  Serial.print("Mendapatkan referensi Suhu: ");
  Serial.println(TEMP_REF);
}

void setup()
{
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  servo.attach(servoPin);
  servo.write(CLOSE_ANGLE);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(mq2, INPUT);
  pinMode(kipasPin, OUTPUT);
  pinMode(pompaPin, OUTPUT);
  
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);

//  // kalibrasi mq2
//  digitalWrite(LED_BUILTIN, 1);
//  lcd.setCursor(0,0);
//  lcd.print("Kalibrasi Sensor Gas");
//  Serial.println("Kalibrasi Sensor Gas");
//
//  Ro = MQCalibration(mq2);
//
//  digitalWrite(LED_BUILTIN, 0);
//  lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.print("Kalibrasi Selesai");
//  delay(1000);
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, updateData);
}

void loop()
{
  Blynk.run();
  timer.run();

  if(isBtnUp() && (unsigned long) millis()-btnUpdate >= 500){
    btnUpdate = millis();
    switch(menu){
//      case 1: PPM_LPG_REF += 1; break;
//      case 2: PPM_Smoke_REF += 1; break;
      case 3: TEMP_REF += 1; break;
    }
  }

  if(isBtnDown() && (unsigned long) millis()-btnUpdate >= 500){
    btnUpdate = millis();
    switch(menu){
//      case 1: PPM_LPG_REF -= 1; break;
//      case 2: PPM_Smoke_REF -= 1; break;
      case 3: TEMP_REF -= 1; break;
    }
  }

  if(isBtnSelect() && (unsigned long) millis()-btnUpdate >= 500){
    btnUpdate = millis();
    ++menu;
    if(menu > 0) menu = 3;
    if(menu > 3) menu = 0;
  }
  
  if((unsigned long) millis()-lcdUpdate >= 500){
    lcdUpdate = millis();

    if(isDanger1){
      lcd.setCursor(0,0);
      lcd.print("GAS TERDETEKSI!!");
      lcd.setCursor(0,1);
      lcd.print("                ");
//      lcd.setCursor(0,2);
//      lcd.print("                    ");
//      lcd.setCursor(0,3);
//      lcd.print("                    ");
    }
    if(isDanger2){
      lcd.setCursor(0,2);
      lcd.print("SUHU TINGGI!!   ");
    }
    if(isDanger1 || isDanger2) return;
    
    switch(menu){
      case 0:
      lcd.setCursor(0,0);
      lcd.print("GAS DETECTED:   ");
      lcd.setCursor(13,0);
      lcd.print(GAS ? "YES":"NO ");

      lcd.setCursor(0,1);
      lcd.print("TEMP:          ");
      lcd.setCursor(6,1);
      lcd.print(TEMP);
      break;
//      case 1:
//      lcd.setCursor(0,0);
//      lcd.print("SETPOINT PPM GAS LPG");
//      lcd.setCursor(0,1);
//      lcd.print("                    ");
//      lcd.setCursor(0,1);
//      lcd.print(PPM_LPG_REF);
//      lcd.print(" ppm");
//      lcd.setCursor(0,2);
//      lcd.print("                    ");
//      lcd.setCursor(0,3);
//      lcd.print("                    ");
//      break;
//      case 2:
//      lcd.setCursor(0,0);
//      lcd.print("SETPOINT PPM ASAP   ");
//      lcd.setCursor(0,1);
//      lcd.print("                    ");
//      lcd.setCursor(0,1);
//      lcd.print(PPM_Smoke_REF);
//      lcd.print(" ppm");
//      lcd.setCursor(0,2);
//      lcd.print("                    ");
//      lcd.setCursor(0,3);
//      lcd.print("                    ");
//      break;
      case 3:
      lcd.setCursor(0,0);
      lcd.print("SETPOINT SUHU   ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(TEMP_REF);
//      lcd.setCursor(0,2);
//      lcd.print("                    ");
//      lcd.setCursor(0,3);
//      lcd.print("                    ");
      break;
      default:
      menu = 0;
      break;
    }
  }
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
bool isBtnDown(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnDown) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
bool isBtnSelect(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnSelect) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
