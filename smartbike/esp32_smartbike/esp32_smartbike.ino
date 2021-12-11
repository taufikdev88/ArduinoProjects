#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
// library yg dibutuhkan untuk membungkus data yg dikirim atau diterima dalam bentuk json
#include <ArduinoJson.h>

const char *ssid = "SmartBike";
const char *password = "smartbike";

WebServer server(80);

#define relayA 13
#define relayB 14
#define relayC 15
#define relayD 16
#define relayE 17

// menyimpan kondisi dari sepedanya
bool normal = true;
bool stateOn = false;

// jika tidak ada perintah apapun, beri tanda saja kalau web servernya berjalan
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

// jika ingin melihat rpm yg sekarang untuk ditampilkan ke android
void handleGet(){
  const size_t capacity = JSON_OBJECT_SIZE(2);
  DynamicJsonDocument doc(capacity);
  doc["status"] = "success";
  doc["state"] = (stateOn ? "Motor On":"Motor Off");
  char output[measureJsonPretty(doc) + 1];
  serializeJsonPretty(doc, &output, measureJsonPretty(doc) + 1);
  server.send(200, "text/html", output);
  doc.garbageCollect();
}

// jika ada perintah untuk mengaktifkan atau mematikan motor via relay
void handleMain(){
  if(server.args() > 0){
    for(int i=0; i<server.args(); i++){
      Serial.println((String) server.argName(i) + ":" + server.arg(i));
      if(server.argName(i) == "motorOn"){
        stateOn = (server.arg(i).toInt() ? true : false);
      }
      if(server.argName(i) == "normal"){
        normal = (server.arg(i).toInt() ? true : false);
      }
    }
  }
  if(stateOn){
    if(normal){
      normalMode();
    } else {
      speedMode();
    }
  } else {
    offMode();
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

void normalMode(){
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, HIGH);
  digitalWrite(relayC, LOW);
  digitalWrite(relayD, LOW);
  digitalWrite(relayE, HIGH);
}
void speedMode(){
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, LOW);
  digitalWrite(relayC, LOW);
  digitalWrite(relayD, HIGH);
  digitalWrite(relayE, HIGH);
}
void offMode(){
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, LOW);
  digitalWrite(relayC, LOW);
  digitalWrite(relayD, LOW);
  digitalWrite(relayE, HIGH);
}

void setup(void) {
  pinMode(relayA, OUTPUT);
  pinMode(relayB, OUTPUT);
  pinMode(relayC, OUTPUT);
  pinMode(relayD, OUTPUT);
  pinMode(relayE, OUTPUT);
  
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, LOW);
  digitalWrite(relayC, LOW);
  digitalWrite(relayD, LOW);
  digitalWrite(relayE, HIGH);
  
  Serial.begin(9600); 
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  server.on("/get", handleGet);
  server.on("/main", handleMain);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
