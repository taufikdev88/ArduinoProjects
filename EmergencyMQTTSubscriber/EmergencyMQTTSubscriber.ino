/*
 * Program yang diupload ke esp sisi satpam dengan menggunakan library yang sama dengan node yang lain
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Tone32.h>
/*
 * Penjelasan ada di file 1 nya
 */
#define SSID "RumahNikita"
#define PASS "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"
/*
 * Penjelasan ada di file 1 nya
 */
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
/*
 * Penjelasan ada di file 1 nya
 * hanya saja kita  juga subcribe ke 4 node masing masing lantai agar mendapatkan 
 * status siaga dari masing masing lantai
 */
#define ESP_TOPIC_1 "rifandiUBAYA/STATUS_LANTAI_1"
#define ESP_TOPIC_2 "rifandiUBAYA/STATUS_LANTAI_2"
#define ESP_TOPIC_3 "rifandiUBAYA/STATUS_LANTAI_3"
#define ESP_TOPIC_4 "rifandiUBAYA/STATUS_LANTAI_4"

#define RPY_TOPIC_1 "rifandiUBAYA/REPLY_LANTAI_1"
#define RPY_TOPIC_2 "rifandiUBAYA/REPLY_LANTAI_2"
#define RPY_TOPIC_3 "rifandiUBAYA/REPLY_LANTAI_3"
#define RPY_TOPIC_4 "rifandiUBAYA/REPLY_LANTAI_4"

#define ESP_TOPIC "rifandiUBAYA/SATPAM"
#define APP_TOPIC "rifandiUBAYA/APP"

#define CLIENT_ID "satpam-client"
/*
 * Penjelasan ada di file 1 nya
 */
#define spkPin 4
#define led1Pin 16
#define led2Pin 17
#define led3Pin 5
#define led4Pin 18

#define LED_ON 0
#define LED_OFF !LED_ON
#define BUZZER_CHANNEL 0
/*
 * Penjelasan ada di file 1 nya
 */
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000);
/*
 * Penjelasan ada di file 1 nya
 */
WiFiClient wifiClient;
PubSubClient client(wifiClient);
/*
 * Penjelasan ada di file 1 nya
 */
void setup()
{
  /*
   * Penjelasan ada di file 1 nya
   */
  Serial.begin(115200);
  /*
   * Penjelasan ada di file 1 nya
   */
  pinMode(spkPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);
  /*
   * Penjelasan ada di file 1 nya
   */
  WiFi.begin(SSID, PASS);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(F("."));
  }

  /*
   * Penjelasan ada di file 1 nya
   */
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  
  /*
   * Penjelasan ada di file 1 nya
   */
  timeClient.begin();
}

/*
 * Menyiapkan variabel untuk penyimpan status masing masing lantai
 */
bool isEmg1 = 0;
bool isEmg2 = 0;
bool isEmg3 = 0;
bool isEmg4 = 0;
bool isSafe = 1;
/*
 * Penjelasan ada di file 1 nya
 */
void loop()
{
  /*
   * Penjelasan ada di file 1 nya
   */
  timeClient.update();
  
  /*
   * Penjelasan ada di file 1 nya
   */
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // jika node 1 menandakan nilainya adalah emergency maka nyalakan led1
  // jika tidak emergency maka matikan saja
  if(isEmg1){
    digitalWrite(led1Pin, LED_ON);
  } else {
    digitalWrite(led1Pin, LED_OFF);
  }
  if(isEmg2){
    digitalWrite(led2Pin, LED_ON);
  } else {
    digitalWrite(led2Pin, LED_OFF);
  }
  if(isEmg3){
    digitalWrite(led3Pin, LED_ON);
  } else {
    digitalWrite(led3Pin, LED_OFF);
  }
  if(isEmg4){
    digitalWrite(led4Pin, LED_ON);
  } else {
    digitalWrite(led4Pin, LED_OFF);
  }

  /*
   * Jika salah satu bernilai emergency maka nyalakan alarm dengan volume tinggi
   */
  if(isEmg1 || isEmg2 || isEmg3 || isEmg4){
    alarmHigh();
    isSafe = 0;
  } else {
    if(!isSafe){
      isSafe = 1;
      alarmStop(); 
    }
  }
}

/*
 * Penjelasan ada di file 1 nya
 */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived ");
  String fromTopic = topic;
  String dataIn = "";
  for (int i=0;i<length;i++) {
    dataIn += (char) payload[i];
  }
  Serial.print((String) " [" + fromTopic + "] ");
  Serial.print(dataIn);
  Serial.println();

  /*
   * Jika pesannya terdapat kata danger maka
   */
  if(dataIn.indexOf(F("DANGER")) >= 0){
    /*
     * Lihat dari topic mana dia datang
     */
    if(fromTopic.indexOf(F("STATUS_LANTAI_1")) >= 0){
      isEmg1 = 1;
    }
    if(fromTopic.indexOf(F("STATUS_LANTAI_2")) >= 0){
      isEmg2 = 1;
    }
    if(fromTopic.indexOf(F("STATUS_LANTAI_3")) >= 0){
      isEmg3 = 1;
    }
    if(fromTopic.indexOf(F("STATUS_LANTAI_4")) >= 0){
      isEmg4 = 1;
    }
  } else if(dataIn.indexOf(F("SAFE"))){
    if(fromTopic.indexOf(F("STATUS_LANTAI_1")) >= 0){
      isEmg1 = 0;
    }
    if(fromTopic.indexOf(F("STATUS_LANTAI_2")) >= 0){
      isEmg2 = 0;
    }
    if(fromTopic.indexOf(F("STATUS_LANTAI_3")) >= 0){
      isEmg3 = 0;
    }
    if(fromTopic.indexOf(F("STATUS_LANTAI_4")) >= 0){
      isEmg4 = 0;
    }
  }

  if(fromTopic.indexOf(F("STATUS_LANTAI_1")) >= 0){
    client.publish(RPY_TOPIC_1, "OK");
  } else
  if(fromTopic.indexOf(F("STATUS_LANTAI_2")) >= 0){
    client.publish(RPY_TOPIC_2, "OK");
  } else 
  if(fromTopic.indexOf(F("STATUS_LANTAI_3")) >= 0){
    client.publish(RPY_TOPIC_3, "OK");
  } else 
  if(fromTopic.indexOf(F("STATUS_LANTAI_4")) >= 0){
    client.publish(RPY_TOPIC_4, "OK");
  }
}

/*
 * Penjelasan ada di file 1 nya
 */
void reconnect() {  
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    // Attempt to connect
    if (client.connect(CLIENT_ID)) {
      Serial.println(F("connected"));

      // beritahu ke node lain, kalau master up kembali jika terjadi putus koneksi
      client.publish(RPY_TOPIC_1, "OK");
      client.publish(RPY_TOPIC_2, "OK");
      client.publish(RPY_TOPIC_3, "OK");
      client.publish(RPY_TOPIC_4, "OK");
      /*
       * Penjelasan ada di file 1 nya
       */
      client.subscribe(ESP_TOPIC_1);
      client.subscribe(ESP_TOPIC_2);
      client.subscribe(ESP_TOPIC_3);
      client.subscribe(ESP_TOPIC_4);
      client.subscribe(APP_TOPIC);
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*
 * Penjelasan ada di file 1 nya
 */
void alarmStop(){
  noTone(spkPin, BUZZER_CHANNEL);
}

/*
 * Penjelasan ada di file 1 nya
 */
void alarmMedium(){
  tone(spkPin, NOTE_F4, 500, BUZZER_CHANNEL);
  noTone(spkPin, BUZZER_CHANNEL);
  tone(spkPin, NOTE_C4, 500, BUZZER_CHANNEL);
  noTone(spkPin, BUZZER_CHANNEL);
}

/*
 * Penjelasan ada di file 1 nya
 */
void alarmHigh(){
  tone(spkPin, NOTE_F7, 500, BUZZER_CHANNEL);
  noTone(spkPin, BUZZER_CHANNEL);
  tone(spkPin, NOTE_C7, 500, BUZZER_CHANNEL);
  noTone(spkPin, BUZZER_CHANNEL);  
}
