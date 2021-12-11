#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//#define SSID "RumahNikita"
//#define PASS "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"
#define SSID "ranirona"
#define PASS "baksoenak"

#define ledY 22
#define ledN 23
#define LED_ON 1
#define LED_OFF !LED_ON

void setup() {
  pinMode(ledY, OUTPUT);
  pinMode(ledN, OUTPUT);
  digitalWrite(ledY, LED_OFF);
  digitalWrite(ledN, LED_OFF);
  
  Serial.begin(9600);
  Serial2.begin(9600);
  
  Serial.println(F("Waiting connection"));
  WiFi.begin(SSID, PASS);
  uint8_t cnt = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print('.');
    if(++cnt > 30) ESP.restart();
  }
  Serial.println();
  Serial.println(F("WiFi Connected"));
  delay(500);
  Serial2.println("WiFi Connected");
}

String url = "http://192.168.43.103/ubayaparking/api/out.php?id=";
void loop() {
  if(Serial2.available() > 0){
    String dataIn = Serial2.readStringUntil('\n');
    if(dataIn.indexOf(F("o,")) >= 0){
      bool success = false;
      if(WiFi.status() == WL_CONNECTED){
        char json[500];
        HTTPClient http;
        int httpResponseCode = 0;
        StaticJsonDocument<200> doc;
        uint8_t n = 0;

        String newUrl = url + dataIn.substring(dataIn.indexOf(",") + 1);
        newUrl.trim();
        
        while(httpResponseCode <= 0){
          http.begin(newUrl);
          httpResponseCode = http.GET();
          
          Serial.print(F("Accessing: "));
          Serial.println(newUrl);
          Serial.print(F("Response Code: "));
          Serial.println(httpResponseCode);
          
          delay(100);
          if(httpResponseCode > 0){
            String response = http.getString();

            Serial.println(F("Response: "));
            Serial.println(response);
            
            response.toCharArray(json, 500);
            deserializeJson(doc, json);

            Serial.print(F("Json Object: "));
            serializeJson(doc, Serial);
            Serial.println();

            if(doc["message"] == "success") success = true;
          } else {
            Serial.println(F("http connection error, retrying"));
          }
          
          delay(500);
          http.end();
          if(++n >= 10) break;
        }
      }

      if(success) {
        Serial2.println(F("OK"));
        digitalWrite(ledY, LED_ON);
        delay(2000);
        digitalWrite(ledY, LED_OFF);
      } else{
        Serial2.println(F("NO"));
        digitalWrite(ledN, LED_ON);
        delay(2000);
        digitalWrite(ledN, LED_OFF);
      }
    }
  }
}
