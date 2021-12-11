#define BLYNK_PRINT Serial
#define RELAY_ON 0
#define RELAY_OFF !RELAY_ON

#define dhtPin 5
#define kipas 2
#define heater 4

#define btnUp 13
#define btnDown 14
#define btnSelect 15

#define DHTTYPE DHT22   

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

char auth[] = "IX76YFIHUnt654ktP_PC7fvAv71-Ov8d";
#define ssid "RumahNikita"
#define pass "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(dhtPin, DHTTYPE);

int tempRef = 0;
int rhRef = 0;

int rhAct = 0;
int tempAct = 0;

bool toggleSetting = 0;
unsigned long lcdUpdate = 0;
unsigned long btnUpdate = 0;

void updateData(){
  rhAct = dht.readHumidity();
  tempAct = dht.readTemperature();

  if(tempAct > tempRef){
    digitalWrite(kipas, RELAY_ON);
  } else if(tempAct < tempRef){
    digitalWrite(kipas, RELAY_OFF);
  }
  if(rhAct < rhRef){
    digitalWrite(heater, RELAY_ON);
  } else if(rhAct > rhRef){
    digitalWrite(heater, RELAY_OFF);
  }

  Serial.print("Humidity: ");
  Serial.println(rhAct);
  Serial.print("Temperature: ");
  Serial.println(tempAct);
  
  Blynk.virtualWrite(V6, rhAct);
  Blynk.virtualWrite(V7, tempAct);
  Blynk.virtualWrite(V8, rhRef);
  Blynk.virtualWrite(V9, tempRef);
}

BLYNK_WRITE(V1)
{
  rhRef = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  Serial.print("Mendapat referensi Humidity: ");
  Serial.println(rhRef);
}

BLYNK_WRITE(V2){
  tempRef = param.asInt();

  Serial.print("Mendapat referensi Temperature: ");
  Serial.println(tempRef);
}

void setup()
{
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  
  dht.begin();
  pinMode(kipas, OUTPUT);
  pinMode(heater, OUTPUT);

  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, updateData);
}

void loop()
{
  Blynk.run();
  timer.run();
  
  if(isBtnUp() && (unsigned long) millis()-btnUpdate >= 500){
    btnUpdate = millis();
    if(toggleSetting) rhRef += 1;
    else tempRef += 1;
  }

  if(isBtnDown() && (unsigned long) millis()-btnUpdate >= 500){
    btnUpdate = millis();
    if(toggleSetting) rhRef -= 1;
    else tempRef -= 1;
  }

  if(isBtnSelect() && (unsigned long) millis()-btnUpdate >= 500){
    btnUpdate = millis();
    toggleSetting = !toggleSetting;
  }
  
  if((unsigned long) millis()-lcdUpdate >= 500){
    lcdUpdate = millis();
    
    lcd.setCursor(0,0);
    lcd.print("Hmdt:           ");
    lcd.setCursor(7,0);
    lcd.print(rhAct);
    lcd.print("(");
    lcd.print(rhRef);
    lcd.print(")");
    lcd.setCursor(0,1);
    lcd.print("Temp:           ");
    lcd.setCursor(7,1);
    lcd.print(tempAct);
    lcd.print("(");
    lcd.print(tempRef);
    lcd.print(")");

    if(toggleSetting){
      lcd.setCursor(19,0);
    } else {
      lcd.setCursor(19,1);
    }
    lcd.print("*");
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
