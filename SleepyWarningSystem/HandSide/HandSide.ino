#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "DEVICE1";
const char *pass = "h4h4h4h4";

ESP8266WebServer server(80);
#define REPORTING_PERIOD_MS 1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
int heartRate;

void handleRoot(){
  server.send(200, "text/html", "Hello There!");
}

void handleReadSensor(){
  server.send(200, "text/html", String(heartRate));
}
void onBeatDetected()
{
    Serial.println("Beat!");
}
void setup() {
  delay(1000);
  Serial.begin(115200);
  
  Serial.println(F("Starting..."));
  if (!pox.begin()) {
    Serial.println("FAILED");
    for(;;);
  } else {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
  
  WiFi.softAP(ssid, pass, 1, 1);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print(F("My Ip Addr: "));
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  server.on("/readsensor", handleReadSensor);
  server.begin();
  Serial.println("HTTP Server Started");
}

void loop() {
  server.handleClient();
  pox.update();
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    heartRate = pox.getHeartRate();
    
    Serial.print("Heart rate:");
    Serial.print(heartRate);
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");

    tsLastReport = millis();
  }
}
