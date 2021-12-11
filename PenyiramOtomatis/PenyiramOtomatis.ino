/*
 * Penyiram otomatis terjadwal, include library
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include "EEPROM.h"

// config sistem
#define BLYNK_PRINT Serial
#define EEPROM_SIZE 64
#define auth "sv3ykNMzo1dPfWkwG6yfD1W88HBOzalx"
#define ssid "duoandaresta"
#define pass "takokomamak"
#define RELAY_ON 0
#define RELAY_OFF !RELAY_ON

// input pin
#define soil1 36
#define soil2 39
#define soil3 34
#define btn1 32 
#define btn2 33
#define btn3 25
#define btn4 26

#define pompa1 16
#define pompa2 17
#define pompa3 18


// rtc object
RTC_DS3231 rtc;
// lcd object
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Datetime object
DateTime now;

/*
 * Global variable untuk menyimpan nilai 
 */
const String hari[7] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
// menyimpan jadwal 2 jam 2 menit
uint8_t jadwalJamMenit[2][2] = { 0 };
// meyimpan status mikro
String statusMikro = F("Mikro Berjalan");
// menyimpan nilai referensi
int rhref_min = 0;
int rhref_max = 0;
// flag untuk menyimpan perubahan nilai referensi
bool flagRhref_min = 0;
bool flagRhref_max = 0;
// menyimpan nilai sensor 
int soilValue1 = 0;
int soilValue2 = 0;
int soilValue3 = 0;
// id menu berguna untuk menu tampilan
uint8_t idMenu = 0;
// periodic update blynk dan button
unsigned long pushUpdate = 0;
unsigned long btnUpdate = 0;

void setup() {
  // inisialisasi pin
  pinMode(soil1, INPUT);
  pinMode(soil2, INPUT);
  pinMode(soil3, INPUT);
  pinMode(pompa1, OUTPUT);
  pinMode(pompa2, OUTPUT);
  pinMode(pompa3, OUTPUT);
  // kondisi pin default
  digitalWrite(pompa1, RELAY_OFF);
  digitalWrite(pompa2, RELAY_OFF);
  digitalWrite(pompa3, RELAY_OFF);
  // baudrate serial
  Serial.begin(115200);
  // inisialisasi lcd
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  // inisialisasi EEPROM
  if(!EEPROM.begin(EEPROM_SIZE)){
    Serial.println(F("Gagal menginisialisasi EEPROM"));
    lcdLine(1, F("EEPROM ERROR"));
    while(1); // jika gagal, berhenti disini loop terus
  }
  
  /*
   * Mengambil data jadwal dari eeprom agar tidak selalu set terus saat alat mati
   * address 0 = 8 bit data bisa digunakan untuk menyimpan data hari
   * address 1,2,3 sebanyak 24 bit bisa digunakan untuk menyimpat 2 jam, 2 menit
   * yang dibagi ke 6 bit masing masing
   * 
   * address 4,5 untuk menyimpan rh ref min
   * address 6,7 untuk menyimpan rh ref max
   */  
  uint32_t temp = 0x00;
  temp = EEPROM.read(1);
  temp = temp << 8;
  temp = temp | EEPROM.read(2);
  temp = temp << 8;
  temp = temp | EEPROM.read(3);

  jadwalJamMenit[0][0] = ( temp & 0b111111000000000000000000) >> 18; // jam 1
  jadwalJamMenit[0][1] = ( temp & 0b000000111111000000000000) >> 12; // menit 1
  jadwalJamMenit[1][0] = ( temp & 0b000000000000111111000000) >> 6; // jam 2
  jadwalJamMenit[1][1] = ( temp & 0b000000000000000000111111); // menit 2

  uint16_t temp2 = 0x00;
  temp2 = EEPROM.read(4);
  temp2 = temp2 << 8;
  temp2 = temp2 | EEPROM.read(5);
  rhref_min = temp2;

  temp2 = 0x00;
  temp2 = EEPROM.read(6);
  temp2 = temp2 << 8;
  temp2 = temp2 | EEPROM.read(7);
  rhref_max = temp2;
  
  Serial.println(F("Mendapat jadwal"));
  Serial.println(jadwalJamMenit[0][0], DEC);
  Serial.println(jadwalJamMenit[0][1], DEC);
  Serial.println(jadwalJamMenit[1][0], DEC);
  Serial.println(jadwalJamMenit[1][1], DEC);
  Serial.println(F("Mendapat nilai referensi"));
  Serial.print(F("Rh Ref Min: "));
  Serial.println(rhref_min);
  Serial.print(F("Rh Ref Max: "));
  Serial.println(rhref_max);

  // cek inisialisasi RTC
  if(!rtc.begin()){
    Serial.println(F("Tidak menemukan modul RTC"));
    lcdLine(1, F("RTC ERROR!!!"));
    while(1);
  }
  // cek baterai rtc apakah pernah di lepas
  if(rtc.lostPower()){
    Serial.println(F("RTC kehilangan tegangan, kita set waktunya sesuai dengan waktu sekarang"));
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
  // inisialisasi blynk agar konek ke wifi dan ke auth app
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  /*
   * Cek pemencetan tombol 1, dengan debounce setiap 500 ms
   * 0. kodisi di menu utama menyalakan pompa secara manual
   * 1. mengurangi referensi rh min 
   * 2. menambahi referensi rh max
   */
  if(isBtn1() && (unsigned long) millis()-btnUpdate >= 500){
    switch(idMenu){
      case 0: digitalWrite(pompa1, RELAY_ON); btnUpdate = millis(); return;
      case 1: rhref_min -= 1; flagRhref_min = 1; btnUpdate = millis(); break;
      case 2: rhref_max -= 1; flagRhref_max = 1; btnUpdate = millis(); break;
    }
  }
  if(isBtn2() && (unsigned long) millis()-btnUpdate >= 500){
    switch(idMenu){
      case 0: digitalWrite(pompa2, RELAY_ON); btnUpdate = millis(); return;
      case 1: rhref_min += 1; flagRhref_min = 1; btnUpdate = millis(); break;
      case 2: rhref_max += 1; flagRhref_max = 1; btnUpdate = millis(); break;
    }
  }
  if(isBtn3() && (unsigned long) millis()-btnUpdate >= 500){
    switch(idMenu){
      case 0: digitalWrite(pompa3, RELAY_ON); btnUpdate = millis(); return;
    }
  }
  if(isBtn4() && (unsigned long) millis()-btnUpdate >= 500){
    if(++idMenu > 2) idMenu = 0;
  }

  /*
   * Cek flag menyimpan nilai referensi ke eeprom, perlu di flag karena pengaturan
   * nilai referensi dari 2 cara , yaitu blynk dan button sehingga meminimalkan 
   * penulisan ke eeprom saat nilai referensi sudah fix di akhir / button selesai
   * ditekan.
   */
  if(flagRhref_min == 1 && (unsigned long) millis()-btnUpdate >= 3000){
    flagRhref_min = 0;
    simpanRhref_min();
    btnUpdate = millis();
  }
  if(flagRhref_max == 1 && (unsigned long) millis()-btnUpdate >= 3000){
    flagRhref_max = 0;
    simpanRhref_max();
    btnUpdate = millis();
  }

  /*
   * Periodic push data ke blynk dan tampilan lcd
   */
  if((unsigned long) millis()-pushUpdate >= 1000){
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
    
    // ambil nilai terbaru masing masing soil
    soilValue1 = getSoilValue(soil1);
    soilValue2 = getSoilValue(soil2);
    soilValue3 = getSoilValue(soil3);
    
    // cek jadwal pertama sesuai jam dan menit nya dan tidak 0 semua
    if(jadwalJamMenit[0][0] == now.hour() && jadwalJamMenit[0][1] == now.minute()
    && jadwalJamMenit[0][0] != 0 && jadwalJamMenit[0][1] != 0){
        statusMikro = F("Jadwal 1 dicapai");
        // nilai soil masing masing jika dibawah referensi min, relay masing masing akan on
        if(soilValue1 <= rhref_min) digitalWrite(pompa1, RELAY_ON);
        if(soilValue2 <= rhref_min) digitalWrite(pompa2, RELAY_ON);
        if(soilValue3 <= rhref_min) digitalWrite(pompa3, RELAY_ON);
    }
    // cek jadwal kedua
    if(jadwalJamMenit[1][0] == now.hour() && jadwalJamMenit[1][1] == now.minute()
    && jadwalJamMenit[0][0] != 0 && jadwalJamMenit[0][1] != 0){
        statusMikro = F("Jadwal 2 dicapai");
        if(soilValue1 <= rhref_min) digitalWrite(pompa1, RELAY_ON);
        if(soilValue2 <= rhref_min) digitalWrite(pompa2, RELAY_ON);
        if(soilValue3 <= rhref_min) digitalWrite(pompa3, RELAY_ON);
    }
    
    // cek kapanpun jika nilai kelembapan sudah diatas maximal matikan relay
    if(soilValue1 >= rhref_max) digitalWrite(pompa1, RELAY_OFF);
    if(soilValue2 >= rhref_max) digitalWrite(pompa2, RELAY_OFF);
    if(soilValue3 >= rhref_max) digitalWrite(pompa3, RELAY_OFF);

    Serial.println(F("Kirim data ke blynk"));
    // kirim data ke blynk
    int jadwal1 = (jadwalJamMenit[0][0]*60*60) + (jadwalJamMenit[0][1]*60);
    int jadwal2 = (jadwalJamMenit[1][0]*60*60) + (jadwalJamMenit[1][1]*60);
    char tz[] = "Asia/Jakarta";
    Blynk.virtualWrite(V0, jadwal1, 0, tz);
    Blynk.virtualWrite(V1, jadwal2, 0, tz);
    Blynk.virtualWrite(V10, statusMikro);
    Blynk.virtualWrite(V11, soilValue1);
    Blynk.virtualWrite(V12, soilValue2);
    Blynk.virtualWrite(V13, soilValue3);
    Blynk.virtualWrite(V14, rhref_min);
    Blynk.virtualWrite(V15, rhref_max);
    Serial.println(F("Selesai kirim data ke blynk"));

    switch(idMenu){
      case 0: lcdUtama(); break;
      case 1: lcdRhref_min(); break;
      case 2: lcdRhref_max(); break;
    }
    
    pushUpdate = millis();
  }
}

#define BUFFER_LENGTH 17
void lcdUtama(){
  char buff[BUFFER_LENGTH] = {0};
  sprintf(buff,"S1:%d S2:%d",soilValue1,soilValue2);
  lcdLine(1,buff);
  sprintf(buff,"S3:%d ",soilValue3);
  lcdLine(2,buff);
}

void lcdRhref_min(){
  lcdLine(1, F("RH REF MINIMUM"));
  char buff[BUFFER_LENGTH] = {0};
  sprintf(buff,"-> %d",rhref_min);
  lcdLine(2, buff);
}

void lcdRhref_max(){
  lcdLine(1, F("RH REF MAXIMUM"));
  char buff[BUFFER_LENGTH] = {0};
  sprintf(buff,"-> %d",rhref_max);
  lcdLine(2, buff);
}

#define MAX_DETECT 20
#define MIN_DETECT 15
bool isBtn1(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btn1) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
bool isBtn2(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btn2) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
bool isBtn3(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btn3) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
bool isBtn4(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btn4) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}

void simpanJadwal(){
  Serial.println(F("Simpan jadwal ke EEPROM"));
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
  Serial.println(F("Commit"));
  EEPROM.commit();
}

void simpanRhref_min(){
  uint16_t temp = 0x00;
  temp = rhref_min;

  EEPROM.write(4, (temp & 0b1111111100000000) >> 8);
  EEPROM.write(5, (temp & 0b0000000011111111));
  EEPROM.commit();

  statusMikro = F("Menyimpan rh min");
  lcdLine(1, statusMikro);
}

void simpanRhref_max(){
  uint16_t temp = 0x00;
  temp = rhref_max;

  EEPROM.write(6, (temp & 0b1111111100000000) >> 8);
  EEPROM.write(7, (temp & 0b0000000011111111));
  EEPROM.commit();

  statusMikro = F("Menyimpan rh max");
  lcdLine(1, statusMikro);
}

BLYNK_WRITE(V0){
  TimeInputParam t(param);
  if(t.hasStartTime()){
    jadwalJamMenit[0][0] = t.getStartHour();
    jadwalJamMenit[0][1] = t.getStartMinute();
    statusMikro = (String) "Dapat input jadwal 1 " + 
                  jadwalJamMenit[0][0] + ":" +
                  jadwalJamMenit[0][1];
    simpanJadwal();
  }
  Serial.println(statusMikro);
}

BLYNK_WRITE(V1){
  TimeInputParam t(param);
  if (t.hasStartTime()){
    jadwalJamMenit[1][0] = t.getStartHour();
    jadwalJamMenit[1][1] = t.getStartMinute();
    statusMikro = (String) "Dapat input jadwal 2 " + 
                  jadwalJamMenit[1][0] + ":" +
                  jadwalJamMenit[1][1];
    simpanJadwal();
  }
  Serial.println(statusMikro);
}

BLYNK_WRITE(V2){
  rhref_min = param.asInt();
  statusMikro = (String) "Dapat input setpoint rh min: " + rhref_min;
  Serial.println(statusMikro);
  flagRhref_min = 1;
  btnUpdate = millis();
}

BLYNK_WRITE(V3){
  rhref_max = param.asInt();
  statusMikro = (String) "Dapat input setpoint rh max: " + rhref_max;
  Serial.println(statusMikro);
  flagRhref_max = 1;
  btnUpdate = millis();
}

/*
 * keluarannya 0-100 persen
 */
int getSoilValue(int pin){
  return (100 - ((analogRead(pin)/4096.0) * 100.0));
}

void lcdLine(uint8_t line, String text){
  lcd.setCursor(0, line-1);
  lcd.print("                ");
  lcd.setCursor(0, line-1);
  lcd.print(text);
}
