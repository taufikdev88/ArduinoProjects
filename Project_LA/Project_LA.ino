#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>

String key = "12345";
#define relay1 16
#define relay2 17

float PZEMVoltage = 0.0;
float PZEMCurrent = 0.0;
float PZEMPower = 0.0;
float PZEMEnergy = 0.0;

PZEM004Tv30 pzem(&Serial2);

void setup() {
  const char* ssid = "RumahNikita"; //Nama WiFi
  const char* password = "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"; //Password WiFi
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, 0);
  digitalWrite(relay2, 0);

  Serial.println(F("Connecting."));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    Serial.print(F("."));
  }
  Serial.println(F("Connection OK"));
}

const String query1 = "http://projectla.xyz/control.php?token=12345&channel=mikro1r1"; 
const String query2 = "http://projectla.xyz/control.php?token=12345&channel=mikro1r2";
const String query3 = "http://projectla.xyz/control.php?token=12345&mikro=mikro1&";
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    char json[500];
    HTTPClient http;
    int httpResponseCode;
    StaticJsonDocument<200> doc;
    
    /*
     * Cek relay 1
     */
    http.begin(query1);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpResponseCode = http.GET();
    delay(100);
    if(httpResponseCode > 0) {
      String response = http.getString();
      response.toCharArray(json, 500);
      deserializeJson(doc, json);

      Serial.print(F("Relay 1 response: "));
      serializeJson(doc, Serial);
      Serial.println();

      if(doc["data"] == "ON") digitalWrite(relay1, 1);
      else if(doc["data"] == "OFF") digitalWrite(relay1, 0);
    } else {
      Serial.print(F("Relay 1 error code: "));
      Serial.println(httpResponseCode);
    }
    http.end();  //Free resources
    delay(500);

    /*
     * Cek relay 2
     */
    http.begin(query2);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpResponseCode = http.GET();
    delay(100);

    if(httpResponseCode > 0){
      String response = http.getString();
      response.toCharArray(json, 500);
      deserializeJson(doc, json);

      Serial.print(F("Relay 2 response: "));
      serializeJson(doc, Serial);
      Serial.println();

      if(doc["data"] == "ON") digitalWrite(relay2, 1);
      else if(doc["data"] == "OFF") digitalWrite(relay2, 0);
    } else {
      Serial.print(F("Relay 2 error code: "));
      Serial.println(httpResponseCode);
    }
    http.end(); //Free resources
    delay(500);

    /*
     * Kirim data tegangan
     */
    PZEMVoltage = pzem.voltage();
    PZEMCurrent = pzem.current();
    PZEMPower = pzem.power();
    PZEMEnergy = pzem.energy();
    char vstr[9];
    char cstr[9];
    char pstr[9];
    char estr[9];
    dtostrf(PZEMVoltage,5,3,vstr);
    dtostrf(PZEMCurrent,5,3,cstr);
    dtostrf(PZEMPower,5,3,pstr);
    dtostrf(PZEMEnergy,5,3,estr);
    char buff[50];
    sprintf(buff,"v=%s&c=%s&p=%s&e=%s",vstr,cstr,pstr,estr);
    http.begin(String(query3 + buff));
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpResponseCode = http.GET();
    delay(100);

    if(httpResponseCode > 0){
      String response = http.getString();
      response.toCharArray(json, 500);
      deserializeJson(doc, json);

      Serial.print(F("Set data response: "));
      serializeJson(doc, Serial);
      Serial.println();
    } else {
      Serial.print(F("Set data error code: "));
      Serial.println(httpResponseCode);
    }
    http.end(); //Free resources
    
  } else {
    Serial.println("Error in WiFi connection");
  }
  
  delay(1000);
}
