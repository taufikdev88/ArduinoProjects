/*
 * Program flow control untuk pengisian pupuk cair per galon
 */
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
/*
 * Sensor PH pin
 */
float PH_CALIBRATION_VALUE = 21.34;
#define phSensorPin A0
/*
 * Pin sensor DHT
 */
#define dhtPin 3
#define DHTTYPE DHT22  
DHT dht(dhtPin, DHTTYPE);
/*
 * Sesuai dengan spesififkasi sensor
 */
#define pulsePerLiter 450
/*
 * Pin sensor flow yang diletakkan pada pipa pupuk
 */
#define flowSensor 2
/*
 * Pin sensor yang diletakkan di tabung air
 */
#define sensorBawah 4
#define sensorAtas 5
/*
 * Pin relay yang digunakan untuk menyalakan pompa
 */
#define relayAir 6
#define relayPupuk 7
/*
 * Pin button yang digunakan untuk setting setpoint, konfigurasi INPUT_PULLUP
 */
#define btnUp 8
#define btnDown 9
#define btnSelect 10
/*
 * Relay PH up down dan exhaust Fan
 */
#define relayPhUp 11
#define relayPhDown 12
#define relayFan 13
/*
 * Sensor Pupuk dan Buzzer
 */
#define sensorPupuk 22
#define buzzer 23
bool state;
/*
 * Settingan default relay dan sensor atas bawah
 * 1. jika relay on harus dikasih logic 0 maka nilai RELAY_ON adalah 0
 * 2. jika sensor float yang digunakan aktif memberi nilai 0 maka nilai SENSOR_AKTIF adalah 0
 * 
 */
#define RELAY_ON 1
#define RELAY_OFF !RELAY_ON
/*
 * Objek lcd i2c
 */
LiquidCrystal_I2C lcd(0x27, 20, 4);
/*
 * Penghitung pulse yang didapat dari flow control
 */
volatile uint32_t pulse = 0;
void countPulse(){
  pulse++;
}
/*
 * Menu
 * 0 monitoring
 * 1 setting flow setpoint
 * 2 setting ph setpoint
 * 3 setting temp setpoint
 */
uint8_t menu = 0;
/*
 * Update lcd setiap 500 ms sekali
 */
#define TREFRESH 500
unsigned long tRefresh = 0;
/*
 * Pencimpan debounce button
 */
#define TBUTTON 1000
unsigned long tButton = 0;
/*
 * Buffer untuk menyimpan text untuk ditampilkan
 */
char txtBuffer[21];
char floatString[6];
/*
 * Variable penyimpan nilai flow referensi dan aktualnya
 * masukkan sensor ph di pin A0
 */
float flowRef = 0;
float flowAct = 0;
#define MAX_FLOW_REF 4
#define MIN_FLOW_REF 0
/*
 * Variable penyimpan nilai tegangan sensor PH dan nilai PH 
 */
float phVoltage;
float phValue;
float phRef = 14; // set phReferensi default
#define MAX_PH_REF 100
#define MIN_PH_REF 0
/*
 * Variable peyimpan nilai humidity dan Temperatur
 */
float humidity = 0;
float humidityRef = 50;
#define MAX_HUMI_REF 100
#define MIN_HUMI_REF 0
float temperature;
float temperatureMax = 30;
#define MAX_TEMP_REF 100
#define MIN_TEMP_REF 0

void setup() {
  Serial.begin(115200);
  Serial.println("Mulai program");
  
  lcd.begin();
  lcd.clear();
  lcd.backlight();

  dht.begin();

  pinMode(sensorBawah, INPUT_PULLUP);
  pinMode(sensorAtas, INPUT_PULLUP);
  pinMode(sensorPupuk, INPUT_PULLUP);
  
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);

  pinMode(relayAir, OUTPUT);
  pinMode(relayPupuk, OUTPUT);
  pinMode(relayPhUp, OUTPUT);
  pinMode(relayPhDown, OUTPUT);
  pinMode(relayFan, OUTPUT);
  pinMode(buzzer, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(flowSensor), countPulse, FALLING);

  digitalWrite(relayAir, RELAY_OFF);
  digitalWrite(relayPupuk, RELAY_OFF);
}

void loop() {
  /*
   * Cek pemencetan tombol 
   */
  if(isBtnUp() && (unsigned long) millis()-tButton >= TBUTTON){
    switch(menu){
      case 1: if(flowRef < MAX_FLOW_REF) flowRef += 1.0; break;
      case 2: if(phRef < MAX_PH_REF) phRef += 1.0; break;
//      case 3: if(temperatureMax < MAX_TEMP_REF) temperatureMax += 1.0; break;
      case 3: if(humidityRef < MAX_HUMI_REF) humidityRef += 1.0; break;
      case 4: PH_CALIBRATION_VALUE += 0.01; break;
    }
    tButton = millis();
  }
  if(isBtnDown() && (unsigned long) millis()-tButton >= TBUTTON){
    switch(menu){
      case 1: if(flowRef > MIN_FLOW_REF) flowRef -= 1.0; break;
      case 2: if(phRef > MIN_PH_REF) phRef -= 1.0; break;
//      case 3: if(temperatureMax > MIN_TEMP_REF) temperatureMax -= 1.0; break;
      case 3: if(humidityRef > MIN_HUMI_REF) humidityRef -= 1.0; break;
      case 4: PH_CALIBRATION_VALUE -= 0.01; break;
    }
    tButton = millis();
  }
  if(isBtnSelect() && (unsigned long) millis()-tButton >= TBUTTON){
    if(++menu > 4) menu = 0;
    tButton = millis();
  }
  /*
   * Cek kondisi sensor float
   */
  if(digitalRead(sensorBawah) == 1){
    digitalWrite(relayAir, RELAY_ON);
    digitalWrite(relayPupuk, RELAY_ON);
    pulse = 0; // setiap pembukaan pompa baru, reset nilai sensor flow ke 0
  }
  if(digitalRead(sensorAtas) == 0){
    digitalWrite(relayAir, RELAY_OFF); // pompa air dimatikan berdasarkan sensor atas
  }
  /*
   * Cek selalu kondisi apakah flow pupuk sudah sama atau malah lebih
   * kalau sudah sama atau lebih langsung matikan
   */
  flowAct = (1.0 * pulse / pulsePerLiter);
  if(flowAct >= flowRef){
    digitalWrite(relayPupuk, RELAY_OFF); // pompa pupuk dimatikan berdasarkan jumlah liter
  }
  /*
   * Cek nilai temperature dari sensor DHT 
   */
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
//  if(temperature > temperatureMax){
//    digitalWrite(relayFan, RELAY_ON);
//  } else {
//    digitalWrite(relayFan, RELAY_OFF);
//  }
  if(humidity > humidityRef){
    digitalWrite(relayFan, RELAY_ON);
  } else {
    digitalWrite(relayFan, RELAY_OFF);
  }
  /*
   * Cek nilai PH
   */
  // sampling
  int buffer_arr[10] = { 0 };
  int temp = 0;
  for(uint8_t i=0; i<10; i++){
    buffer_arr[i] = analogRead(phSensorPin);
    delay(30);
  }
  // sorting
  for(uint8_t i=0; i<9; i++){
    for(uint8_t j=0; j<10; j++){
      if(buffer_arr[i]>buffer_arr[j]){
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  // calculate average 6 centre analog values
  int avgval = 0;
  for(uint8_t i=0; i<8; i++){
    avgval += buffer_arr[i];
    phVoltage = (float) avgval* 5.0/1024/6;
    phValue = (-5.70 * phVoltage) + PH_CALIBRATION_VALUE;
  }
  if(phValue < phRef - 1) digitalWrite(relayPhUp, RELAY_ON);
  if(phValue >= phRef) digitalWrite(relayPhUp, RELAY_OFF);
  if(phValue > phRef + 1) digitalWrite(relayPhDown, RELAY_ON);
  if(phValue <= phRef) digitalWrite(relayPhDown, RELAY_OFF);
  /*
   * Tampilkan kepada operator
   * 
   * dan taruh status pupuk ke buzzer di bagian ini
   */
  if((unsigned long) millis()-tRefresh >= TREFRESH){
    Serial.println("Program berjalan");
    
    // jika sensor pupuk terdeteksi habis, bunyikan alarm melalui buzzer
    if(digitalRead(sensorPupuk) == 1){
      digitalWrite(buzzer, state);
      state = !state;
    } else {
      digitalWrite(buzzer, 0);
    }
    
    switch(menu){
      case 0: vMonitoring(); break;
      case 1: vSettingFlow(); break;
      case 2: vSettingPh(); break;
      case 3: vSettingHumi(); break;
      case 4: vKalibrasiPh(); break;
    } 
    tRefresh = millis();
  }
}

void vMonitoring(){
  dtostrf(flowAct,3,1,floatString);
  sprintf(txtBuffer, "Flow Counter:%s lt", floatString);
  lcdLine(1, txtBuffer);

  char humiStr[7] = {0};
  humidity = constrain(humidity,-999,9999);
  phValue = constrain(phValue,-99,999);
  
  dtostrf(humidity,4,1,humiStr);
  dtostrf(phValue,3,1,floatString);
  sprintf(txtBuffer, "Hum:%s ,PH:%s", humiStr, floatString);
  lcdLine(2, txtBuffer);
    
  sprintf(txtBuffer, "Pump1:%d ,Pump2:%d ", (digitalRead(relayAir) == RELAY_ON ? 1:0), (digitalRead(relayPupuk) == RELAY_ON ? 1:0));
  lcdLine(3, txtBuffer);
  
  sprintf(txtBuffer, "Ph(Up-Dw):%d-%d ,Fan:%d", (digitalRead(relayPhUp) == RELAY_ON ? 1:0), (digitalRead(relayPhDown) == RELAY_ON ? 1:0), (digitalRead(relayFan) == RELAY_ON ? 1:0));
  lcdLine(4, txtBuffer);
}

void vSettingFlow(){
  lcdLine(1, "   FLOW SETPOINT    ");
  
  dtostrf(flowRef,3,1,floatString);
  sprintf(txtBuffer, "Ltr: %s", floatString);
  lcdLine(2, txtBuffer);
  lcdLine(3, "                    ");
  lcdLine(4, "                    ");
}

void vSettingTemp(){
  lcdLine(1, " MAX-TEMP SETPOINT  ");

  dtostrf(temperatureMax,3,1,floatString);
  sprintf(txtBuffer, "Temp: %s", floatString);
  lcdLine(2, txtBuffer);
  lcdLine(3, "                    ");
  lcdLine(4, "                    ");
}

void vSettingHumi(){
  lcdLine(1, " MAX-HMDT SETPOINT  ");

  dtostrf(humidityRef,3,1,floatString);
  sprintf(txtBuffer, "Humi: %s", floatString);
  lcdLine(2, txtBuffer);
  lcdLine(3, "                    ");
  lcdLine(4, "                    ");
}

void vSettingPh(){
  lcdLine(1, "    PH SETPOINT     ");

  dtostrf(phRef,3,1,floatString);
  sprintf(txtBuffer, "PH: %s", floatString);
  lcdLine(2, txtBuffer);
  lcdLine(3, "                    ");
  lcdLine(4, "                    ");
}

void vKalibrasiPh(){
  lcdLine(1, "PH CALIBRATION VALUE");

  char f1[7];
  dtostrf(PH_CALIBRATION_VALUE,3,2,f1);
  sprintf(txtBuffer, "PCV: %s:", f1);
  lcdLine(2, txtBuffer);
  lcdLine(3, "  PH CURRENT VALUE  ");
  dtostrf(phValue,4,1,f1);
  sprintf(txtBuffer, "PH: %s", f1);
  lcdLine(4, txtBuffer);
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

void lcdLine(uint8_t line, char* text){
  lcd.setCursor(0, line-1);
  lcd.print("                    ");
  lcd.setCursor(0, line-1);
  lcd.print(text);
}
