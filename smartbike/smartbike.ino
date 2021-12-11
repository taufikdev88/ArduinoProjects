// library untuk software interrupt , digunakan untuk menghitung jumlah pulse dari encoder dalam suatu periode lalu diubah ke bentuk rpm digunakan untuk referensi aktual dari PID
#include <Ticker.h>
// library default untuk ESP
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
// library yg dibutuhkan untuk membuat http server
#include <ESP8266WebServer.h>
// library yg dibutuhkan untuk membungkus data yg dikirim atau diterima dalam bentuk json
#include <ArduinoJson.h>
// library yg dibutuhkan untuk menyimpan variabel ke eeprom
#include <EEPROM.h>

// sesuaikan nama hostpot dan password yg ingin digunakan
#ifndef APSSID
#define APSSID "SmartBike"
#define APPSK  "baksoenak"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

// siapkan sebuah server dengan port 80
ESP8266WebServer server(80);

// konstanta default untuk PID motor
#define KP 0.2
#define KI 0.04
#define KD 0.07

// variable yg digunakan untuk referensi rpm dari motor
int normalRpm = 500;
int speedRpm = 700;
// variable yg digunakan untuk menyimpan nilai rpm aktual
int rpmAct = 0;
// variable untuk menyimpan delta error dan integral error
float dErr = 0.0, iErr = 0.0;
// variable yg digunakan untuk menyimpan jumlah pulsa yg dihitung dalam suatu periode
volatile int pulseCounter = 0;
#define pulsesperturn 72.0

// definisi pin 
#define encoderPin D3 
#define pwmPin D2 
#define lPwm D1
#define rPwm D0
#define relay D4

// object untuk software interrupt
Ticker motorControl;

// menyimpan kondisi dari sepedanya
bool normal = true;
bool stateOn = false;

// readRpm adalah fungsi yg nantinya dijalankan setiap periode tertentu oleh software interrupt
unsigned long timeold = millis();
void readRpm(){
  noInterrupts();
  rpmAct = (pulseCounter/pulsesperturn) * 1000.0 / ((unsigned long)millis()-timeold) * 60.0;
  pulseCounter = 0;
  timeold = millis();
  interrupts();
}

// outrpm juga sesuai dijalankan setiap periode tertentu yg nantinya outputnya ke kecepatan motor
void outRpm(){
  if(stateOn){
    float pErr = (normal ? normalRpm : speedRpm) - rpmAct;
    int oPwm = KP * pErr + KI * iErr + KD * (pErr - dErr);
    dErr = pErr;
    iErr += pErr;
    iErr = constrain(iErr, -6000, 6000);
    oPwm = constrain(oPwm, 0, 255);
    analogWrite(pwmPin, oPwm);
  } else {
    analogWrite(pwmPin, 0);
  }
}

// external hardware interrupt, yg selalu dijalankan saat ada trigger dari perubahan nilai tegangan dari encoder dari logic 0 ke 1 dan sebaliknya
unsigned long lastInterrupt = millis();
ICACHE_RAM_ATTR void handleInterrupt(){
  if((unsigned long) millis() - lastInterrupt > 1){
    pulseCounter++;
    lastInterrupt = millis();
  }
}

// fungsi setup pin motor default
void setupMotor(){
  pinMode(pwmPin, OUTPUT);
  pinMode(lPwm, OUTPUT);
  pinMode(rPwm, OUTPUT);
  analogWrite(pwmPin, 0);
  digitalWrite(lPwm, 0);
  digitalWrite(rPwm, 1);

  // attach interrupt artinya untuk external interrupt
  pinMode(encoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPin), handleInterrupt, FALLING);
  // ini untuk software interrupt setiap 100 ms untuk readRPm dan setiap 30 ms outRpm
  motorControl.attach_ms(100, readRpm);
  motorControl.attach_ms(30, outRpm); 
}

// membaca nilai yg terakhir di simpan oleh user
void readEEPROM(){
  int temp = 0;
  for(int i=0; i<5; i++){
    temp += EEPROM.read(i);
    Serial.println((String) "Read at address #" + i + "\t Sum value: " + temp);
  }
  normalRpm = temp;
  Serial.println((String) "Normal RPM: " + normalRpm);
  temp = 0;
  for(int i=5; i<10; i++){
    temp += EEPROM.read(i);
    Serial.println((String) "Read at address #" + i + "\t Sum value: " + temp);
  }
  speedRpm = temp;
  Serial.println((String) "Speed RPM: " + speedRpm);
}

// menyimpan nilai settingan user ke EEPROM agar saat robot di nyalakan ulang, nilainya tidak hilang
void writeEEPROM(){
  for(int i=0; i<5; i++){
    if(normalRpm / 255 > 0 && normalRpm != 0){
      EEPROM.write(i, 255);
      normalRpm -= 255;
      Serial.println((String) "Normal Rpm 1: " + normalRpm);
      continue;
    } else if (normalRpm % 255 > 0 && normalRpm != 0){
      EEPROM.write(i, (normalRpm % 255));
      normalRpm -= (normalRpm % 255);
      Serial.println((String) "Normal Rpm 2: " + normalRpm);
      continue;
    } else {
      Serial.println((String) "Normal Rpm 3: " + normalRpm);
      EEPROM.write(i, 0);
    }
  }
  for(int i=5; i<10; i++){
    if(speedRpm / 255 > 0 && speedRpm != 0){
      EEPROM.write(i, 255);
      speedRpm -= 255;
      continue;
    } else if(speedRpm % 255 > 0 && speedRpm != 0){
      EEPROM.write(i, (speedRpm % 255));
      speedRpm -= (speedRpm % 255);
      continue;
    } else {
      EEPROM.write(i, 0);
    }
  }
  if(EEPROM.commit()) Serial.println("berhasil"); else Serial.println("gagal");
}

// jika tidak ada perintah apapun, beri tanda saja kalau web servernya berjalan
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

// jika ada perintah untuk set nilai normal rpm dan speed rpm yang baru
void handleSet(){
  if(server.args() > 0){
    for(int i=0; i<server.args(); i++){
      Serial.println(server.argName(i));
      if(server.argName(i) == "normalRpm"){
        normalRpm = server.arg(i).toInt();
      }
      if(server.argName(i) == "speedRpm"){
        speedRpm = server.arg(i).toInt();
      }
    }
    writeEEPROM();
    readEEPROM();
  }
  const size_t capacity = JSON_OBJECT_SIZE(2);
  DynamicJsonDocument doc(capacity);
  doc["normalRpm"] = normalRpm;
  doc["speedRpm"] = speedRpm;
  serializeJson(doc, Serial);
  char output[measureJsonPretty(doc) + 1];
  serializeJsonPretty(doc, &output, measureJsonPretty(doc) + 1);
  server.send(200, "text/html", output);
  doc.garbageCollect();
}

// jika ingin melihat rpm yg sekarang untuk ditampilkan ke android
void handleGet(){
  const size_t capacity = JSON_OBJECT_SIZE(2);
  DynamicJsonDocument doc(capacity);
  doc["status"] = "success";
  doc["rpm"] = rpmAct;
  char output[measureJsonPretty(doc) + 1];
  serializeJsonPretty(doc, &output, measureJsonPretty(doc) + 1);
  server.send(200, "text/html", output);
  doc.garbageCollect();
  Serial.println("RPM Request");
}

// jika ada perintah untuk mengaktifkan atau mematikan motor via relay
void handleMain(){
  if(server.args() > 0){
    for(int i=0; i<server.args(); i++){
      Serial.println((String) server.argName(i) + ":" + server.arg(i));
      if(server.argName(i) == "motorOn"){
        stateOn = (server.arg(i).toInt() ? true : false);
        digitalWrite(relay, stateOn);
      }
      if(server.argName(i) == "normal"){
        normal = (server.arg(i).toInt() ? true : false);
      }
    }
  }
  const size_t capacity = JSON_OBJECT_SIZE(2);
  DynamicJsonDocument doc(capacity);
  doc["status"] = "success";
  doc["state"] = stateOn;
  char output[measureJsonPretty(doc) + 1];
  serializeJsonPretty(doc, &output, measureJsonPretty(doc) + 1);
  server.send(200, "text/html", output);
  doc.garbageCollect();
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.on("/get", handleGet);
  server.on("/main", handleMain);
  server.begin();
  Serial.println("HTTP server started");

  EEPROM.begin(512);
  readEEPROM();
  setupMotor();

  pinMode(relay, OUTPUT);
  digitalWrite(relay, stateOn);
}

void loop() {
  server.handleClient();
}
