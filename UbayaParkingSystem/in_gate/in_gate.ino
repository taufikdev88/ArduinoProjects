#include <WiFi.h>
#include "BluetoothSerial.h"
#include <PubSubClient.h>
#include <WiFiUdp.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//#define SSID "RumahNikita"
//#define PASS "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"
#define SSID "Masukkan SSID"
#define PASS "1sampek8"

#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883

#define CLIENT_ID "in-gate"
#define IN_VALUE_TOPIC "UbayaParkingSystem/in/value"
#define IN_ANSWER_TOPIC "UbayaParkingSystem/in/answer"
#define REG_VALUE_TOPIC "UbayaParkingSystem/reg/value"
#define REG_ANSWER_TOPIC "UbayaParkingSystem/reg/answer"

#define ledY 22
#define ledN 23
#define LED_ON 1
#define LED_OFF !LED_ON

BluetoothSerial SerialBT;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void callback(char *topic, byte *payload, unsigned int length){
  Serial.print(F("Message arrived["));
  Serial.print(topic);
  Serial.println(F("]"));
  String dataIn = "";
  String fromTopic = topic;
  for(int i=0; i<length; i++){
    dataIn += (char) payload[i];
  }

  if(fromTopic.indexOf(F(IN_ANSWER_TOPIC)) >= 0){
    Serial.print(F("in answer: "));
    Serial.println(dataIn);
    if(dataIn == "Y"){
      SerialBT.println(0);      
      digitalWrite(ledY, LED_ON);
      delay(3000);
      digitalWrite(ledY, LED_OFF);
    } else 
    if(dataIn == "N"){
      SerialBT.println(1);
      digitalWrite(ledN, LED_ON);
      delay(3000);
      digitalWrite(ledN, LED_OFF);
    }
  } else
  if(fromTopic.indexOf(F(REG_ANSWER_TOPIC)) >= 0){
    Serial.print(F("reg answer: "));
    Serial.println(dataIn);
    if(dataIn == "Y"){
      SerialBT.println(4);
    } else
    if(dataIn == "N"){
      SerialBT.println(5);
    }
  }
}

void reconnect(){
  while(!client.connected()){
    Serial.println(F("Attemting MQTT connection..."));
    if(client.connect(CLIENT_ID)){
      Serial.println(F("MQTT Connected"));
      client.subscribe(IN_ANSWER_TOPIC);
      client.subscribe(REG_ANSWER_TOPIC);
    } else {
      Serial.print(F("MQTT Failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledY, OUTPUT);
  pinMode(ledN, OUTPUT);
  digitalWrite(ledY, LED_OFF);
  digitalWrite(ledN, LED_OFF);
  
  Serial.begin(9600);
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
  
  SerialBT.begin("IN GATE");

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  
  if(SerialBT.available() > 0){
    String dataIn = SerialBT.readStringUntil('\n');
    String dataValue = "";
    
    Serial.println(dataIn);

    if(dataIn.indexOf(F("i,")) >= 0){
      dataValue = dataIn.substring(dataIn.indexOf(',') + 1);
      // check ijin ke hp satpam dulu
      client.publish(IN_VALUE_TOPIC, dataValue.c_str());
    } else
    if(dataIn.indexOf(F("o,")) >= 0){
      // langsung kasih sinyal 3 karena ini khusus tempat masuk
      SerialBT.println(3);
    } else 
    if(dataIn.indexOf(F("r,")) >= 0){
      dataValue = dataIn.substring(dataIn.indexOf(',') + 1);
      // kirim data register ke satpam, dan di daftarin satpam
      client.publish(REG_VALUE_TOPIC, dataValue.c_str());
    } 
  }
}
