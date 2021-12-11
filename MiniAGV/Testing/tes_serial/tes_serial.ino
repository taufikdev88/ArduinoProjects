#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcdSetup(){
  lcd.begin();
  lcd.backlight();
  lcd.clear();
}
void lcdBar(uint8_t input, uint8_t x, uint8_t y){
  lcd.setCursor(x,y);
  if(input == 0){
    lcd.print(' ');
  } else {
    lcd.write(input);
  }
}
unsigned long delayUpdateLCD = millis();
void lcdLine0(String message){
  lcd.setCursor(0,0);
  lcd.print(message);
}
void lcdLine1(char *message){
  if((unsigned long) millis() - delayUpdateLCD >= 500){
    lcd.setCursor(0,1);
    lcd.print(message);
    delayUpdateLCD = millis();
  }
}

void setup() {
  // put your setup code here, to run once:
  lcdSetup();
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println("minta");
  Serial2.println("minta");
  delay(10);
  if(Serial2.available() > 0){
    String a = Serial2.readStringUntil('\n');
    Serial.println(a);
    lcdLine0(a);
    lcdLine0("                ");
  }
  delay(1000);
}
