/*
 * Agar BLYNK melakukan debugging ke Serial0
 */
#define BLYNK_PRINT Serial
/*
 * Load header library masing-masing modul
 * 1. WiFi: mengakses hardware wifi di esp32
 * 2. WiFiClient: membuat esp32 sebagai client wifi saja
 * 3. BlynkSimpleEsp32: agar bisa menggunakan koneksi ke server blynk
 * 4. max6675: library pembantu untuk membaca sensor max6675
 * 5. EEPROM: library yang membantu untuk baca/tulis ke eeprom
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
//#include <max6675.h>
#include <Adafruit_MLX90614.h>
#include "EEPROM.h"
/*
 * Blynk dan WiFi setting
 * 1. auth adalah key yang didapat dari blynk melalui email
 * 2. ssid adalah nama wifi
 * 3. pass adalah password wifi
 */
char auth[] = "cbmUwkMpuXIj3Snl5iTpPX-QKJgGNTnP";
char ssid[] = "RumahNikita";
char pass[] = "1WeKKEadyTKlGZ29EgqO5ndZSwjMpSRdcjJImvAltuCsQJE1pHJUclXmWzXrRTd";
/*
 * Pin configuration, hardware yg dipakai
 * 1. max6675: 3x
 * 2. led indicator: 3x
 * 3. buzzer: 1x
 * 4. lcd i2c: 1x
 * 5. button: 3x
 */
//// thermo 1
//#define D0_1  33
//#define CS_1  32
//#define CLK_1 27
//// thermo 2
//#define D0_2  26
//#define CS_2  25
//#define CLK_2 23
//// thermo 3
//#define D0_3  19
//#define CS_3  18
//#define CLK_3 17
// mlx address
#define IR1 0x5C
#define IR2 0x5A
#define IR3 0x5B
// button
#define btnUp 16
#define btnDw 15
#define btnSl 14
// human interface
#define buzz  13
#define led_1 5
#define led_2 4
#define led_3 2
WidgetLED led1(V17);
WidgetLED led2(V18);
WidgetLED led3(V19);
/*
 * Object declaration
 * 1. LiquidCrystal_I2C: object yang bisa mengakses lcd melalui komunikasi I2C
 * 2. MAX6675: object yang membantu membaca data dari sensor thermo couple
 */
LiquidCrystal_I2C lcd(0x27, 16, 2);
//MAX6675 thermoR(CLK_1, CS_1, D0_1);
//MAX6675 thermoS(CLK_2, CS_2, D0_2);
//MAX6675 thermoT(CLK_3, CS_3, D0_3);
Adafruit_MLX90614 mlx1 = Adafruit_MLX90614(IR1);
Adafruit_MLX90614 mlx2 = Adafruit_MLX90614(IR2);
Adafruit_MLX90614 mlx3 = Adafruit_MLX90614(IR3);
/*
 * Global variable
 */
// address untuk penyimpanan nilai referensi
#define rtemp1_addr 10
#define rtemp2_addr 11
#define rtemp3_addr 12
// nilai variable yg bisa diubah ubah
int rtemp1, rtemp2, rtemp3; // referensi/setpoint dari masing masing sensor
float temp1, temp2, temp3; // aktual yang didapat dari sensor temperatur
unsigned long lcdUpdate, btnUpdate, dataUpdate; // timing untuk lcd, button dan push data ke server blynk
int8_t menu = 0; // menyimpan menu untuk setting setpoint dari masing masing sensor secara manual
bool isOvertemp = false; // menyimpan status dari kondisi suhu
bool flipflop = 0; // untuk animasi flip flop

BLYNK_WRITE(V0){
  rtemp1 = param.asInt(); // dapatkan nilai dari server blynk
  rtemp1 = constrain(rtemp1, 0, 255); // berikan nilai minimal dan maksimal
  EEPROM.write(rtemp1_addr, rtemp1); // tulis ke addressnya eeprom
  EEPROM.commit(); // commit agar eeprom menyimpan nilainya
}

BLYNK_WRITE(V1){
  rtemp2 = param.asInt();
  rtemp2 = constrain(rtemp2, 0, 255);
  EEPROM.write(rtemp2_addr, rtemp2);
  EEPROM.commit();
}

BLYNK_WRITE(V2){
  rtemp3 = param.asInt();
  rtemp3 = constrain(rtemp3, 0, 255);
  EEPROM.write(rtemp3_addr, rtemp3);
  EEPROM.commit();
}

void setup() {
  Serial.begin(115200);
  /*
   * Tampilkan LCD pertama kali
   */
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.home();
  lcd.print("Setting UP");
  /*
   * Baca nilai eeprom untuk melihat terakhir kali nilai referensi di set
   */
  EEPROM.begin(64); // inisialisasi EEPROM
  rtemp1 = EEPROM.read(rtemp1_addr);
  rtemp2 = EEPROM.read(rtemp2_addr);
  rtemp3 = EEPROM.read(rtemp3_addr);
  Serial.print("Setpoint R: "); Serial.println(rtemp1);
  Serial.print("Setpoint S: "); Serial.println(rtemp2);
  Serial.print("Setpoint T: "); Serial.println(rtemp3);
  
  /*
   * Set pinMode untuk button, led dan buzzer
   */
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDw, INPUT_PULLUP);
  pinMode(btnSl, INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  digitalWrite(buzz, 0);
  digitalWrite(led_1, 0);
  digitalWrite(led_2, 0);
  digitalWrite(led_3, 0);
  /*
   * Mlx begin
   */
  mlx1.begin();
  mlx2.begin();
  mlx3.begin();
  /*
   * Inisialisasi Blynk
   */
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  /*
   * Wajib ada agar blynk selalu terkoneksi ke server
   */
  Blynk.run();

  /*
   * Button routine
   */
  if(isBtnUp() && (unsigned long) millis()-btnUpdate >= 500){
    btnUpdate = millis();
    switch(menu){
      case 1: rtemp1 += 1; break;
      case 2: rtemp2 += 1; break;
      case 3: rtemp3 += 1; break;
      default: menu = 1; break;
    }
  }
  if(isBtnDw() && (unsigned long) millis()-btnUpdate >= 500){
    btnUpdate = millis();
    switch(menu){
      case 1: rtemp1 -= 1; break;
      case 2: rtemp2 -= 1; break;
      case 3: rtemp3 -= 1; break;
      default: menu = 1; break;
    }
  }
  if(isBtnSl() && (unsigned long) millis()-btnUpdate >= 500){
    btnUpdate = millis();
    if(++menu > 3) menu = 0;
  }

  /*
   * LCD routine
   */
  if((unsigned long) millis()-lcdUpdate >= 500){
    lcdUpdate = millis();

    if(isOvertemp){
      lcd.setCursor(0,0);
      lcd.print("  SUHU TINGGI   ");
      lcd.setCursor(0,1);
      lcd.print(" TERDETEKSI!!!  ");
    } else {
      switch(menu){
        case 0:
        lcd.setCursor(0,0);
        lcd.print("T1:     T2:     ");
        lcd.setCursor(0,1);
        lcd.print("T3:        <1/4>");
        lcd.setCursor(3,0);
        lcd.print(temp1);
        lcd.setCursor(11,0);
        lcd.print(temp2);
        lcd.setCursor(3,1);
        lcd.print(temp3);
        break;
        case 1:
        lcd.setCursor(0,0);
        lcd.print(" SETPOINT SUHU1 ");
        lcd.setCursor(0,1);
        lcd.print("           <2/4>");
        lcd.setCursor(0,1);
        lcd.print(rtemp1);
        break;
        case 2:
        lcd.setCursor(0,0);
        lcd.print(" SETPOINT SUHU2 ");
        lcd.setCursor(0,1);
        lcd.print("           <3/4>");
        lcd.setCursor(0,1);
        lcd.print(rtemp2);
        break;
        case 3:
        lcd.setCursor(0,0);
        lcd.print(" SETPOINT SUHU3 ");
        lcd.setCursor(0,1);
        lcd.print("           <4/4>");
        lcd.setCursor(0,1);
        lcd.print(rtemp3);
        break;
        default:
        menu = 0;
        break;
      }
    }
  }

  /*
   * push data routine
   */
  if((unsigned long) millis()-dataUpdate >= 1000){
    dataUpdate = millis();

    /*
     * Baca suhu terbaru
     */
//    temp1 = thermoR.readCelsius();
//    temp2 = thermoS.readCelsius();
//    temp3 = thermoT.readCelsius();
    temp1 = mlx1.readObjectTempC();
    temp2 = mlx2.readObjectTempC();
    temp3 = mlx3.readObjectTempC();

    /*
     * Lakukan pengolahan data
     */
    if(temp1 > rtemp1) {
      led1.on();
      digitalWrite(led_1, 1); 
    } else {
      led1.off();
      digitalWrite(led_1, 0); // cek kondisi suhu untuk ditampilkan di masing masing led
    }
    if(temp2 > rtemp2) {
      led2.on();
      digitalWrite(led_2, 1);
    } else {
      led2.off();
      digitalWrite(led_2, 0);
    }
    if(temp3 > rtemp3) {
      led3.on();
      digitalWrite(led_3, 1); 
    } else {
      led3.off();
      digitalWrite(led_3, 0);
    }
    
    if(temp1 > rtemp1 || temp2 > rtemp2 || temp3 > rtemp3){ // tandai jika ada salah satu yg overtemp
      digitalWrite(buzz, 1);
      isOvertemp = true;
    } else {
      digitalWrite(buzz, 0);
      isOvertemp = false;
    }

    /*
     * Kirim data ke Blynk
     */
    Blynk.virtualWrite(V10, rtemp1);
    Blynk.virtualWrite(V11, rtemp2);
    Blynk.virtualWrite(V12, rtemp3);
    
    Blynk.virtualWrite(V13, temp1);
    Blynk.virtualWrite(V14, temp2);
    Blynk.virtualWrite(V15, temp3);
    
    Blynk.virtualWrite(V16, isOvertemp);
  }
}

#define MAX_DETECT 20
#define MIN_DETECT 15
bool isBtnUp(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnUp) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
bool isBtnDw(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnDw) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
bool isBtnSl(){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<MAX_DETECT; i++){
    if(digitalRead(btnSl) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > MIN_DETECT) return true;
  return false;
}
