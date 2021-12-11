#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#define SSID "RumahNikita"
#define PASS "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"

#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883

#define STS_TOPIC "HAPalembang/status"
#define CMD_TOPIC "HAPalembang/command"
#define CLIENT_ID "HAClient"

#define channel1pin 12
#define channel2pin 14

WiFiClient wifiClient;
PubSubClient client(wifiClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000);

bool isLogRequest = false;

#define openStartAddrCh1 0
#define closeStartAddrCh1 1024
#define openStartAddrCh2 2048
#define closeStartAddrCh2 3072

void writeTime(uint16_t startAddr, uint8_t hour, uint8_t minute){
  for(uint16_t i = startAddr; i < (startAddr + 1024); i += 2){
    if(EEPROM.read(i) == 255){
      EEPROM.write(i, hour);
      EEPROM.write(i+1, minute);
      EEPROM.commit();
      break;
    }
  }
}

String readTime(){
  String tmp = "";
  for(uint16_t i = openStartAddrCh1; i < 1024; i += 2){
    if(EEPROM.read(i) != 255){
      tmp += ",";
      tmp += String(EEPROM.read(i));
      tmp += ":";
      tmp += String(EEPROM.read(i+1));
    } else break;
  }
  tmp += ",_";
  for(uint16_t i = closeStartAddrCh1; i < (closeStartAddrCh1 + 1024); i += 2){
    if(EEPROM.read(i) != 255){
      tmp += ",";
      tmp += String(EEPROM.read(i));
      tmp += ":";
      tmp += String(EEPROM.read(i+1));
    } else break;
  }
  tmp += ",_";
  for(uint16_t i = openStartAddrCh2; i < (openStartAddrCh2 + 1024); i += 2){
    if(EEPROM.read(i) != 255){
      tmp += ",";
      tmp += String(EEPROM.read(i));
      tmp += ":";
      tmp += String(EEPROM.read(i+1));
    } else break;
  }
  tmp += ",_";
  for(uint16_t i = closeStartAddrCh2; i < (closeStartAddrCh2 + 1024); i += 2){
    if(EEPROM.read(i) != 255){
      tmp += ",";
      tmp += String(EEPROM.read(i));
      tmp += ":";
      tmp += String(EEPROM.read(i+1));
    } else break;
  }
  return tmp;
}

void clearTime(){
  Serial.println(F("Resetting EEPROM addr 0-4096 to 255"));
  for(uint16_t i = 0; i < 4096; i++){
    EEPROM.write(i, 255);
  }
  
  if(EEPROM.commit()){
    Serial.println(F("EEPROM succesfully cleared"));
  } else {
    Serial.println(F("ERROR! EEPROM commit failed"));
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print(F("Message arrived["));
  Serial.print(topic);
  Serial.println(F("]"));

  String dataIn = "";
  for(uint16_t i=0; i<length; i++){
    dataIn += (char) payload[i];
  }
  Serial.println(dataIn);

  if(dataIn.substring(0, dataIn.indexOf(':')) == "1"){
    if(dataIn.substring(dataIn.indexOf(':') + 1) == "1"){
      digitalWrite(channel1pin, 1);
      Serial.println(F("channel 1 on"));
      writeTime(openStartAddrCh1, timeClient.getHours(), timeClient.getMinutes());
      
    } else if(dataIn.substring(dataIn.indexOf(':') + 1) == "0"){
      digitalWrite(channel1pin, 0);
      Serial.println(F("channel 1 off"));
      writeTime(closeStartAddrCh1, timeClient.getHours(), timeClient.getMinutes());
      
    }
  } else if(dataIn.substring(0, dataIn.indexOf(':')) == "2"){
    if(dataIn.substring(dataIn.indexOf(':') + 1) == "1"){
      digitalWrite(channel2pin, 1);
      Serial.println(F("channel 2 on"));
      writeTime(openStartAddrCh2, timeClient.getHours(), timeClient.getMinutes());
      
    } else if(dataIn.substring(dataIn.indexOf(':') + 1) == "0"){
      digitalWrite(channel2pin, 0);
      Serial.println(F("channel 2 off"));
      writeTime(closeStartAddrCh2, timeClient.getHours(), timeClient.getMinutes());
      
    }
  } else if(dataIn.indexOf("read") >= 0){
    isLogRequest = true;
    
    String data = "{\"data\":\"";
    data += readTime();
    data += "\"}";
    Serial.println(data);
    
    uint16_t st = 0;
    uint16_t en = 128;
    while(st < data.length()){
      client.publish(STS_TOPIC, data.substring(st,en).c_str());
      st = en;
      en += 128;
      delay(10);
    }
    client.publish(STS_TOPIC, "finish");
    isLogRequest = false;
  } else if(dataIn.indexOf("clear") >= 0){
    clearTime();
    
  }
}

void reconnect(){
  while(!client.connected()){
    Serial.println(F("Attempting MQTT connection.."));
    if(client.connect(CLIENT_ID)){
      Serial.println(F("Connected"));
      client.subscribe(CMD_TOPIC);
    } else {
      Serial.println(F("Failed ,try again in 5 seconds"));
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(channel1pin, OUTPUT);
  pinMode(channel2pin, OUTPUT);
  EEPROM.begin(4096);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  uint8_t cnt = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print('.');
    if(++cnt > 60) ESP.restart();
  }

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);

  timeClient.begin();
}

unsigned long timingUpdate = millis();
void loop() {
  timeClient.update();
  
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  if((unsigned long) millis()-timingUpdate >= 500 && isLogRequest == false){
    char payload[50] = {0};
    sprintf(payload, "{\"channel1\":\"%s\",\"channel2\":\"%s\"}", (digitalRead(channel1pin) == 1 ? "on" : "off"), (digitalRead(channel2pin) == 1 ? "on" : "off"));
    client.publish(STS_TOPIC, payload);
    timingUpdate = millis();
  }
}
