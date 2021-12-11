#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

// Set these to run example.
#define FIREBASE_HOST "mysmarthome-822fd-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Ueu6yR162x8BmhEtP8bS7RfgmuMEoaUJspIBlKof"
#define WIFI_SSID "RumahNikita"
#define WIFI_PASSWORD "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {
  
}
