/*
### Read SMS messages
 1. This example is used to test DFRobot_SIM808 GPS/GPRS/GSM Shield's reading SMS
 2. Open the SIM808_SMSread example or copy these code to your project
 3. Download and dial the function switch to Arduino
 4. it will receive SMS Messages and send it to serial

create on 2016/09/23, version: 1.0
by jason
*/

#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;

char MESSAGE[300];
char lat[12];
char lon[12];
char wspeed[12];

char phone[16];
char datetime[24];

SoftwareSerial mySerial(9,8);
DFRobot_SIM808 sim808(&mySerial);

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  //******** Initialize sim808 module *************
  while(!sim808.init()) {
      Serial.print(F("Sim808 init error\r\n"));
      delay(1000);
  }
  
  Serial.println(F("Init Success, please send SMS message to me!"));
  
  //************* Turn on the GPS power************
//  if( sim808.attachGPS())
//      Serial.println(F("Open the GPS power success"));
//  else 
//      Serial.println(F("Open the GPS power failure"));
}

unsigned long timing = 0;
bool sw = false;
void loop() {
  if((unsigned long) millis()-timing > 10000){
    mySerial.println(( sw ? "at+creg?" : "at+cops?"));
    timing = millis();
    sw = !sw;
    delay(100);
    while (mySerial.available()) {
      Serial.write(mySerial.read());
    }
    return;
  }
  
  //*********** Detecting unread SMS ************************
  messageIndex = sim808.isSMSunread();
  Serial.print(F("messageIndex: "));
  Serial.println(messageIndex);

  //*********** At least, there is one UNREAD SMS ***********
  if (messageIndex > 0) { 
    sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
               
    //***********In order not to full SIM Memory, is better to delete it**********
    sim808.deleteSMS(messageIndex);
    Serial.print(F("From number: "));
    Serial.println(phone);  
    Serial.print(F("Datetime: "));
    Serial.println(datetime);        
    Serial.print(F("Recieved Message: "));
    Serial.println(message);    
//    sim808.sendSMS(phone,"Muahhh");

    return;
    while(!sim808.getGPS());
    
    Serial.print(sim808.GPSdata.year);
    Serial.print(F("/"));
    Serial.print(sim808.GPSdata.month);
    Serial.print(F("/"));
    Serial.print(sim808.GPSdata.day);
    Serial.print(F(" "));
    Serial.print(sim808.GPSdata.hour);
    Serial.print(F(":"));
    Serial.print(sim808.GPSdata.minute);
    Serial.print(F(":"));
    Serial.print(sim808.GPSdata.second);
    Serial.print(F(":"));
    Serial.println(sim808.GPSdata.centisecond);
    
    Serial.print(F("latitude :"));
    Serial.println(sim808.GPSdata.lat,6);
    
    Serial.print(F("longitude :"));
    Serial.println(sim808.GPSdata.lon,6);
    
    Serial.print(F("speed_kph :"));
    Serial.println(sim808.GPSdata.speed_kph);
    Serial.print(F("heading :"));
    Serial.println(sim808.GPSdata.heading);
    
//    float la = sim808.GPSdata.lat;
//    float lo = sim808.GPSdata.lon;
//    float ws = sim808.GPSdata.speed_kph;
//    
//    dtostrf(la, 6, 2, lat); //put float value of la into char array of lat. 6 = number of digits before decimal sign. 2 = number of digits after the decimal sign.
//    dtostrf(lo, 6, 2, lon); //put float value of lo into char array of lon
//    dtostrf(ws, 6, 2, wspeed);  //put float value of ws into char array of wspeed
    
//    sprintf(MESSAGE, "Latitude : %s\nLongitude : %s\nWind Speed : %s kph\nMy Module Is Working. Mewan Indula Pathirage. Try With This Link.\nhttp://www.latlong.net/Show-Latitude-Longitude.html\nhttp://maps.google.com/maps?q=%s,%s\n", lat, lon, wspeed, lat, lon);
//    
//    Serial.println("Sim808 init success");
//    Serial.println("Start to send message ...");
//    Serial.println(MESSAGE);
//    Serial.println(phone);
//    sim808.sendSMS(phone,MESSAGE);
  }
}
