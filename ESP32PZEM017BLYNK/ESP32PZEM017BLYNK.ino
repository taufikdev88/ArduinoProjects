#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <PZEM004Tv30.h>
#include <SimpleModbusMaster.h>
#include <LiquidCrystal_I2C.h>
#define baud 9600
#define timeout 1000
#define polling 200 // the scan rate
#define retry_count 100 
#define TxEnablePin 2

#define auth "55dt91QmWb3TNWJRxy6sE2HJJXFStdw1"
#define ssid "rumahnikita"
#define pass "58828515"

#define RELAY_CUTOFF 23
#define VOLTAGE_SENSOR 36

float MAX_SOLAR_VOLTAGE = 16.0;

float MIN_BATTERY_OFF_VOLTAGE = 12.5;
float MIN_BATTERY_ON_VOLTAGE = 13.5;

#define RELAY_ON 0
#define RELAY_OFF !RELAY_ON

// variable global
float PZEM017Voltage = 0; 
float PZEM017Current = 0;
float PZEM017Power = 0;
float PZEM017Energy = 0;
float PZEM004Voltage = 0;
float PZEM004Current = 0;
float PZEM004Power = 0;
float PZEM004Energy = 0;
float SolarVoltage = 0;

unsigned long startMillisReadData;
#define periodReadData 1000
unsigned long lcdSwitch;
#define periodLcdSwitch 2000
bool lcdToggle = 0;

enum {
  PACKET1,
  TOTAL_NO_OF_PACKETS
};
Packet packets[TOTAL_NO_OF_PACKETS];
packetPointer packet1 = &packets[PACKET1];
unsigned int regs[8];

PZEM004Tv30 pzem(&Serial1);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup(){  
  Serial.begin(9600);
  lcd.begin();
  lcd.clear();
  lcd.backlight();   
  pinMode(RELAY_CUTOFF, OUTPUT);
  pinMode(VOLTAGE_SENSOR, INPUT);
  
  Blynk.begin(auth, ssid, pass); 
    
  packet1->id = 1;
  packet1->function = READ_INPUT_REGISTERS;
  packet1->address = 0;
  packet1->no_of_registers = 8;
  packet1->register_array = regs;
  modbus_configure(baud, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS);
  
  startMillisReadData = millis(); 
}

void loop(){
  Blynk.run();
  if((unsigned long) millis() - startMillisReadData >= periodReadData){  
    modbus_update(packets); // update pzem017 
    PZEM017Voltage = regs[0]*0.01;
    PZEM017Current = regs[1];
    PZEM017Power = regs[2];
    PZEM017Energy = regs[4];
    PZEM004Voltage = pzem.voltage();
    PZEM004Current = pzem.current();
    PZEM004Power = pzem.power();
    PZEM004Energy = pzem.energy();
    SolarVoltage = (analogRead(VOLTAGE_SENSOR) / 2192.0) * MAX_SOLAR_VOLTAGE;

    if(PZEM017Voltage < MIN_BATTERY_OFF_VOLTAGE) digitalWrite(RELAY_CUTOFF, RELAY_OFF);
    else if(PZEM017Voltage > MIN_BATTERY_ON_VOLTAGE) digitalWrite(RELAY_CUTOFF, RELAY_ON);
                                   
    Serial.print("Vdc : "); Serial.print(PZEM017Voltage); Serial.println(" V ");
    Serial.print("Idc : "); Serial.print(PZEM017Current); Serial.println(" A ");
    Serial.print("Power : "); Serial.print(PZEM017Power); Serial.println(" W ");
    Serial.print("Energy : "); Serial.print(PZEM017Energy); Serial.println(" Wh ");
    
    Blynk.virtualWrite(V0,PZEM017Voltage);
    Blynk.virtualWrite(V1,PZEM017Current);
    Blynk.virtualWrite(V2,PZEM017Power);
    Blynk.virtualWrite(V3,PZEM017Energy);
    Blynk.virtualWrite(V4,SolarVoltage);
    Blynk.virtualWrite(V5,MAX_SOLAR_VOLTAGE);
    Blynk.virtualWrite(V6,PZEM004Voltage);
    Blynk.virtualWrite(V7,PZEM004Current);
    Blynk.virtualWrite(V8,PZEM004Power);
    Blynk.virtualWrite(V9,PZEM004Energy);
    startMillisReadData = millis();
  }

  if((unsigned long) millis()-lcdSwitch >= periodLcdSwitch){
    char buff[21];
    char vStr[9];
    char cStr[9];
    char pStr[9];
    char eStr[9];
    
    if(lcdToggle){
      dtostrf(PZEM017Voltage,5,2,vStr);
      dtostrf(PZEM017Current,5,2,cStr);
      dtostrf(PZEM017Power,5,2,pStr);
      dtostrf(PZEM017Energy,5,2,eStr);

      sprintf(buff, "Vol: %s", vStr);
      lcdLine(1, buff);
      lcd.setCursor(18,0);
      lcd.print("AC");
      sprintf(buff, "Cur: %s", cStr);
      lcdLine(2, buff);
      sprintf(buff, "Pwr: %s", pStr);
      lcdLine(3, buff);
      sprintf(buff, "Eng: %s", eStr);
      lcdLine(4, buff);
    } else {
      dtostrf(PZEM004Voltage,5,2,vStr);
      dtostrf(PZEM004Current,5,2,vStr);
      dtostrf(PZEM004Power,5,2,pStr);
      dtostrf(PZEM004Energy,5,2,eStr);
      
      sprintf(buff, "Vol: %s", vStr);
      lcdLine(1, buff);
      lcd.setCursor(18,0);
      lcd.print("DC");
      sprintf(buff, "Cur: %s", cStr);
      lcdLine(2, buff);
      sprintf(buff, "Pwr: %s", pStr);
      lcdLine(3, buff);
      sprintf(buff, "Eng: %s", eStr);
      lcdLine(4, buff);
    }
    lcdToggle = !lcdToggle;
    lcdSwitch = millis();
  }
}

BLYNK_WRITE(V10){
  String v = param.asString();
  MAX_SOLAR_VOLTAGE = v.toFloat();
}

BLYNK_WRITE(V11){
  String v = param.asString();
  MIN_BATTERY_OFF_VOLTAGE = v.toFloat();
}

BLYNK_WRITE(V12){
  String v = param.asString();
  MIN_BATTERY_ON_VOLTAGE = v.toFloat();
}

BLYNK_WRITE(V13){
  int a = param.asInt();
  if(a == 1){
    pzem.resetEnergy();
  }
}

void lcdLine(uint8_t line, String text){
  lcd.setCursor(0, line-1);
  lcd.print("                ");
  lcd.setCursor(0, line-1);
  lcd.print(text);
}
