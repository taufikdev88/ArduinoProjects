#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define dirPin1   7 //stepper 1 paling kiri tampak depan
#define stepPin1  6
#define dirPin2   9 //stepper 2 paling kiri tampak depan
#define stepPin2  8
#define dirPin3   11  //stepper 3 paling kiri tampak depan
#define stepPin3  10
#define dirPin4   13  //stepper 4 paling kiri tampak depan
#define stepPin4  12

#define stepsPerRevolutionMj 520
#define stepsPerRevolutionMd 2000
int kecepatan = 500;//Delay tambah besar tambah pelan

#define R 28
#define G 30
#define B 26

#define limitD1   A14 //depan paling kanan
#define limitD2   A12
#define limitD3   A10
#define limitD4   A8  //depan paling kiri
#define limitB1   A6  //belakang paling kanan
#define limitB2   A4
#define limitB3   A2
#define limitB4   A0  //belakang paling kiri

char keys[] = {
  'F',  'G', '#', '*',
  '1',  '2', '3', 'U',
  '4',  '5', '6', 'D',
  '7',  '8', '9', 'E',
  'L',  '0', 'R', 'N'
};

const byte ROWS = 5;
const byte COLS = 4;
byte rowPins[ROWS] = {41, 42, 43, 44, 45}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {49, 48, 47, 46}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char key;

int8_t stok[4] = {4, 4, 4, 4};
int8_t obat = 0;
int8_t jumlah = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(stepPin4, OUTPUT);
  pinMode(dirPin4, OUTPUT);

  pinMode(limitD1, INPUT_PULLUP);
  pinMode(limitD2, INPUT_PULLUP);
  pinMode(limitD3, INPUT_PULLUP);
  pinMode(limitD4, INPUT_PULLUP);
  pinMode(limitB1, INPUT_PULLUP);
  pinMode(limitB2, INPUT_PULLUP);
  pinMode(limitB3, INPUT_PULLUP);
  pinMode(limitB4, INPUT_PULLUP);
  
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  mundur(stepPin1, limitB1);
  mundur(stepPin2, limitB2);
  mundur(stepPin3, limitB3);
  mundur(stepPin4, limitB4);
  
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);
  delay(500);
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(B, LOW);
  delay(500);
  digitalWrite(R, LOW);
  digitalWrite(G, LOW);
  digitalWrite(B, HIGH);
  delay(500);
  digitalWrite(R, LOW);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Selamat Datang");
  delay(500);
  lcd.clear();
}

unsigned long timingDisplay = 0;
void loop() {  
  key = keypad.getKey();
  if(key){
    Serial.println(key);
    switch(key){
      case '1':
      obat = 1;
      jumlah = 0;
      break;
      case '2':
      obat = 2;
      jumlah = 0;
      break;
      case '3':
      obat = 3;
      jumlah = 0;
      break;
      case '4':
      obat = 4;
      jumlah = 0;
      break;
      case 'R':
      case 'U':
      if(obat > 0){
        if(stok[obat-1] > jumlah){
          jumlah++;
        }
      }
      break;
      case 'L':
      case 'D':
      if(jumlah > 1) jumlah--;
      break;
      case 'N':
      proses();
      break;
    }
  }

  if((unsigned long) millis()-timingDisplay >= 500){
    showDisplay();
    cekStepper();
    
    Serial.println(readLimit(limitD1));
    Serial.println(readLimit(limitD2));
    Serial.println(readLimit(limitD3));
    Serial.println(readLimit(limitD4));
    Serial.println("-");
    delay(100);
    timingDisplay = millis();
  }
}

void showDisplay(){
  lcd.setCursor(0,0);
  lcd.print("     ROBOTOBAT      ");
  lcd.setCursor(0,1);
  lcd.print("OBAT 1:");
  lcd.print(stok[0]);
  lcd.setCursor(10,1);
  lcd.print("OBAT 2:");
  lcd.print(stok[1]);
  lcd.setCursor(0,2);
  lcd.print("OBAT 3:");
  lcd.print(stok[2]);
  lcd.setCursor(10,2);
  lcd.print("OBAT 4:");
  lcd.print(stok[3]);
  lcd.setCursor(0,3);
  lcd.print("Pilih:");
  lcd.print(obat);
  lcd.setCursor(10,3);
  lcd.print("Jumlah:");
  lcd.print(jumlah);
}

void maju(const int16_t pin, const int16_t lm){
  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, HIGH);
  digitalWrite(dirPin3, HIGH);
  digitalWrite(dirPin4, HIGH);
  
  for (int i = 0; i < (long) stepsPerRevolutionMj * jumlah; i++) {
    if (readLimit(lm)){
      Serial.println("break");
      break;
    }
    digitalWrite(pin, HIGH);
    delayMicroseconds(kecepatan);
    digitalWrite(pin, LOW);
    delayMicroseconds(kecepatan);
  }
}

void mundur(int16_t pin, int16_t lm){
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);
  digitalWrite(dirPin3, LOW);
  digitalWrite(dirPin4, LOW);
  
  for (int i = 0; i < stepsPerRevolutionMd; i++) {
    if (readLimit(lm)) break;
    digitalWrite(pin, HIGH);
    delayMicroseconds(kecepatan);
    digitalWrite(pin, LOW);
    delayMicroseconds(kecepatan);
  }
}

void proses(){
  int16_t pin = 0;
  int16_t lm = 0;
  switch(obat){
    case 1:
    pin = stepPin1;
    lm = limitD1;
    break;
    case 2:
    pin = stepPin2;
    lm = limitD2;
    break;
    case 3:
    pin = stepPin3;
    lm = limitD3;
    break;
    case 4:
    pin = stepPin4;
    lm = limitD4;
    break;
    default:
    return;
    break;
  }
  Serial.print("obat:");
  Serial.println(obat);
  Serial.print("jumlah:");
  Serial.println(jumlah);
  stok[obat-1] -= jumlah;
  maju(pin, lm);
  obat = 0;
  jumlah = 0;
}

void cekStepper(){
  if(readLimit(limitD1)){
    mundur(stepPin1, limitB1);
  }  
  if(readLimit(limitD2)){
    mundur(stepPin2, limitB2);
  }  
  if(readLimit(limitD3)){
    mundur(stepPin3, limitB3);
  }  
  if(readLimit(limitD4)){
    mundur(stepPin4, limitB4);
  }  
}

bool readLimit(int16_t pin){
  uint8_t cnt = 0;
  for(uint8_t i=0; i<10; i++){
    if(digitalRead(pin) == 0) cnt++;
    delayMicroseconds(10);
  }
  if(cnt > 5) return true;
  return false;
}
