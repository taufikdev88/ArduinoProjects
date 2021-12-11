#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(10,11);

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);
}

unsigned long timing = 0;
bool sw = false;
void loop() { // run over and over
  if(gpsSerial.available()){
    Serial.write(gpsSerial.read());
  }
  if(gsmSerial.available()){
    Serial.write(gsmSerial.read());
  }
  if((unsigned long) millis()-timing > 2000){
    gsmSerial.println(( sw ? "at+creg?" : "at+cops?"));
    timing = millis();
    sw = !sw;
    delay(100);
    while(gsmSerial.available()){
      Serial.write(gsmSerial.read());
    }
  }
  if(gpsSerial.available()){
    
  }
}
