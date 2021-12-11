#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "DEVICE1";
const char* pass = "h4h4h4h4";

const uint8_t vibrPin = 17;
uint8_t n = 0;

const int BPM_MIN_TH = 97;

void setup() {
  Serial.begin(115200);
  pinMode(vibrPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(vibrPin, 0);
  digitalWrite(LED_BUILTIN, 1);

  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(++n > 60) ESP.restart();
  }
  digitalWrite(LED_BUILTIN,0);

  Serial.println(F(""));
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void loop(){
  if(WiFi.status() == WL_CONNECTED){
    n = 0;
    WiFiClient client;
    HTTPClient http;
    http.begin(client, "http://192.168.4.1/readsensor");

    int httpRC = http.GET();
    if(httpRC > 0){
      Serial.print(F("Response Code: "));
      Serial.println(httpRC);

      String payload = http.getString();
      Serial.println(payload);

      if(httpRC == 200){
        if(payload.toInt() < BPM_MIN_TH){
          Serial.println(">>> BPM Under Threshold!!!");
          digitalWrite(vibrPin, 1);
        } else {
          digitalWrite(vibrPin, 0);
        } 
      }
    } else {
      Serial.print(F("Error Code: "));
      Serial.println(httpRC);
    }
    http.end();
  } else {
    Serial.println(F("WiFi Disconnected"));
    if(++n > 30) ESP.restart();
  }
  delay(500);
}
