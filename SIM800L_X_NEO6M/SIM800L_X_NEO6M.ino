#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial gsmSerial(8, 9);

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;

char MESSAGE[300];
float lat;
float lon;

char latstr[16];
char lonstr[16];

char phone[16];
char datetime[24];

DFRobot_SIM808 sim808(&gsmSerial);

void setup(){
  Serial.begin(9600);
  gsmSerial.begin(9600);
  
  while(!sim808.init()) {
    Serial.print(F("Sim808 init error\r\n"));
    delay(1000);
  }

  Serial.println(F("Siap digunakan"));
}

void loop(){ 
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for (unsigned long start = millis(); millis() - start < 1000;){
    while (Serial.available()){
      char c = Serial.read();
      Serial.write(c);
      if(gps.encode(c)) newData = true;
    }
  }

  if(newData){
    unsigned long age;
    gps.f_get_position(&lat, &lon, &age);
    Serial.print("LAT=");
    Serial.print(lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lat, 6);
    Serial.print(" LON=");
    Serial.print(lon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");

  messageIndex = sim808.isSMSunread();
  Serial.print(F("messageIndex: "));
  Serial.println(messageIndex);

  if(messageIndex > 0){
    sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
    sim808.deleteSMS(messageIndex);
    Serial.print(F("From number: "));
    Serial.println(phone);  
    Serial.print(F("Datetime: "));
    Serial.println(datetime);        
    Serial.print(F("Recieved Message: "));
    Serial.println(message);    
    
    dtostrf((lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lat), 6, 6, latstr);
    dtostrf((lon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lon), 6, 6, lonstr);
    sprintf(MESSAGE, "Latitude : %s\nLongitude : %s\nAtau bisa juga dengan akses link berikut.\nhttp://www.latlong.net/Show-Latitude-Longitude.html\nhttp://maps.google.com/maps?q=%s,%s\n", latstr, lonstr, latstr, lonstr);
    Serial.println(F("Sending reply to sender: "));
    Serial.println(MESSAGE);
    sim808.sendSMS(phone, MESSAGE);
  }
}
