/*
 * File untuk node yang diletakkan di lantai-lantai
 * 
 * WiFi adalah library yg digunakan untuk mengakses modul wifi esp32
 * PubSubClient adalah library yg digunakan untuk mempermudah koneksi dengan MQTT Server (atau sering disebut dengan broker)
 * NTPClient adalah library untuk mendapatkan waktu sekarang secara realtime dari internet
 * WiFiUdp adalah library tambahan untuk koneksi UDP
 * Tone32 adalah library untuk menyalakan alarm biar bisa ada tangga nada
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Tone32.h>

/*
 * SSID adalah nama wifi yang akan digunakan sebagai jalur koneksi internet
 * PASS adalah password dari wifi tersebut
 */
#define SSID "RumahNikita"
#define PASS "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd"

/*
 * Ini adalah broker mqtt public (gratisan) yang bisa langsung dipakai
 * port dari server ini adalah 1883
 */
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883

/*
 * Ini adalah TOPIC yang digunakan oleh node-node untuk menandai datanya setiap lantai ESP_TOPIC nya berbeda nampun APP_TOPIC nya sama karena memakai 1 aplikasi
 * Client ID disesuaikan dengan id masing masing node
 */
#define ESP_TOPIC "rifandiUBAYA/STATUS_LANTAI_2"
#define MTR_TOPIC "rifandiUBAYA/REPLY_LANTAI_2"

#define APP_TOPIC "rifandiUBAYA/APP"

#define CLIENT_ID "lantai-2-client"

/*
 * Letak pin dari perangkat button dan speaker
 */
#define btnPin 34
#define spkPin 4
#define BUZZER_CHANNEL 0

/*
 * Inisialisasi koneksi udp untuk menjembatani NTP mendapatkan waktu dari internet
 */
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000);

/*
 * Inisialisasi objek untuk koneksi ke MQTT Broker
 */
WiFiClient wifiClient;
PubSubClient client(wifiClient);

/*
 * variable timing digunakan untuk menyimpan jarak waktu pengiriman paket ke mqtt servernya
 * variable state digunakan untuk menyimpan status perangkat 0 (aman) , 1 (bahaya-belum konfirm) , 2 (bahaya konfirm)
 * variable isSafe digunakan untuk toggle mematikan speaker agar program berjalan seperti biasa
 * variable isReplied digunakan untuk memastikan mendapatkan reply dari node master dan baru mengirim data lagi setelah mendapat balasan
 */
unsigned long timing = 0;
uint8_t state = 0;
bool isSafe = 1;
bool isReplied = 1;
bool isDangerReplied = 0;

/*
 * Fungsi callback ini merupakan fungsi yang dijalankan saat mendapatkan data dari mqtt server
 */
void callback(char* topic, byte* payload, unsigned int length) {
  /*
   * fungsi serial merupakan fungsi untuk melihat saja data yang didapat ke laptop, data disalurkan melalui usb, hanya untuk melihat, 
   * tidak mempengaruhi jalannya program
   */
  Serial.print(F("Message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));

  /*
   * Menyiapkan variabel penampung data yang masuk, yaitu dataIn
   */
  String dataIn = "";
  /*
   * data yang masuk , disimpan ke dataIn agar bisa diproses selanjutnya
   */
  for (int i=0;i<length;i++) {
    dataIn += (char) payload[i];
  }

  /*
   * Sekali lagi dikirimkan ke laptop (jika tersambung) untuk melihat data yang masuk
   */
  Serial.print(dataIn);
  Serial.print(F(", State: "));
  Serial.print(state);
  Serial.println();

  /*
   * nah ini adalah pemrosesan datanya, jika di data tersebut menemukan kata CONFIRM 
   * alias kata tersebut pasti dikirim oleh aplikasi, maka ganti state menjadi 2
   * efeknya alarm akan menyala keras
   */
  if(dataIn.indexOf("CONFIRM") >= 0){
    state = 2;
  }
  /*
   * Jika ada pesan OK dari master, tandai kalau sudah di balas oleh master
   */
  if(dataIn.indexOf("OK") >= 0){
    isReplied = 1;
    if(state != 0){
      isDangerReplied = 1;
    }
  }
}

/*
 * Fungsi ini akan dijalankan saat gagal melakukan koneksi ke mqtt server / saat koneksi terputus
 */
void reconnect() {  
  // selama belum tersambung
  while (!client.connected()){
    Serial.print(F("Attempting MQTT connection..."));
    // Mencoba untuk menyambung server dengan client id yang telah dideklarasikan diatas
    if (client.connect(CLIENT_ID)) {
      Serial.println(F("connected"));
      
      // setelah berhasil tersambung kirim data ke servernya menggunakan topic yang telah dideklarasikan diatas dengan kata "Register" pasti android dan esp satpam
      // akan menerima kata Register ini
      client.publish(ESP_TOPIC, "Register");

      // saat pertama kali konek ke mqtt server , lakukan subcribe untuk mendapatkan data
      // subcribe ke reply dari master
      client.subscribe(MTR_TOPIC);
      // lalu subcribe ke topiknya aplikasi, agar saat aplikasi mengirim kata CONFIRM, node lantai ini bisa mendapatkan data tersebut
      client.subscribe(APP_TOPIC);
    } else {
      // jika gagal terkonek , tampilkan ke laptop bahwa gagal konek
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      // tunggu 5 detik untuk mulai lagi
      delay(5000);
    }
  }
}

/*
 * ini adalah fungsi untuk mematikan alarm
 */
void alarmStop(){
  noTone(spkPin, BUZZER_CHANNEL);
}

/*
 * ini adalah fungsi untuk menyalakan alarm dengan volume medium diatur menggunakan tangga nada F4 dan C4
 */
void alarmMedium(){
  tone(spkPin, NOTE_F4, 500, BUZZER_CHANNEL);
  noTone(spkPin, BUZZER_CHANNEL);
  tone(spkPin, NOTE_C4, 500, BUZZER_CHANNEL);
  noTone(spkPin, BUZZER_CHANNEL);
}

/*
 * ini adalah fungsi untuk menyalakan alarm dengan volume tinggi diatur menggunakan tangga nada F7 dan C7
 */
void alarmHigh(){
  tone(spkPin, NOTE_F7, 500, BUZZER_CHANNEL);
  noTone(spkPin, BUZZER_CHANNEL);
  tone(spkPin, NOTE_C7, 500, BUZZER_CHANNEL);
  noTone(spkPin, BUZZER_CHANNEL);
}

//***********************************************************************************************************************************
/*
 * Fungsi setup ini akan dijalankan pertama kali saat esp menyala, dan hanya dijalankan sekali
 */
void setup()
{
  // inisialisasi serial, agar esp dapat berkomunikasi dengan laptop melalui USB
  // jika USB tidak tersambung pun tidak masalah
  Serial.begin(115200);

  // inisialisasi tombol emergency sebagai input
  pinMode(btnPin, INPUT);
  
  // melakukan penyambungan wifi menggunakan SSID dan PASS yang telah dideklarasikan diatas
  WiFi.begin(SSID, PASS);
  // selama status wifinya belum terkoneksi
  while(WiFi.status() != WL_CONNECTED){
    // tunggu terus
    delay(500);
    Serial.print(F("."));
  }

  // setelah terkoneksi , atur mqtt servernya ke server yang telah dideklarasikan diatas
  client.setServer(MQTT_SERVER, MQTT_PORT);
  // atus juga fungsi yang akan dijalankan saat ada paket yang diterima
  client.setCallback(callback);

  // lalu mulai juga untuk mengambil data jam sekarang secara realtime
  timeClient.begin();
}

/*
 * fungsi loop adalah fungsi yang dijalankan setelah fungsi setup selesai dijalankan
 * fungsi loop ini akan dijalankan terus menerus oleh esp
 */
void loop()
{
  // pertama tama pasti update tanggalnya dahulu dari internet
  timeClient.update();

  // cek jika mqtt servernya tidak tersambung
  if (!client.connected()) {
    // maka lakukan koneksi ulang
    reconnect();
  }
  // jika tersambung, jalankan fungsi bawaan dari library mqtt nya agar bisa melakukan pertukaran data
  client.loop();

  // ini akan mengatur periode pengiriman data oleh esp selama 1000 miliseconds sekali
  if((unsigned long) millis()-timing>=1000){
    // setiap 1000 ms sekali, cek kondisi button nya apakah ditekan atau tidak
    if(digitalRead(btnPin) == 0){
      // jika ditekan, siapkan data yang akan dikirim ke server yaitu, tanggal dan waktu sekarang ditambah kata DANGER
      String payload = timeClient.getFormattedTime() + " DANGER";
      // kirimkan datanya ke topic yang telah dideklarasikan diatas
      if(isReplied){ // cek dulu bahwa memang sudah dapat balasan dari master
        isReplied = 0;
        client.publish(ESP_TOPIC, payload.c_str());
      }
      // nah jika awal state nya adalah 0 ubah dulu ke 1 agar menyalakan alarm dengan suara medium
      if(state == 0){
        state = 1;
      }
    } else {
      // jika tombol emergency tidak ditekan, cukup siapkan data yang mau dikirim berupa tanggal waktu sekarang ditambah kata SAFE
      String payload = timeClient.getFormattedTime() + " SAFE";
      // kirimkan datanya ke topic yang telah dideklarasikan diatas
      if(isReplied){ // cek dulu kalau memang sudah dapat balasan dari master
        isReplied = 0; // reset kembali flag balasan, anggap sudah mengirim data baru dan menunggu balasan lagi
        client.publish(ESP_TOPIC, payload.c_str());
      }
      state = 0;
    }

    // update waktu terakhir pengecekan tombol agar fungsi pengecekan dan pengiriman data dilakukan selama 1000 ms sekali
    timing = millis();
  }

  /* 
   * pengecekan status dari esp ini dilakukan terus menerus tidak tergantung oleh 1000ms sekali, agar nyala alarm dapat baik
   * switch(state) artinya adalah melihat isi state
   * case 1: artinya jika isinya adalah 1
   * maka jalankan alarmMedium()
   * 
   * jika isinya 2 (case 2:)
   * jalankan alarmHigh();
   */
  switch(state){
    case 0:
    if(!isSafe){ // jika flag nya belum safe , maka matikan speaker hanya sekali saja
      alarmStop();
      isSafe = 1; // tandai kalau memang sudah safe
    }
    isDangerReplied = 0;
    break;
    case 1:
    isSafe = 0;
    if(isDangerReplied){
      alarmMedium(); 
    }
    break;
    case 2:
    isSafe = 0;
    if(isDangerReplied){
      alarmHigh(); 
    }
    break;
  }
}
