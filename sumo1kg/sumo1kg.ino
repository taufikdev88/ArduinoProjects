#include <EEPROM.h>
#include <Psx.h>
#define dataPin 12  //coklat==putih
#define cmndPin 11  //abang==abu-abu
#define attPin 10   //ijo==ijo
#define clockPin 13 //biru==kuning

int last_data = 12312;
int data_stick = 0;
unsigned long delRc = 0;
Psx Psx;
#define atas 8
#define kanan 4
#define kiri 1
#define bawah 2
#define K 256
#define X 512
#define O 1024
#define A 2048     
#define R1 4096
#define L1 8192
#define R2 16384
#define L2 -32768

#define LED1 A4
#define LED2 A5
unsigned long delLed = millis();
bool state = false;

/*
 * Robot Mode
 */
#define standby 0
#define calib 1
#define startingright 2
#define startingleft 3
#define search 4
#define attack 5
#define escape 6
uint8_t robotMode = standby;

// delay untuk rotasi pertama saat starting
#define startingDelay 100
/*
 * Motor Configuration and Function
 */
// pwm offset adalah efek zigzag saat melakukan attacking
// minus turning adalah pengurangan nilai pwm untuk rotasi di mode search agar tidak terlalu over
#define pwmOffset 0
#define minusTurning 50 

// motor Left
#define outPwmL1 3  
#define outPwmL2 5
#define outPwmLMax 255

// motor Right
#define outPwmR1 6 
#define outPwmR2 9
#define outPwmRMax 255

void setupMotor(){
  pinMode(outPwmL1, OUTPUT);
  pinMode(outPwmL2, OUTPUT);
  pinMode(outPwmR1, OUTPUT);
  pinMode(outPwmR2, OUTPUT);
}
void mKiFw(uint8_t pwm){
  analogWrite(outPwmL1, pwm);
  analogWrite(outPwmL2, 0);
}
void mKiBw(uint8_t pwm){
  analogWrite(outPwmL1, 0);
  analogWrite(outPwmL2, pwm);
}
void mKaFw(uint8_t pwm){
  analogWrite(outPwmR1, 0);
  analogWrite(outPwmR2, pwm);
}
void mKaBw(uint8_t pwm){
  analogWrite(outPwmR1, pwm);
  analogWrite(outPwmR2, 0);
}

/*
 * Distance Sensor Configuration and Sensor
 */
// jarak sampling pada 1 sensor dalam micro seconds
#define delayDistSensor 10

#define cfSen 4
#define rfSen 2
#define lfSen 7
#define crSen 8
#define clSen A7

void setupDistSensor(){
  pinMode(cfSen, INPUT);
  pinMode(rfSen, INPUT);
  pinMode(lfSen, INPUT);
  pinMode(crSen, INPUT);
  pinMode(clSen, INPUT);
}
bool isCfDist(){
  uint8_t n = 0;
  for(uint8_t i=0; i<10; i++){
    n += (digitalRead(cfSen) ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 8 ? true : false);
}
bool isRfDist(){
  uint8_t n = 0;
  for(uint8_t i=0; i<200; i++){
    n += (digitalRead(rfSen) ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 180 ? true : false);
}
bool isLfDist(){
  uint8_t n = 0;
  for(uint8_t i=0; i<200; i++){
    n += (digitalRead(lfSen) ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 180 ? true : false);
}
bool isCrDist(){
  uint8_t n = 0;
  for(uint8_t i=0; i<200; i++){
    n += (digitalRead(crSen) ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 180 ? true : false);
}
bool isClDist(){
  uint8_t n = 0;
  for(uint8_t i=0; i<200; i++){
    n += (analogRead(clSen) > 512 ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 180 ? true : false);
}

/*
 * Line Sensor Configuration and Sensor
 */
// jarak sampling pada 1 sensor dalam microseconds
#define delayLineSensor 10
// offset diatas nilai minimal garis putih
#define lineOffset 80

uint16_t trimRf = 1024;
uint16_t trimLf = 1024;
uint16_t trimCb = 1024;
 
#define rfLine A0
#define lfLine A1
#define cbLine A2

void setupLineSensor(){
  pinMode(rfLine, INPUT);
  pinMode(lfLine, INPUT);
  pinMode(cbLine, INPUT);
  readTrimLineSensor();
}
void readTrimLineSensor(){
  for(uint8_t i=0; i<3; i++){
    switch(i){
      case 0:
      trimRf = EEPROM.read(i) * 4;
      break;
      case 1:
      trimLf = EEPROM.read(i) * 4;
      break;
      case 2:
      trimCb = EEPROM.read(i) * 4;
      break;
    }
  }
  Serial.println((String) "Got from EEPROM: " + trimRf + '\t' + trimLf + '\t' + trimCb);
  delay(3000);
}
void trimLineSensor(){
  Serial.println((String) "Write to EEPROM: " + '\t' + trimRf + '\t' + trimLf + '\t' + trimCb);
  for(uint8_t i=0; i<3; i++){
    switch(i){
      case 0:
      EEPROM.write(i, trimRf / 4.0);
      break;
      case 1:
      EEPROM.write(i, trimLf / 4.0);
      break;
      case 2:
      EEPROM.write(i, trimCb / 4.0);
      break;
    }
  }
  delay(3000);
}
bool isRfLine(){
  uint8_t n = 0;
  for(uint8_t i=0; i<10; i++){
    n += (analogRead(rfLine) < trimRf ? 1 : 0);
    delayMicroseconds(delayLineSensor);
  }
  return (n > 5 ? true : false);
}
bool isLfLine(){
  uint8_t n = 0;
  for(uint8_t i=0; i<10; i++){
    n += (analogRead(lfLine) < trimLf ? 1 : 0);
    delayMicroseconds(delayLineSensor);
  }
  return (n > 5 ? true : false);
}
bool isCbLine(){
  uint8_t n = 0;
  for(uint8_t i=0; i<10; i++){
    n += (analogRead(cbLine) < trimCb ? 1 : 0);
    delayMicroseconds(delayLineSensor);
  }
  return (n > 5 ? true : false);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, 0);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, 0);
  digitalWrite(LED2, 0);
  
  setupMotor();
  setupDistSensor();
  setupLineSensor();

  robotMode = standby;
}

bool testCalib = false;
void loop() {
  data_stick = Psx.read();
  Serial.print(data_stick);
  Serial.print('\t');
  
    /*
     * Robot standby Diam
     */
  if(robotMode == standby){
    Serial.println((String) isClDist() + '\t' + isLfDist() + '\t' + isCfDist() + '\t' + isRfDist() + '\t' + isCrDist() + "\t line: \t" + isLfLine() + '\t' + isCbLine() + '\t' + isRfLine());

    testCalib = false;
    digitalWrite(LED1, 0);
    digitalWrite(LED2, 0);
    mKaFw(0);
    mKiFw(0);
    
    if(data_stick != last_data && (unsigned long) millis()-delRc>500){
      switch(data_stick){
        case A: robotMode = calib; break;
        case kanan: robotMode = startingright; break;
        case kiri: robotMode = startingleft; break;
        case atas: robotMode = search; break;
      }
      last_data = data_stick;
      delRc = millis();
    }
  }
    /*
     * Robot kalibrasi garis
     */
  else if(robotMode == calib){
    if(data_stick != last_data && (unsigned long) millis()-delRc>500){
      switch(data_stick){
        case X: 
        // keluar dari mode kalibari dengan menyimpan nilainya ke eeprom
        trimLineSensor();
        robotMode = standby;
        break;
        // untuk keluar dari mode kalibrasi tanpa menyimpan nilai ke eeprom
        case O:
        readTrimLineSensor();
        robotMode = standby;
        break;
        // untuk tes apakah nilai sensor sudah baik, tekan Kotak lalu arahkan sensor ke garis
        // jika robot bereaksi pada masing masing sensor garis, maka kalibrasi berhasil
        case K:
        testCalib = !testCalib;
        break;
      }
      last_data = data_stick;
      delRc = millis();
    }
    if((unsigned long) millis()-delLed>500){
      delLed = millis();
      digitalWrite(LED1, state);
      digitalWrite(LED2, !state);
      state = !state;
    }

    if(analogRead(rfLine) + lineOffset < trimRf) trimRf = analogRead(rfLine) + lineOffset;
    if(analogRead(lfLine) + lineOffset < trimLf) trimLf = analogRead(lfLine) + lineOffset;
    if(analogRead(cbLine) + lineOffset < trimCb) trimCb = analogRead(cbLine) + lineOffset;
    
    //realTime checking  
    if(isRfLine() && testCalib){
      Serial.println("Kanan");
      mKaFw(50);
      mKiBw(50);
      return;
    }
    if(isLfLine() && testCalib){
      Serial.println("Kiri");
      mKaBw(50);
      mKiFw(50);
      return;
    }
    if(isCbLine() && testCalib){
      Serial.println("Belakang");
      mKaFw(50);
      mKiFw(50);
      return;
    }
    Serial.println((String) "Calib\t" + trimLf + '\t' + trimCb + '\t' + trimRf);
    mKaFw(0);
    mKiFw(0);
  }
    /*
     * Posisi awal dimana start ambil ke kanan dahulu, lalu serong belok kiri menuju search
     */
  else if(robotMode == startingright){
    // rotasi ke kanan
    mKaBw(outPwmRMax);
    mKiFw(outPwmLMax);
    delay(startingDelay);
    // maju sampai ....
    mKaFw(outPwmRMax);
    mKiFw(outPwmLMax);
    delRc = millis();
    // ... 1,5 detik full atau ...
    while((unsigned long) millis()-delRc <= 1500){
      // ... terdeteksi garis tepi, maka masuk mode search atau ...
      if(isLfLine() || isRfLine()){
        mKaBw(outPwmRMax);
        mKiBw(outPwmLMax);
        break;
      }
      // ... terdeteksi ada lawah di sebelah robot
      if(isCrDist() || isClDist()){
        break;
      }
    }
    robotMode = search;
  }
    /*
     * Posisi awal dimana start ambil ke kiri dahulu, lalu serong belok kanan menuju search
     */
  else if(robotMode == startingleft){
    mKaFw(outPwmRMax);
    mKiBw(outPwmLMax);
    delay(startingDelay);
    mKaFw(outPwmRMax);
    mKiFw(outPwmLMax);
    delRc = millis();
    while((unsigned long) millis()-delRc <= 1500){
      if(isLfLine() || isRfLine()){
        mKaBw(outPwmRMax);
        mKiBw(outPwmLMax);
        break;
      }
      if(isCrDist() || isClDist()){
        break;
      }
    }
    robotMode = search;
  }
    /*
     * Posisi dimana robot rotasi mencari robot lawan
     */
  else if(robotMode == search){
    digitalWrite(LED1, 1);
    digitalWrite(LED2, 1);
    if(data_stick != last_data && (unsigned long) millis()-delRc>500){
      if(data_stick == O){
        robotMode = standby;
        return;
      }
      last_data = data_stick;
      delRc = millis();
    }
    // saat maju dalam mode search tapi mencapai garis tepi, mundur dan rotasi ke kiri
    if(isLfLine()){
      mKaBw(outPwmRMax);
      mKiBw(outPwmLMax);
      delay(300);
      mKaBw(outPwmRMax);
      mKiFw(outPwmLMax);
      delay(100);
      return;
    }
    // saat maju dalam mode search tapi mencapai garis tepi, mundur dan rotasi ke kanan
    if(isRfLine()){
      mKaBw(outPwmRMax);
      mKiBw(outPwmLMax);
      delay(300);
      mKaFw(outPwmRMax);
      mKiBw(outPwmLMax);
      delay(100);
      return;
    }    
    // jika ada terdeteksi lawan di sebelah kanan robot , rotasi sampai menghadap lawan
    if(isCrDist()){
      mKaBw(outPwmRMax-minusTurning);
      mKiFw(outPwmLMax-minusTurning);
      delLed = millis();
      while(!isCfDist()){
        if((unsigned long) millis() - delLed > 1000) break;
      }
      goto gaskan;
    }
    if(isClDist()){
      mKaFw(outPwmRMax-minusTurning);
      mKiBw(outPwmLMax-minusTurning);
      delLed = millis();
      while(!isCfDist()){
        if((unsigned long) millis() - delLed > 1000) break;
      }
      goto gaskan;
    }
    // jika lawan sudah didepan robot, lakukan mode attack
    if(isCfDist() && isLfDist() && isRfDist()){
      robotMode = attack;
      return;
    }
    gaskan:
    mKaFw(outPwmRMax);
    mKiFw(outPwmLMax);
  }
    /*
     * posisi dimana robot melakukan attacking ke robot lawan
     */
  else if(robotMode == attack){
    if(data_stick != last_data && (unsigned long) millis()-delRc>500){
      if(data_stick == O){
        robotMode = standby;
        return;
      }
      last_data = data_stick;
      delRc = millis();
    }
    
    // lakukan pengaman berdasar sensor garis
    if(isRfLine() && !isLfLine()){
      mKaFw(outPwmRMax);
      mKiBw(outPwmLMax);
      while(isRfLine());
      robotMode = search;
    } 
    else if(isLfLine() && !isRfLine()){
      mKaBw(outPwmRMax);
      mKiFw(outPwmLMax);
      while(isLfLine());
      robotMode = search;
    }
    else if(isRfLine() && isLfLine()){
      mKaBw(outPwmRMax);
      mKiBw(outPwmLMax);
      while(isLfLine() || isRfLine());
      robotMode = search;
    }
    if(isCbLine()){
      mKaFw(outPwmRMax-100);
      mKiFw(outPwmLMax);
      delay(300);
      mKaFw(outPwmRMax);
      mKiFw(outPwmLMax);
      delay(300);
      robotMode = search;
    }
    
    uint8_t d = 0;
    if(isLfDist()) d = d | 0b100;
    if(isCfDist()) d = d | 0b010;
    if(isRfDist()) d = d | 0b001;

    // jika dalam mode attack, lawan sudah tidak ada, ganti ke mode search
    if(d == 0){
      robotMode = search;
      return;
    }
    // fungsi zigzag
    if((unsigned long) millis()-delLed>500){
      delLed = millis();
      digitalWrite(LED1, state);
      digitalWrite(LED2, state);
      
      if(state){
        mKaFw(outPwmRMax);
        mKiFw(outPwmLMax-pwmOffset);
      } else {
        mKaFw(outPwmRMax-pwmOffset);
        mKiFw(outPwmLMax);
      }
      state = !state;
    }
  }
    /*
     * Posisi robot di tabrak dari samping maka, lakukan rotasi dari arah tabrakannya lalu lakukan attack
     * Note: Masih belum fix, belum dipakai
     */
  else if(robotMode == escape){
    while(isCrDist()){
      mKiFw(outPwmLMax);
      mKaBw(outPwmRMax);
      if((unsigned long) millis()-delLed > 500){
        delLed = millis();
        digitalWrite(LED1, state);
        digitalWrite(LED2, state);
        state = !state;
      }
    }
    while(isClDist()){
      mKaFw(outPwmRMax);
      mKiBw(outPwmLMax);
      if((unsigned long) millis()-delLed > 500){
        delLed = millis();
        digitalWrite(LED1, state);
        digitalWrite(LED2, state);
        state = !state;
      }
    }
    robotMode = search;
  }
}
