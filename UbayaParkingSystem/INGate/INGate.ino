#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println(F("device connected"));
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println(F("device disconnected"));
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      String dataIn = "";
      String dataValue = "";
      uint8_t answer = 100;
      
      if (rxValue.length() > 0) {
        for (int i = 0; i < rxValue.length(); i++){
          dataIn += rxValue[i];
        }
      }

      if(WiFi.status() == WL_CONNECTED){
        if(dataIn.indexOf(F("i,")) >= 0){
          dataValue = dataIn.substring(dataIn.indexOf(',') + 1);
          // check ijin ke hp satpam dulu
          
          answer = 0;
        } else
        if(dataIn.indexOf(F("o,")) >= 0){
          dataValue = dataIn.substring(dataIn.indexOf(',') + 1);
          answer = 3;
          // langsung kasih sinyal 3 karena ini khusus tempat masuk
        } else 
        if(dataIn.indexOf(F("r,")) >= 0){
          dataValue = dataIn.substring(dataIn.indexOf(',') + 1);
          // cek id device dulu
          answer = 4;
        } else 
        if(dataIn.indexOf(F("n,")) >= 0){
          dataValue = dataIn.substring(dataIn.indexOf(',') + 1);
          // cek nrp dulu
          answer = 6;
        } else
        if(dataIn.indexOf(F("f,")) >= 0){
          dataValue = dataIn.substring(dataIn.indexOf(',') + 1);
          // coba simpan data ke server
          answer = 8;
        } 
      }

      pTxCharacteristic->setValue(&answer, 1);
      pTxCharacteristic->notify();
      delay(10); // bluetooth stack will go into congestion, if too many packets are sent
    }
};

void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("IN GATE");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY |
                    BLECharacteristic::PROPERTY_READ
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println(F("Connecting to wifi"));

  const char* ssid = "RumahNikita"; //Nama WiFi
  const char* password = "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"; //Password WiFi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(10);
    Serial.println(F("."));
  }
  Serial.println(F("Connection OK"));
}

void loop() {
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
