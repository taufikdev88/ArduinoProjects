
/*
 * Distance Sensor Configuration and Sensor
 */
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
//  return digitalRead(cfSen);
  
  uint8_t n = 0;
  for(uint8_t i=0; i<10; i++){
    n += (digitalRead(cfSen) ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 5 ? true : false);
}
bool isRfDist(){
//  return digitalRead(rfSen);
  
  uint8_t n = 0;
  for(uint8_t i=0; i<10; i++){
    n += (digitalRead(rfSen) ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 5 ? true : false);
}
bool isLfDist(){
//  return digitalRead(lfSen);
  
  uint8_t n = 0;
  for(uint8_t i=0; i<10; i++){
    n += (digitalRead(lfSen) ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 5 ? true : false);
}
bool isCrDist(){
//  return digitalRead(crSen);
  
  uint8_t n = 0;
  for(uint8_t i=0; i<10; i++){
    n += (digitalRead(crSen) ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 5 ? true : false);
}
bool isClDist(){
//  return analogRead(clSen);
  
  uint8_t n = 0;
  for(uint8_t i=0; i<10; i++){
    n += (analogRead(clSen) > 512 ? 1 : 0);
    delayMicroseconds(delayDistSensor);
  }
  return ( n >= 5 ? true : false);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupDistSensor();
}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.println((String) isClDist() + '\t' + isLfDist() + '\t' + isCfDist() + '\t' + isRfDist() + '\t' + isCrDist());
    delay(100);
}
