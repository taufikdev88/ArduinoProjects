#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEAddress.h>
#include <BLE2902.h>
#include <BLEServer.h>
#include <WiFi.h>
#include <PubSubClient.h>
//#include <WiFiUdp.h>
#include <ArduinoJson.h>

#define buzzer 32

BLEScan* pBLEScan;
BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;

int scanTime = 5;
String ourDeviceMac = "";
bool deviceConnected = false;
bool oldDeviceConnected = false;
bool isLogged = false;
String username = "";
String password = "";
String mode = "";
String dataReply = "";

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

const char* SSID = "RumahNikita";
const char* PASS = "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd";

/*
 * urutan       mac address show      mac address real
 * client 1 mac 123                   30:ae:a4:97:ec:d2
 * client 2 mac 456                   24:62:ab:e6:4d:aa
 */

#define DEVICE_NAME "UBAYA SoDis 1"
#define MASTER_MAC "30:ae:a4:97:ec:d2"

#define OTHER_MAC1 "24:6f:28:aa:87:8e"
#define OTHER_MAC2 "24:62:ab:e6:4d:aa"
#define OTHER_MAC3 "24:6f:28:aa:a0:fe"

#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883

const char* API_TOPIC = "UbayaSocialDistancing/API"; 
String ESP_TOPIC = ""; 

WiFiClient wifiClient;
PubSubClient client(wifiClient);

/*
 * Rutin untuk menerima data dari mqtt
 */
void callback(char* topic, byte* payload, unsigned int length){
  String dataIn = "";
  for(unsigned int i=0; i<length; i++){
    dataIn += (char) payload[i];
  }
  if(dataIn.indexOf("endpacket") < 0){
    dataReply += dataIn;
  } else {
    Serial.println(dataReply);

    if(mode == "l") return;
    
    if(dataReply.length() < 100){
      StaticJsonDocument<128> doc;
      deserializeJson(doc, dataReply);

      if(mode == "o" && doc["data"] == "ok"){
        Serial.print(username);
        Serial.println(" logged out");
        
        mode = "";
        username = "";
        password = "";
        isLogged = false;
      } else if(mode == "i" && doc["data"] == "ok"){
        Serial.print(username);
        Serial.println(" logged in");
        
        isLogged = true;
      }
      serializeJson(doc, Serial);
    }

    
    uint32_t st = 0;
    uint32_t en = 20;
    std::string answer = "";
    while(st < dataReply.length()){
      answer = dataReply.substring(st,en).c_str();
      pTxCharacteristic->setValue(answer);
      pTxCharacteristic->notify();
      delay(10);
      st = en;
      en += 20;
    }
    answer = "endpacket";
    pTxCharacteristic->setValue(answer);
    pTxCharacteristic->notify();
    delay(10);
    dataReply = "";
  }
}

/*
 * Rutin untuk koneksi ke mqtt server
 */
void reconnect(){
  while(!client.connected()){
    Serial.println(F("Attemting MQTT Connection"));
    if(client.connect(ourDeviceMac.c_str())){
      Serial.println(F("MQTT Connected"));
      client.subscribe(ESP_TOPIC.c_str());
    } else {
      Serial.print(F("MQTT Failed, rc="));
      Serial.println(client.state());
      delay(5000);
    }
  }
}

/*
 * Rutin untuk scan ble disekitar
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String DEVICE_MAC = advertisedDevice.getAddress().toString().c_str();
    int DEVICE_RSSI = advertisedDevice.getRSSI();
    
    Serial.print("MAC: ");
    Serial.print(DEVICE_MAC);
    Serial.print(",RSSI: ");
    Serial.println(DEVICE_RSSI);
    
    if(DEVICE_MAC == OTHER_MAC1 && DEVICE_RSSI < -51){
      digitalWrite(buzzer, 1);
    } else if(DEVICE_MAC == OTHER_MAC2 && DEVICE_RSSI < -51){
      digitalWrite(buzzer, 1);
    } else if(DEVICE_MAC == OTHER_MAC3 && DEVICE_RSSI < -51){
      digitalWrite(buzzer, 1);
    } else {
      digitalWrite(buzzer, 0);
    }

    if(digitalRead(buzzer) == 1 && username != "" && isLogged == 1){ // cek jika memang ada yang dekat dan ada user tersambung
      StaticJsonDocument<256> doc;
      mode = "l";
      doc["mode"] = mode;
      doc["mac"] = ourDeviceMac;
      doc["data"]["u"] = username;
      doc["data"]["p"] = password;
      doc["data"]["r"] = String(DEVICE_RSSI);
      doc["data"]["rm"] = DEVICE_MAC;
      char datatemp[120] = {0};
      serializeJson(doc, datatemp);
      client.publish(API_TOPIC, datatemp);

      Serial.println("kirim data karena terlalu dekat");
    }
  }
};

/*
 * rutin untuk melihat orang connect dan disconnect
 */
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param){
    deviceConnected = true;
    Serial.print(F("Device Connected: "));
  }

  void onDisconnect(BLEServer* pServer){
    deviceConnected = false;
    Serial.println(F("Device Disconnected From Bluetooth"));
    delay(1000);

    if(username != ""){
      StaticJsonDocument<128> doc;
      mode = "o";
      doc["mode"] = mode;
      doc["mac"] = ourDeviceMac;
      doc["data"]["u"] = username;
      doc["data"]["p"] = password;
      char datatemp[100] = {0};
      serializeJson(doc, datatemp);
      client.publish(API_TOPIC, datatemp); 
    }
  }
};

/*
 * rutin untuk menangkap data serial dari android
 */
class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic){
    String dataIn = "";
    
    std::string rxValue = pCharacteristic->getValue();
    if(rxValue.length() > 0){
      for(uint16_t i=0; i<rxValue.length(); i++){
        dataIn += rxValue[i];
      }
    }

    Serial.println(dataIn);
    uint8_t cp = 0;
    mode = dataIn.substring(cp,dataIn.indexOf(',', cp));
    cp = dataIn.indexOf(',') + 1;
    username = dataIn.substring(cp, dataIn.indexOf(',', cp));
    cp = dataIn.indexOf(',',cp) + 1;
    password = dataIn.substring(cp);

    StaticJsonDocument<256> doc;
    doc["mode"] = mode;
    doc["mac"] = ourDeviceMac;
    doc["data"]["u"] = username;
    doc["data"]["p"] = password;
    char datatemp[100] = {0};
    serializeJson(doc, datatemp);
    client.publish(API_TOPIC, datatemp);
  }
};

void setup() {
  Serial.begin(115200);

  //Buzzer
  pinMode(buzzer, OUTPUT);
  // inisialisasi nama ble
  BLEDevice::init(DEVICE_NAME);
  // rutin untuk scan nearby device
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  // rutin untuk ble server uart
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pTxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY | 
    BLECharacteristic::PROPERTY_READ
    );
  pTxCharacteristic->addDescriptor(new BLE2902);

  BLECharacteristic* pRxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE
    );
  pRxCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  pServer->getAdvertising()->start();
  
  BLEAddress BLEAddr = BLEDevice::getAddress();
  ourDeviceMac = BLEAddr.toString().c_str();
  Serial.println(DEVICE_NAME);
  Serial.print(F("DEVICE_MAC: "));
  Serial.print(ourDeviceMac);
  Serial.println(F("...start scanning"));

  ESP_TOPIC = "UbayaSocialDistancing/" + ourDeviceMac;
  ESP_TOPIC.replace(":","");
  Serial.println(ESP_TOPIC);

  WiFi.begin(SSID, PASS);
  uint8_t cnt = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print('.');
    if(++cnt > 60) ESP.restart();
  }
  Serial.println();
  Serial.println(F("WiFi Connected"));

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

unsigned long scanTiming = millis();
void loop() {
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  
  if((unsigned long) millis()-scanTiming >= 5000){
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    Serial.print("Devices found: ");
    Serial.println(foundDevices.getCount());
    Serial.println("Scan done!");
    pBLEScan->clearResults();
    scanTiming = millis();
  }

  if(!deviceConnected && oldDeviceConnected){
    // disconnecting
    delay(500);
    pServer->startAdvertising();
    Serial.println("Start advertising");
    oldDeviceConnected = deviceConnected;
  }
  if(deviceConnected && !oldDeviceConnected){
    // connecting
    oldDeviceConnected = deviceConnected;
  }
}
