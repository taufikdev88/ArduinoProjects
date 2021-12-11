#define senL 2
#define senR 3
//#define isSenR() digitalRead(senR)
//#define isSenL() digitalRead(senL)

//============== Filtering Left and Right Sensor =================
bool isSenR(void){
  int senVal=0;
  for(int i=0; i<10; i++){
    if(digitalRead(senR)) senVal++;
    delayMicroseconds(10);
  }
  if(senVal>3) return true;
  else return false;
}
bool isSenL(void){
  int senVal=0;
  for(int i=0; i<10; i++){
    if(digitalRead(senL)) senVal++;
    delayMicroseconds(10);
  }
  if(senVal>3) return true;
  else return false;
}

void timerSetup(){
  attachInterrupt(digitalPinToInterrupt(senL), interruptKi, CHANGE);
  attachInterrupt(digitalPinToInterrupt(senR), interruptKa, CHANGE);
}
void timerShutdown(){
  detachInterrupt(digitalPinToInterrupt(senR));
  detachInterrupt(digitalPinToInterrupt(senL));
}

#define d 10
volatile bool lock = false;
volatile bool flagVs = false;
volatile unsigned long delayInt = millis();
void interruptKi(){
  if(lock == false && (unsigned long) millis()-delayInt > 500){
    Serial.println("interrupt kiri");
    delayInt = millis();
    timerShutdown();
    lock = true; 
    flagVs = false;
    delay(100);
    while(isSenL()){
      delay(d);
      if(isSenR()) flagVs = true;
    }
    if(flagVs){
      Serial.println("per4");
    } else {
      Serial.println("per3l");
    }
    lock = false;
    timerSetup();
  }
}

void interruptKa(){
  if(lock == false && (unsigned long) millis()-delayInt > 500){
    Serial.println("interrupt kanan");
    delayInt = millis();     
    timerShutdown();
    lock = true;
    flagVs = false;
    delay(100);
    while(isSenR()){
      delay(d);
      if(isSenL()) flagVs = true;
    }
    if(flagVs){
      Serial.println("per4");
    } else {
      Serial.println("per3r");
    }
    lock = false;
    timerSetup();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  timerSetup();
}

void loop() {
  // put your main code here, to run repeatedly:

}
