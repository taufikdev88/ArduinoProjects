/*
 * Fish Feeder Semi-Auto, PH Controlled
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include "EEPROM.h"
#include <Servo.h>

// config
#define BLYNK_PRINT Serial
#define EEPROM_SIZE 64
#define auth "59CLK7MCiYHcbRRcVqtcU5A38GxWT3or"
#define ssid "Crawlersharp"
#define pass "palembangemas"
#define RELAY_ON 0
#define RELAY_OFF !RELAY_ON
#define OPEN_ANGLE 90
#define CLOSE_ANGLE 0
#define DELAY_OPEN_FEEDER 1000

float PH_CALIBRATION_VALUE = 21.34;

#define MAX_PH_REF 100
#define MIN_PH_REF 0
#define MAX_TB_REF 3000
#define MIN_TB_REF 0

// input
#define phSensorPin 32
#define tbSensorPin 33
#define servoPin 13
#define btnUp 14
#define btnDown 15
#define btnSelect 16
#define btnFeed 17
#define floatDown 18
#define floatUp 19

// output
#define relayPhUp 21
#define relayPhDown 22
#define relayKuras 23
#define relayIsi 25

// Timer object
BlynkTimer timer;
// rtc object
RTC_DS3231 rtc;
// lcd object
LiquidCrystal_I2C lcd(0x27, 16, 2);
// servo object
Servo servo;

// global variable
const String hari[7] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
uint8_t jadwalJamMenit[2][2] = { 0 };
String statusMikro = "Mikro Berjalan";
float phVoltage;
float phRef = 70;
float tbRef = 90;
float phAct = 80;
float tbAct = 60;
uint8_t idMenu = 0;
bool flagKuras = 0;
bool flagTbRef = 0;
bool flagPhRef = 0;
bool flagFeed1 = 0;
bool flagFeed2 = 0;
int8_t nFeed1 = 0;
int8_t nFeed2 = 0;
unsigned long btnUpdate = 0;
unsigned long lcdUpdate = 0;
DateTime now;

// declared function
void feed(){
  servo.write(OPEN_ANGLE);
  delay(DELAY_OPEN_FEEDER);
  servo.write(CLOSE_ANGLE);
}

float getTurbidity(int x){
  Serial.println((String) "nilai analog: " + x);
  x = 3.3 / 4096.0 * x;
  Serial.println((String) "nilai tegangan: " + x);
  // based on tutorial https://wiki.dfrobot.com/Turbidity_sensor_SKU__SEN0189#:~:text=Turbidity%20sensors%20are%20used%20to,and%20digital%20signal%20output%20modes.
  if(x < 2.5) return 3000;
  else return (-1120.4*x*x + 5742.3*x - 4353.8);
}

void updateData(){
  // update tanggal dan waktu dari modul rtc
  now = rtc.now();
  // print ke Serial
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(hari[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.println(F("Cek jadwal"));
  // cek jadwal memberi makan, jika jadwal 00:00 maka dianggap off
  if(jadwalJamMenit[0][0] == now.hour() && jadwalJamMenit[0][1] == now.minute()
   && jadwalJamMenit[0][0] != 0 && jadwalJamMenit[0][1] != 0 && !flagFeed1){
    if(++nFeed1 > 1) flagFeed1 = 1;
    feed();
    statusMikro = "Memberi makan sesuai jadwal 1";
  } else 
  if(jadwalJamMenit[0][0] != now.hour() && jadwalJamMenit[0][1] != now.minute() && flagFeed1){
    flagFeed1 = 0;
    nFeed1 = 0;
  }
  
  if(jadwalJamMenit[1][0] == now.hour() && jadwalJamMenit[1][1] == now.minute()
   && jadwalJamMenit[1][0] != 0 && jadwalJamMenit[1][1] != 0 && !flagFeed2){
    if(++nFeed2 > 1) flagFeed2 = 1;
    feed();
    statusMikro = "Memberi makan sesuai jadwal 2";
  } else
  if(jadwalJamMenit[1][0] != now.hour() && jadwalJamMenit[1][1] != now.minute() && flagFeed2){
    flagFeed2 = 0;
    nFeed2 = 0;
  }
  
  // cek ph
  // sampling
//  Serial.println(F("Sampling"));
//  int buffer_arr[10] = {0};
//  int temp = 0;
//  for(uint8_t i=0; i<10; i++){
//    buffer_arr[i] = map(analogRead(phSensorPin), 0, 1023, 0, 5000)/1000; // mapping 3.3v ke 5.0v
//    delay(30);
//  }
//  Serial.println(F("Sorting"));
//  // sorting
//  for(uint8_t i=0; i<9; i++){
//    for(uint8_t j=0; i<10; i++){
//      if(buffer_arr[i]>buffer_arr[j]){
//        temp = buffer_arr[i];
//        buffer_arr[i] = buffer_arr[j];
//        buffer_arr[j] = temp;
//      }
//    }
//  }
//  Serial.println(F("Calculate average"));
//  // calculate average 6 centre analog
//  int avgval = 0;
//  for(uint8_t i=2; i<8; i++){
//    avgval += buffer_arr[i];
//    phVoltage = (float) avgval*5.0/1024/6;
//    phAct = (-5.70 * phVoltage) + PH_CALIBRATION_VALUE;
//  }
  
  int nilaiAnalog = analogRead(phSensorPin);
  float teganganPh = 3.3 / 4096.0 * nilaiAnalog;
  float ph4 = 1.63;
  float ph7 = 1.44;
  float ph_step = (ph4 - ph7) / 3;
  phAct = 7.00 + ((ph7-teganganPh) / ph_step);
  
  if(phAct < phRef -1) digitalWrite(relayPhUp, RELAY_ON);
  if(phAct >= phRef) digitalWrite(relayPhUp, RELAY_OFF);
  if(phAct > phRef +1) digitalWrite(relayPhDown, RELAY_ON);
  if(phAct <= phRef) digitalWrite(relayPhDown, RELAY_OFF);
  
  // cek turbidity sensor
  Serial.println(F("Cek turbidity sensor"));
  tbAct = getTurbidity(analogRead(tbSensorPin));
  if(tbAct > tbRef && !flagKuras){
    flagKuras = 1;
    digitalWrite(relayIsi, RELAY_OFF);
    digitalWrite(relayKuras, RELAY_ON);
  }
  if(digitalRead(floatDown) == 1){
    flagKuras = 0;
    digitalWrite(relayKuras, RELAY_OFF);
    digitalWrite(relayIsi, RELAY_ON);
  }
  if(digitalRead(floatUp) == 0){
    digitalWrite(relayIsi, RELAY_OFF);
  }


  Serial.println(F("Kirim data ke blynk"));
  // kirim data ke blynkget
  int jadwal1 = (jadwalJamMenit[0][0]*60*60) + (jadwalJamMenit[0][1]*60);
  int jadwal2 = (jadwalJamMenit[1][0]*60*60) + (jadwalJamMenit[1][1]*60);
  char tz[] = "Asia/Jakarta";
  Blynk.virtualWrite(V0, statusMikro);
  Blynk.virtualWrite(V1, jadwal1, 0, tz);
  Blynk.virtualWrite(V2, jadwal2, 0, tz);
  Blynk.virtualWrite(V6, phAct);
  Blynk.virtualWrite(V7, tbAct);
  Blynk.virtualWrite(V8, PH_CALIBRATION_VALUE);
  Serial.println(F("Selesai kirim data ke blynk"));
}

void saveToEeprom(){
  Serial.println("Saving to EEPROM");
  uint32_t temp = 0x00;
  temp = temp | jadwalJamMenit[0][0];
  temp = temp << 6;
  temp = temp | jadwalJamMenit[0][1];
  temp = temp << 6;
  temp = temp | jadwalJamMenit[1][0];
  temp = temp << 6;
  temp = temp | jadwalJamMenit[1][1];
  EEPROM.write(1, (temp & 0b111111110000000000000000) >> 16);
  EEPROM.write(2, (temp & 0b000000001111111100000000) >> 8);
  EEPROM.write(3, (temp & 0b000000000000000011111111));
  Serial.println("Commit");
  EEPROM.commit();  
}

BLYNK_WRITE(V1){
  TimeInputParam t(param);
  if (t.hasStartTime()){
    jadwalJamMenit[0][0] = t.getStartHour();
    jadwalJamMenit[0][1] = t.getStartMinute();
    statusMikro = (String) "Dapat input jadwal 1 " + 
                  jadwalJamMenit[0][0] + ":" +
                  jadwalJamMenit[0][1];
    saveToEeprom();
  }
  Serial.println(statusMikro);
}

BLYNK_WRITE(V2){
  TimeInputParam t(param);
  if (t.hasStartTime()){
    jadwalJamMenit[1][0] = t.getStartHour();
    jadwalJamMenit[1][1] = t.getStartMinute();
    statusMikro = (String) "Dapat input jadwal 2 " + 
                  jadwalJamMenit[1][0] + ":" +
                  jadwalJamMenit[1][1];
    saveToEeprom();
  }
  Serial.println(statusMikro);
}

BLYNK_WRITE(V3){
  Serial.println(param.asInt());
  if(param.asInt() == 1){
    statusMikro = "Membuka pakan";
    feed();
  } else {
    statusMikro = "Menutup pakan";  
  }
}

BLYNK_WRITE(V4){
  phRef = param.asInt();
  statusMikro = (String) "Dapat input setpoint ph: " + phRef;
  Serial.println(statusMikro);
  flagPhRef = 1;
}

BLYNK_WRITE(V5){
  tbRef = param.asInt();
  statusMikro = (String) "Dapat input setpoint turbidity: " + tbRef;
  Serial.println(statusMikro);
  flagTbRef = 1;
}

BLYNK_WRITE(V9){
  String v = param.asString();
  statusMikro = "Dapat input calibration value " + v;
  Serial.println(statusMikro);
  PH_CALIBRATION_VALUE = v.toFloat();
}

void lcdLine(uint8_t l, String t){
  l = l-1;
  lcd.setCursor(0,l);
  lcd.print("                ");
  lcd.setCursor(0,l);
  lcd.print(t);
}

void utama(){ // id menu 0
//  // tampilkan ke lcd 20x4
//  char buff[16] = { 0 };
//  sprintf(buff, "%s, %d:%d", hari[now.dayOfTheWeek()], now.hour(), now.minute());
//  lcdLine(1, buff);
//
//  tbAct = constrain(tbAct, -99.0, 100.0);
//  phAct = constrain(phAct, -99.0, 100.0);
//  char f1[6] = {0};
//  char f2[6] = {0};
//  dtostrf(tbAct,3,1,f1);
//  dtostrf(phAct,3,1,f2);
//  sprintf(buff,"P:%s,T:%s",f1,f2);
//  lcdLine(2, buff);
  char buff[17] = {0};
  char phStr[10] = {0};
  char tbStr[10] = {0};
  dtostrf(tbAct,7,2,tbStr);
  dtostrf(phAct,7,2,phStr);
  sprintf(buff,"PH: %s",phStr);
  lcdLine(1,buff);
  sprintf(buff,"TB: %s",tbStr);
  lcdLine(2,buff);
}
void setPH(){ // id menu 1
  lcdLine(1, "  SETPOINT PH   ");
  char floatStr[10];
  dtostrf(phRef,7,2,floatStr);
  lcdLine(2, floatStr);
}
void setTB(){ // id menu 2
  lcdLine(1, "  SETPOINT TB   ");
  char floatStr[10];
  dtostrf(tbRef,7,2,floatStr);
  lcdLine(2, floatStr);
}
void kurasMan(){ // id menu 3
  lcdLine(1, "  KURAS MANUAL  ");
  lcdLine(2, "(f)ok  (s)cancel");
}

void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.clear();
  lcd.backlight();

  if(!EEPROM.begin(EEPROM_SIZE)){
    Serial.println("Gagal menginisialisasi EEPROM");
    lcdLine(1, "EEPROM ERROR!");
    while(1);
  }

  /*
   * Mengambil data jadwal dari eeprom agar tidak selalu set terus saat alat mati
   * address 0 = 8 bit data bisa digunakan untuk menyimpan data hari
   * address 1,2,3 sebanyak 24 bit bisa digunakan untuk menyimpat 2 jam, 2 menit
   * yang dibagi ke 6 bit masing masing
   * 
   * address 4,5 untuk menyimpan ph ref
   * address 6,7 untuk menyimpan tb ref
   */  
  uint32_t temp = 0x00; // penyiapan penampung data eeprom untuk masking dan shifting
  temp = EEPROM.read(1); // pengambilan nilai eeprom di address 1 8 bit awal  
  temp = temp << 8; // shifting 8 bit ke kiri
  temp = temp | EEPROM.read(2); // pengambilan dan penggabungan nilai eeprom di address 2 8 bit selanjutnya
  temp = temp << 8; // shifting 8 bit ke kiri
  temp = temp | EEPROM.read(3); // pengambilan dan penggabungan nilai eeprom di address 3
  
  uint16_t temp2 = 0x00;
  temp2 = EEPROM.read(4);
  temp2 = temp2 << 8;
  temp2 = temp2 | EEPROM.read(5);
  phRef = temp2;

  temp2 = 0x00;
  temp2 = EEPROM.read(6);
  temp2 = temp2 << 8;
  temp2 = temp2 | EEPROM.read(7);
  tbRef = temp2;
  
  jadwalJamMenit[0][0] = ( temp & 0b111111000000000000000000) >> 18; // jam 1
  jadwalJamMenit[0][1] = ( temp & 0b000000111111000000000000) >> 12; // menit 1
  jadwalJamMenit[1][0] = ( temp & 0b000000000000111111000000) >> 6; // jam 2
  jadwalJamMenit[1][1] = ( temp & 0b000000000000000000111111); // menit 2

  Serial.println("Mendapat jadwal");
  Serial.println(jadwalJamMenit[0][0], DEC);
  Serial.println(jadwalJamMenit[0][1], DEC);
  Serial.println(jadwalJamMenit[1][0], DEC);
  Serial.println(jadwalJamMenit[1][1], DEC);
  Serial.println("Mendapat nilai referensi");
  Serial.print("TB Ref: ");
  Serial.println(tbRef);
  Serial.print("PH Ref: ");
  Serial.println(phRef);
  
  if(!rtc.begin()){
    Serial.println("Tidak menemukan modul RTC");
    lcdLine(1, "RTC ERROR!");
    while(1);
  }
  if(rtc.lostPower()){
    Serial.println("RTC kehilangan tegangan, kita set waktunya sesuai dengan waktu sekarang");
    /*
     * Saat modul rtc butuh di masukkan ke perangkat ygbaru, atau ada kehilangan tegangan,
     * fungsi ini akan mengeset data sesuai dengan tanggal dan waktu saat koding ini di compile
     */
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    /*
     * Jika ingin set data tanggal dan jam sesuai yang kamu inginkan, secara ekplisit
     * jalankan perintah dibawah ini, katakan ingin set tanggal ke 
     * Januari 21, 2014 jam 3 pagi
     * rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
     */
  }

  servo.attach(servoPin);
  servo.write(CLOSE_ANGLE);

  pinMode(phSensorPin,INPUT);
  pinMode(tbSensorPin, INPUT);
  
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);
  pinMode(btnFeed, INPUT_PULLUP);
  pinMode(floatUp, INPUT_PULLUP);
  pinMode(floatDown, INPUT_PULLUP);

  pinMode(relayPhUp, OUTPUT);
  pinMode(relayPhDown, OUTPUT);
  pinMode(relayKuras, OUTPUT);
  pinMode(relayIsi, OUTPUT);

  digitalWrite(relayPhUp, RELAY_OFF);
  digitalWrite(relayPhDown, RELAY_OFF);
  digitalWrite(relayKuras, RELAY_OFF);
  digitalWrite(relayIsi, RELAY_OFF);
 
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, updateData);
}

void loop() {
  Blynk.run();
  timer.run();
  
  if((unsigned long) millis()-btnUpdate >= 10000 && flagPhRef){
    flagPhRef = 0;
    
    uint16_t temp = 0x00;
    temp = phRef;
    
    EEPROM.write(4, (temp & 0b1111111100000000) >> 8);
    EEPROM.write(5, (temp & 0b0000000011111111));
    EEPROM.commit();
    
    statusMikro = "Menyimpan PH Ref";
    lcd.clear();
    lcdLine(1, statusMikro);
    lcdUpdate = millis();
  }
  if((unsigned long) millis()-btnUpdate >= 10000 && flagTbRef){
    flagTbRef = 0;
    
    uint16_t temp = 0x00;
    temp = tbRef;
    
    EEPROM.write(6, (temp & 0b1111111100000000) >> 8);
    EEPROM.write(7, (temp & 0b0000000011111111));
    EEPROM.commit();
    
    statusMikro = "Menyimpan TB Ref";
    lcd.clear();
    lcdLine(1, statusMikro);
    lcdUpdate = millis();
  }

//  return;
  if(isBtnSelect() && (unsigned long) millis()-btnUpdate >= 1000){
    btnUpdate = millis();
    ++idMenu;
  }
  if(isBtnUp() && (unsigned long) millis()-btnUpdate >= 1000){
    btnUpdate = millis();
    switch(idMenu){
      case 1: if(phRef < MAX_PH_REF) ++phRef; flagPhRef = 1; break;
      case 2: if(tbRef < MAX_TB_REF) tbRef += 10; flagTbRef = 1; break;
    }
  }
  if(isBtnDown() && (unsigned long) millis()-btnUpdate >= 1000){
    btnUpdate = millis();
    switch(idMenu){
      case 1: if(phRef > MIN_PH_REF) --phRef; flagPhRef = 1; break;
      case 2: if(tbRef > MIN_TB_REF) tbRef -= 10; flagTbRef = 1; break;
    }
  }
  if(isBtnFeed() && (unsigned long) millis()-btnUpdate >= 1000){
    btnUpdate = millis();
    switch(idMenu){
      case 0: feed(); break;
      case 3: 
      digitalWrite(relayIsi, RELAY_OFF);
      digitalWrite(relayKuras, RELAY_ON);
      break;
    }
  }

//  return;
  if((unsigned long) millis()-lcdUpdate >= 500){
    lcdUpdate = millis();

    switch(idMenu){
      case 0: utama(); break;
      case 1: setPH(); break;
      case 2: setTB(); break;
      case 3: kurasMan(); break;
      default: idMenu = 0; break;
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
bool isBtnFeed(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnFeed) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
