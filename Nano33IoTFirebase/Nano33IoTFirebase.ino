/*
 * Menambahkan header libray ke file, agar kita gampang untuk mengatur hardware dengan
 * fungsi-fungsi yang telah disediakan
 * 
 * 1. firebase -> untuk koneksi wifi dan koneksi ke firebase google
 * 2. DHT -> untuk pengolahan data yang didapat dari sensor dht
 * 4. Wire -> untuk kebutuhan komunikasi I2C 
 * 5. LiquiedCrustal_I2C -> untuk mengakses lcd i2c
 */
#include "Firebase_Arduino_WiFiNINA.h"
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/*
 * Wifi & Firebase config
 */
#define FIREBASE_HOST "mysmarthome-822fd-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Ueu6yR162x8BmhEtP8bS7RfgmuMEoaUJspIBlKof"
#define WIFI_SSID "YOUR_WIFI_AP"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

/*
 * Global Variable , untuk menyimpan nilai yang diperlukan
 * 1. sendDataPrevMillis -> untuk keperluan periodic send data ke firebase
 * 2. humidity -> untuk menampung data humidity
 * 3. temperature -> untuk menampung data temperature
 * 4. iotEnable -> untuk keperluan alat agar menggunakan mode auto ngambil dari internet
 */
unsigned long sendDataPrevMillis = 0;
float humidity = 0.0;
float temperature = 0.0;
bool iotEnable = true; // definisikan sekalian saat pertama kali menyala mode lgsg IoT

/*
 * Pinout Arduino Nano 33 IoT 
 */
#define relay1 3
#define relay2 4
#define relay3 5
#define relay4 6
#define dhtPin 2

#define btn1 7
#define btn2 8
#define btn3 9
#define btn4 10
#define btnMode 11

/*
 * Config Default program
 */
#define RELAY_ON 0
#define RELAY_OFF !RELAY_ON
#define DHTTYPE DHT22

/*
 * Objek dari library yang kita gunakan untuk mengakses masing masing hardware
 */
FirebaseData firebaseData;
DHT dht(dhtPin, DHTTYPE); 
LiquidCrystal_I2C lcd(0x27, 16, 2); // alamat i2c di 0x27 lebar 16 karakter tinggi 2 karakter

/*
 * Fungsi setup, fungsi yang harus ada
 * fungsi ini akan dijalankan 1 kali saat arduino baru pertama kali menyala
 */
void setup(){
  // definisikan pin relay sebagai output semua
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  // nyatakan kondisi pertama masing masing relay pada kondisi off
  digitalWrite(relay1, RELAY_OFF);
  digitalWrite(relay2, RELAY_OFF);
  digitalWrite(relay3, RELAY_OFF);
  digitalWrite(relay4, RELAY_OFF);
  // definisikan pin button sebagai input pullup
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(btnMode, INPUT_PULLUP);
  // inisialisasi sensor DHT
  dht.begin();
  // mulai komunikasi serial di baudrate 115200 untuk keperluan debugging
  Serial.begin(115200);
  // inisialisasi lcd dan nyalakan led backlight nya
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  // mulai koneksi wifi
  lcdLine(1, "Connecting to WiFi");
  Serial.print(F("Connecting to WiFi"));
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED){
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(F("."));
    delay(300);
  }
  Serial.println();
  Serial.print(F("Connected with IP: "));
  Serial.println(WiFi.localIP());
  Serial.println();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
}

/*
 * Fungsi loop, fungsi yang harus ada
 * fungsi ini lah yang akan dijalankan secara terus menerus oleh arduino
 */
void loop(){
  if(isBtnMode()){
    // jika btn mode ditekan dan ditahan
    while(isBtnMode());
    // jika sudah dilepaskan
    if(iotEnable){
      // jika sebelumnya mode nya aktif, set wifi reconnect ke false
      Firebase.reconnectWiFi(false);
    } else {
      // jika sebelumnya nonaktif, set wifi reconnect ke true
      Firebase.reconnectWiFi(true);
    }
    iotEnable = !iotEnable;
  }

  if(isBtn1()){
    while(isBtn1());
    if(iotEnable){
      sinyalError();
    } else {
      digitalWrite(relay1, !digitalRead(relay1)); // set relay1 ke kebalikannya yg sekarang
    }
  }

  if(isBtn2()){
    while(isBtn2());
    if(iotEnable){
      sinyalError();
    } else {
      digitalWrite(relay2, !digitalRead(relay2));
    }
  }

  if(isBtn3()){
    while(isBtn3());
    if(iotEnable){
      sinyalError();
    } else {
      digitalWrite(relay3, !digitalRead(relay3));
    }
  }

  if(isBtn4()){
    while(isBtn4());
    if(iotEnable){
      sinyalError();
    } else {
      digitalWrite(relay4, !digitalRead(relay4));
    }
  }
  
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  // routine kirim data ke firebase
  if((unsigned long) millis() - sendDataPrevMillis > 2000){
    sendDataPrevMillis = millis();

    // kirim hanya jika mode iot diaktifkan
    if(iotEnable){
      // kirim data kelembapan
      if(!Firebase.setFloat(firebaseData, "/kelembapan", humidity)){
        Serial.println(F("-----Gagal Kirim Kelembapan-----"));
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println(F("--------------------------------"));
        Serial.println();
      }
  
      // kirim data suhu
      if(!Firebase.setFloat(firebaseData, "/suhu", temperature)){
        Serial.println(F("------Gagal Kirim Suhu----------"));
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println(F("--------------------------------"));
        Serial.println();
      } 
    }

    // siapkan variable untuk menampung nilai float untuk ditampilkan di lcd
    char f1[10] = {0};
    char f2[10] = {0};
    // masukkan nilai float ke penampung
    dtostrf(humidity,7,2,f1); // 7 angka didepan koma, 2 angka dibelakang koma
    dtostrf(temperature,7,2,f2);
    // siapkan variable untuk menampung karakter kata
    char text[17] = {0};
    // masukkan ke variable
    sprintf(text,"RH:%s ,TP:%s",f1,f2);
    // tampilkan ke lcd atas
    lcdLine(1, text);
    lcdLine(2, (iotEnable ? F("--IoT Enabled---"):F("--IoT Disabled--")));
  }

  // cek hanya jika mode iot di aktifkan
  if(iotEnable){
    // cek data relay1
    if(Firebase.getInt(firebaseData, "/relay1")){
      if(firebaseData.intData() == 1){
        digitalWrite(relay1, RELAY_ON);
      } else {
        digitalWrite(relay1, RELAY_OFF);
      }
    } else {
      Serial.println(F("-------Gagal Baca Relay1--------"));
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println(F("--------------------------------"));
      Serial.println();
    }
  
    // cek data relay2
    if(Firebase.getInt(firebaseData, "/relay2")){
      if(firebaseData.intData() == 1){
        digitalWrite(relay2, RELAY_ON);
      } else {
        digitalWrite(relay2, RELAY_OFF);
      }
    } else {
      Serial.println(F("-------Gagal Baca Relay2--------"));
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println(F("--------------------------------"));
      Serial.println();
    }
    
    // cek data relay3
    if(Firebase.getInt(firebaseData, "/relay3")){
      if(firebaseData.intData() == 1){
        digitalWrite(relay3, RELAY_ON);
      } else {
        digitalWrite(relay3, RELAY_OFF);
      }
    } else {
      Serial.println(F("-------Gagal Baca Relay3--------"));
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println(F("--------------------------------"));
      Serial.println();
    }
    
    // cek data relay4
    if(Firebase.getInt(firebaseData, "/relay4")){
      if(firebaseData.intData() == 1){
        digitalWrite(relay4, RELAY_ON);
      } else {
        digitalWrite(relay4, RELAY_OFF);
      }
    } else {
      Serial.println(F("-------Gagal Baca Relay4--------"));
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println(F("--------------------------------"));
      Serial.println();
    }
  }
}

/*
 * Fungsi sinyal error, untuk menandai kalau button1-4 dipencet dimode yang tidak seharusnya
 */
void sinyalError(){
  lcd.noBacklight();
  delay(500);
  lcd.backlight();
}

/*
 * Fungsi penulisan ke lcd agar mempermudah penghapusan karakter
 */
void lcdLine(uint8_t line, String text){
  lcd.setCursor(0,line-1);
  lcd.print(F("                ")); // untuk menghapus lcd cukup tulis dengan karakter kosong, F maksudnya adalah text itu disimpan di flash agar tidak membuang buang memory
  lcd.setCursor(0,line-1);
  lcd.print(text);
}

/*
 * Fungsi pembacaan button dengan sampling, agar jika terjadi noise tidak terdeteksi kalau button dipencet
 */
#define MAX_DETECT 20
#define MIN_DETECT 15
bool isBtn1(){
  // siapkan penampung data
  uint8_t cnt = 0;
  // sampling sebanyak maximal deteknya di delay 10micro second
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btn1) == 0) cnt++;
    delayMicroseconds(10);
  }
  // jika banyaknya data melebihi threshold yang kita berikan berarti memang benar dipencet manusia
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
bool isBtnMode(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnMode) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
