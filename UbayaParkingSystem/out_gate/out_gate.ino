#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {  
  Serial.begin(9600);
  Serial2.begin(9600);
  SerialBT.begin("OUT GATE");

  Serial.println(F("Waiting for second device"));
  while(true){
    if(Serial2.available() > 0){
      String dataIn = Serial2.readStringUntil('\n');
      Serial.print(dataIn);
      Serial.print('\t');
      Serial.println(dataIn.indexOf("WiFi"));
      
      if(dataIn.indexOf("Connected") >= 0){
        Serial.println(F("OK Wifi Connected"));
        break;
      }
    }
  }
  Serial.println(F("Second device is up"));
}

void loop() {
  if(SerialBT.available() > 0){
    String dataIn = SerialBT.readStringUntil('\n');
    Serial.println(dataIn);
    Serial2.flush();
    delay(1);

    if(dataIn.indexOf(F("i,")) >= 0){
      // langsung kasih sinyal 1 karena ini khusus tempat keluar
      SerialBT.println(1);
    } else
    if(dataIn.indexOf(F("o,")) >= 0){
      Serial2.println(dataIn);
      while(true){
        if(Serial2.available() > 0){
          String answer = Serial2.readStringUntil('\n');
          if(answer.indexOf(F("OK")) >= 0){
            SerialBT.println(2);
            break;
          } else
          if(answer.indexOf(F("NO")) >= 0){
            SerialBT.println(3);
            break;
          }
        }
      }
    } else
    if(dataIn.indexOf(F("r,")) >= 0){
      // langsung kasih sinyal 5 karena register hanya bisa dilakukan di in gate
      SerialBT.println(5);
    }
  }
}
