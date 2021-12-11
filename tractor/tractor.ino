/*
 * Arduino Nano Old Bootloader
 */
#include <Servo.h>
//#include <Ultrasonic.h>

// Input
#define throt 9
#define steer 10

// Output
#define in1 3
#define in2 4
#define outThrot 5
#define outSteer 6
#define outBreak 8
#define breDefault 180
#define breAction 90

Servo sSer, sBre;
//Ultrasonic ultrasonic(A0, 11);
#define disPin 11
int distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(disPin, INPUT);
  
  pinMode(throt, INPUT);
  pinMode(steer, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(outThrot, OUTPUT);
  
  sSer.attach(outSteer);
  sBre.attach(outBreak);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(outThrot, 0);
  sSer.write(90);
  sBre.write(breDefault);
}

unsigned long pwmThrot = 0, pwmSteer = 0, delBreak = 0;
#define minPwmSteer 1000
#define maxPwmSteer 1900

#define minPwmThrot 980
#define trimPwmThrot 1470
#define maxPwmThrot 2000

void loop() {
  // ambil pwm throttle
  pwmThrot = pulseIn(throt, HIGH);
  delayMicroseconds(10);
  // ambil pwm steering
  pwmSteer = pulseIn(steer, HIGH);
  delayMicroseconds(10);
  // ambil nilai jarak
//  distance = ultrasonic.read();

  Serial.print((String) distance + '\t' + pwmThrot + '\t' + pwmSteer + '\t');

  // batasi dan ubah pwm steering
  if(pwmSteer >= minPwmSteer && pwmSteer <= maxPwmSteer){
    pwmSteer = map(pwmSteer, minPwmSteer, maxPwmSteer, 50, 140);
    sSer.write(pwmSteer);
  } else {
    sSer.write(90);
  }
  
  // batasi dan ubah pwm throttle
  if(pwmThrot >= minPwmThrot+50 && pwmThrot <= maxPwmThrot){
    if(pwmThrot > trimPwmThrot){
      // maju
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
//      if(distance >= 30){
      if(digitalRead(disPin)){
        Serial.print("\t Maju \t");
        delBreak = millis();
        sBre.write(breDefault);
        pwmThrot = map(pwmThrot, trimPwmThrot, maxPwmThrot, 0,  255); 
      } else {
        pwmThrot = 0;
        Serial.print("\t Halangan \t");
        if((unsigned long) millis()-delBreak <= 3000){
          sBre.write(breAction);
        } else {
          sBre.write(breDefault);
        }
      }
    } else {
      // mundur
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      sBre.write(breDefault);
      Serial.print("\t Mundur \t");
      pwmThrot = map(pwmThrot, trimPwmThrot, minPwmThrot, 0, 180);
    }
  } else {
    pwmThrot = 0;
    Serial.print("\t Diam \t");
    if((unsigned long) millis()-delBreak <= 3000){
      sBre.write(breAction);
    } else {
      sBre.write(breDefault);
    }
  }
  analogWrite(outThrot, pwmThrot);
  
  Serial.print((String) distance + '\t' + pwmThrot + '\t' + pwmSteer);
  Serial.println();
  delay(20);
}
