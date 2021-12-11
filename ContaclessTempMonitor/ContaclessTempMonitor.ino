/*
 * Input library yang dibutuhkan
 * WiFi: library yang digunakan oleh esp32 untuk koneksi wifi
 * WiFiClient: library yang digunakan oleh esp32 untuk koneksi wifi -> berperan sebagai client
 * BlynkSimpleEsp32: library yang digunakan untuk tersambung dengan server blynk
 * Wire: library untuk komunikasi i2c
 * Adafruit_MLX90614: libray untuk mempermudah pengolahan data sensor MLX9014
 * Servo: library yang digunakan untuk mempermudah mengakses dan memberi perintah ke servo
 * LiquidCrystal_I2C: library yg digunakan untuk mengakses lcd i2c
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// defined variable 
const char auth[] = "4xHZJZASCsMZTnEh14PmLyKMW5WDoiMj"; // auth key yang didapat dari blynk
const char ssid[] = "RumahNikita"; // nama ssid wifi yang akan kita gunakan sebagai jalur komunikasi ke internet
const char pass[] = "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"; // password wifi

// pin declaration
#define SPK_PIN 4
#define SERVO_PIN 13
#define LED_R_PIN 14
#define LED_G_PIN 15
#define TRIGGER_PIN 16
#define ECHO_PIN 17

// config
#define CLOSE_ANGLE 0
#define OPEN_ANGLE 90
#define MAX_SERVO_OPEN 10000

// list object
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // object dari sensor mlx
BlynkTimer timer; // object timer yang digunakan untuk menjalankan sebaris kode dengan periode yang spesifik
Servo servo; // object yang digunakan untuk mengontrol servo
LiquidCrystal_I2C lcd(0x27, 16, 2); // object yg digunakan untuk mengakses lcd i2c

// global variable yang digunakan untuk menyimpan nilai temperatur
float ambientTemp = 0;
float objectTemp = 0;
int minTemp = 30;
int maxTemp = 37; // celcius
int maxDist = 30; // temp
unsigned long timing = 0; // variable untuk menyimpan timing mikro
unsigned long lcdTiming = 0; // variable untuk menyimpan timing lcd

/*
 * Fungsi untuk mengambil nilai ultrasonic
 */
#define MAX_RANGE_DETECTION 150
float getRange(){
  digitalWrite(TRIGGER_PIN, 0); // beri sinyal trigger ke ultrasonic
  delayMicroseconds(2); // sesuai dengan syarat ultrasonic agar masuk ke mode pembacaan
  digitalWrite(TRIGGER_PIN, 1);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, 0);
  float range = pulseIn(ECHO_PIN, 1, 200000) / 29.0 / 2.0; // saat nya menbaca jaraknya
  if(range > MAX_RANGE_DETECTION) return MAX_RANGE_DETECTION;
  return range;
}

/*
 * Fungsi updateData yang akan di tanam di timer agar dijalankan 1 detik sekali 
 * untuk push data ke server blynk maupun untuk pembacaan nilai sensor
 */
void updateData(){  
  Blynk.virtualWrite(V0, ambientTemp);
  Blynk.virtualWrite(V1, objectTemp);
  Blynk.virtualWrite(V2, maxTemp);
  Blynk.virtualWrite(V3, maxDist);
}

// fungsi yang digunakan untuk menerima data dari blynk
BLYNK_WRITE(V5){
  minTemp = param.asInt();
}

BLYNK_WRITE(V6){
  maxTemp = param.asInt();
}

BLYNK_WRITE(V7){
  maxDist = param.asInt();
}

/*
 * Kode yang ada di setup() hanya akan dijalankan sekali saja
 */
void setup(){
  lcd.begin(); // inisialisasi lcd
  lcd.clear(); // bersihkan cache lcd
  lcd.backlight(); // nyalakan backlight lcd
  mlx.begin(); // inisialisasi mlx 
  servo.attach(SERVO_PIN); // tanamkan pin ke object servonya
  servo.write(CLOSE_ANGLE); // langsung write default ke posisi menutup
  pinMode(LED_G_PIN, OUTPUT); // set pin led sebagai output
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(SPK_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT); // set pin trigger dari ultrasonic ke output
  pinMode(ECHO_PIN, INPUT); // set pin echo dari ultrasonic sebagai input

  // set semua led dan spk ke mati dulu
  digitalWrite(LED_G_PIN, 0);
  digitalWrite(LED_R_PIN, 0);
  digitalWrite(SPK_PIN, 0);

  Blynk.begin(auth, ssid, pass); // mulai koneksi ke server blynk
  timer.setInterval(1000L, updateData); // set periode untuk fungsi update data selama 1 detik sekali
}

/*
 * Kode yang ada di loop() akan dijalankan berulang ulang
 */
void loop(){
  Blynk.run(); // fungsi wajib agar blynk bisa terus update dan sinkron
  timer.run(); // fungsi wajib agar timer terus berjalan

  // rutin untuk membaca nilai sensor dan mengecek jarak ultrasonic
  ambientTemp = mlx.readAmbientTempC();
  objectTemp = 0.0; // jika jaraknya lebih dari maxDist set ke 0 saja

  if(getRange() <= maxDist){ // jika ultrasonic melihat ada object yang dekat
    delay(100); // delay sedikit dulu kira kira orang bisa menempatkan tangannya di tempat sesuai
    objectTemp = mlx.readObjectTempC(); // update suhu object saja
    if(objectTemp <= maxTemp && objectTemp >= minTemp){ // jika suhunya dibawah batas maka buka servo
      lcd.clear(); // hapus tampilan lcd
      lcd.setCursor(0,0);
      lcd.print("silahkan masuk!!");
      
      servo.write(OPEN_ANGLE); // buka servo 
      digitalWrite(LED_G_PIN, 1); // nyalakan lampu hijau
      digitalWrite(SPK_PIN, 1); // nyalakan speaker
      delay(500); // delay dikit
      digitalWrite(SPK_PIN, 0); // matikan lampu
      
      timing = millis(); // set timing untuk lama pembukaan servo
      lcdTiming = millis();
    } else if(objectTemp > maxTemp) {
      lcd.clear(); // hapus tampilan lcd
      lcd.setCursor(0,0);
      lcd.print("suhu tinggi!!");
      
      digitalWrite(LED_R_PIN, 1); // nyalakan lampu merah
      digitalWrite(SPK_PIN, 1);
      delay(500);
      digitalWrite(SPK_PIN, 0);
      delay(100);
      digitalWrite(SPK_PIN, 1);
      delay(500);
      digitalWrite(SPK_PIN, 0);
      digitalWrite(LED_R_PIN, 0);

      lcdTiming = millis();
    }
    lcd.setCursor(0,1);
    lcd.print("suhu : ");
    lcd.print(objectTemp);
  }

  if((unsigned long) millis() - lcdTiming >= 2000){
    lcdTiming = millis();
    
    // bersihkan tampilan lcd
    lcd.setCursor(0,0);
    lcd.print("Dekatkan Tangan ");
    lcd.setCursor(0,1);
    lcd.print("Anda ke Sensor!!");
  }
  
  if((unsigned long) millis() - timing >= MAX_SERVO_OPEN){
    servo.write(CLOSE_ANGLE);
  }
}
