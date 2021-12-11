#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

// Configuration
const char *ssid = "RumahNikita";
const char *pass = "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd";
const char *mqtt_server = "broker.hivemq.com";
const uint16_t port = 1883;
const char *ntp_server = "pool.ntp.org";

// MQTT TOPIC SET
const char *device_topic = "ScoreBoardPalembang/device1/status";
const char *remote_topic_teamname1 = "ScoreBoardPalembang/device1/teamname1";
const char *remote_topic_teamname2 = "ScoreBoardPalembang/device1/teamname2";
const char *remote_topic_pointcount1 = "ScoreBoardPalembang/device1/point1up";
const char *remote_topic_pointcount2 = "ScoreBoardPalembang/device1/point2up";
const char *remote_topic_team1win = "ScoreBoardPalembang/device1/team1win";
const char *remote_topic_team2win = "ScoreBoardPalembang/device1/team2win";
const char *remote_topic_undoaction = "ScoreBoardPalembang/device1/undoaction";
const char *remote_topic_resetgame = "ScoreBoardPalembang/device1/resetgame";

// NTP Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntp_server, 25200, 60000);

// MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);

String message = "";
void callback(const char *topic, byte *payload, unsigned int length){
  message = "";
  for(int i=0; i<length; i++){
    message += (char) payload[i];
  }

retry:
  Serial.flush();
  if(String(topic) == String(remote_topic_teamname1)){
    Serial.print("team1:");
    Serial.println(message);
  }
  if(String(topic) == String(remote_topic_teamname2)){
    Serial.print("team2:");
    Serial.println(message);
  }
  if(String(topic) == String(remote_topic_pointcount1)){
    Serial.println("point1up");
  }
  if(String(topic) == String(remote_topic_pointcount2)){
    Serial.println("point2up");
  }
  if(String(topic) == String(remote_topic_team1win)){
    Serial.println("team1win");
  }
  if(String(topic) == String(remote_topic_team2win)){
    Serial.println("team2win");
  }
  if(String(topic) == String(remote_topic_undoaction)){
    Serial.println("undoaction");
  }
  if(String(topic) == String(remote_topic_resetgame)){
    Serial.println("resetgame");
  }

  unsigned long timeStartWait = millis();
  bool gotReply = 1;
  while(!Serial.available()){
    if((unsigned long) millis()-timeStartWait >= 500){
      gotReply = 0; // timeout
      break;
    }
  }

  if(gotReply == 0) goto retry;
  if(Serial.readStringUntil('\n').indexOf("OK") < 0) goto retry;
}

void reconnect(){
  while(!client.connected()){
    if(client.connect("sbpdevice1")){
      client.publish(device_topic, "back online");

      client.subscribe(remote_topic_teamname1);
      client.subscribe(remote_topic_teamname2);
      client.subscribe(remote_topic_pointcount1);
      client.subscribe(remote_topic_pointcount2);
      client.subscribe(remote_topic_team1win);
      client.subscribe(remote_topic_team2win);
      client.subscribe(remote_topic_undoaction);
      client.subscribe(remote_topic_resetgame);
    } else {
      delay(5000);
    }
  }
}

void setup(){
  // setup serial connection first on Serial 0 for get the wifi connection status
  Serial.begin(9600);

  // setup wifi
  delay(10);
  Serial.println(F("Connecting to"));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.write('.');
  }
  Serial.println();
  Serial.println(F("Connected"));

  // setup mqtt connection
  client.setServer(mqtt_server, port);
  client.setCallback(callback);

  // setup ntp client
  timeClient.begin();

  // swap to serial 2 pin D7(RX) D8(TX)
  delay(1000);
  Serial.swap();
}

unsigned long updatePeriod = millis();
void loop() {
  // update time same as now
  timeClient.update();

  // always re check the connection
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  if((unsigned long) millis()-updatePeriod >= 1000 && client.connected()){
    String payload = "device 1 is up " + timeClient.getFormattedTime();
    client.publish(device_topic, payload.c_str());
    updatePeriod = millis();
  }
}
