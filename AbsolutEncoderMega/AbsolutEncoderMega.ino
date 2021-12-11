// bool pinState[9] = {0};
// const uint8_t inputPin[9] = {2,4,6,8,10,12,14,16,18};
const uint8_t stepPin = 7;
const uint8_t dirPin = 9;
bool pinState[10] = {0};
const uint8_t inputPin[10] = {2,4,6,8,10,12,14,16,18,20};

int oldPosition = 0;
int encPosition = 0;
long realPosition = 0;
long realPositionMm = 0;

#define CW 0
#define CCW 1
bool dir = CW;

#define MAX_RESOLUTION 1023
#define stepToMmRatio 20

float Kp = 0.6;
float Ki = 0.01;
float Kd = 0.03;
float iError = 0;
float dError = 0;

long refPosition = 0;
long refPositionMm = 0;

int stepCorner = 0; // kasih 0 dulu, kalo button ditekan, nanti jadi 1
float distance,distanceX,distanceY;
float thetaSliderToCenterPoint,thetaToCenterPoint;
int8_t sign,signD;
const float rCircle = 135;
int degreePos = 90;

//definisikan posisi target target disini
#define centerPointX 150
#define centerPointY 150
#define targetX1 100
#define targetY1 100

const char *COMMA = ", ";

void setup() {
  Serial.begin(115200);
  Serial.println(F("hallo bos"));
  for(uint8_t i=0; i<10; i++){
    pinMode(inputPin[i], INPUT_PULLUP);
  }

  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  digitalWrite(stepPin, 1);
}

unsigned long logTiming = millis();
void loop() {
  if(Serial.available() > 0){
    String dataIn = Serial.readStringUntil('\n');
    refPositionMm = dataIn.toInt();
    if(refPositionMm == 12345) stepCorner = 1;
  }

  refPosition = refPositionMm * stepToMmRatio;
  readPosition();
  int delayStepper = PID(refPosition, realPosition, Kp, Ki, Kd, &iError, &dError, -200, 200);
  
  if(delayStepper < 0){
    dir = CCW;
    delayStepper = 300+delayStepper;
    delayStepper = max(delayStepper, 50);
    readPosition();
    
    digitalWrite(dirPin, 1);
    digitalWrite(stepPin, 0);
    delayMicroseconds(delayStepper);
    digitalWrite(stepPin, 1);
    delayMicroseconds(delayStepper);
  } else if(delayStepper > 0) {
    dir = CW;
    delayStepper = 300-delayStepper;
    delayStepper = max(delayStepper, 50);
    readPosition();
    
    digitalWrite(dirPin, 0);
    digitalWrite(stepPin, 0);
    delayMicroseconds(delayStepper);
    digitalWrite(stepPin, 1);
    delayMicroseconds(delayStepper);
  }
  
  if((unsigned long) millis()-logTiming >= 100){
    Serial.print(F("sc: "));
    Serial.print(stepCorner);
    Serial.print(COMMA);
    
    Serial.print(F("d: "));
    Serial.print(distance);
    Serial.print(COMMA);
    
    Serial.print(F("dx: "));
    Serial.print(distanceX);
    Serial.print(COMMA);
    
    Serial.print(F("dy: "));
    Serial.print(distanceY);
    Serial.print(COMMA);
    
    Serial.print(F("tstcp: "));
    Serial.print(thetaSliderToCenterPoint);
    Serial.print(COMMA);
    
    Serial.print(F("ttcp: "));
    Serial.print(thetaToCenterPoint);
    Serial.print(COMMA);

    Serial.print(F("dp: "));
    Serial.print(degreePos);
    Serial.print(COMMA);
    
    Serial.print(F("ep: "));
    Serial.print(encPosition);
    Serial.print(COMMA);
    
    Serial.print(F("rlp: "));
    Serial.print(realPosition);
    Serial.print(COMMA);
    
    Serial.print(F("rfp: "));
    Serial.print(refPosition);
    Serial.print(COMMA);
    
    Serial.print(F("ds: "));
    Serial.print(delayStepper);
    Serial.print(COMMA);
    
    Serial.print(F("dir: "));
    Serial.print(dir == CW ? F("CW"):F("CCW"));
    Serial.print(COMMA);
    
    Serial.print(F("ie: "));
    Serial.print(iError);
    Serial.print(COMMA);
    
//    Serial.print(F("a: "));
//    Serial.print(map(encPosition, 0, MAX_RESOLUTION, 0, 360)); 
    Serial.println();
    
    logTiming = millis();
  }

  if(stepCorner == 1){ 
    // tentukan jarak antara posisi aktual dengan posisi yang ingin dituju
    distanceX = targetX1 - realPositionMm; // realPositionMmX 
    distanceY = targetY1 - realPositionMm; // realPositionMmY
    // cari jarak sebenarnya dengan rumus phytagoras
    distance = (float)(sqrt((double)distanceX*(double)distanceX + (double)distanceY*(double)distanceY));

    // jika jaraknya masih jauh atau lebih dari nol, ya terus saja kasih referensi ke target pertama
    if(distance > 0){
      refPositionMm = targetX1; // refPositionMmX
      refPositionMm = targetY1; // refPositionMmY
      stepCorner = 1;
    } else {
      stepCorner = 2;
    }

    // cari jarak titik slider ke titik tengah
    distanceX = realPositionMm - centerPointX; // realPositionMmX
    distanceY = realPositionMm - centerPointY; // realPositionMmY
    // tentukan di posisi mana dia sekarang theta slider dengan titik tengahnya
    thetaSliderToCenterPoint = (float) atan2(distanceX, distanceY)*57.2957795; // konversi ke derajat

    if(thetaSliderToCenterPoint > 0){
      sign = 1;
    } else if(thetaSliderToCenterPoint < 0){
      sign = -1;
    }

    if(degreePos > 0){
      signD = 1;
    } else if(degreePos < 0){
      signD = -1;
    }
  } else if(stepCorner == 2){
    float subsPosX = centerPointX + rCircle * sin(thetaSliderToCenterPoint / 57.2957795);
    float subsPosY = centerPointY + rCircle * cos(thetaSliderToCenterPoint / 57.2957795);

    distanceX = subsPosX - realPositionMm; // realPositionMmX
    distanceY = subsPosY - realPositionMm; // realPositionMmY
    distance = (float)(sqrt((double)distanceX *(double)distanceX + (double)distanceY*(double)distanceY));

    distanceX = centerPointX - realPositionMm; // realPositionMmX
    distanceY = centerPointY - realPositionMm; // realPositionMmY
    thetaToCenterPoint = (float) atan2(distanceX, distanceY) * 57.2957795;

    if(distance > 0){
      refPositionMm = subsPosX; // refPositionMmX
      refPositionMm = subsPosY; // refPositionMmY
    } else {
      if(sign == signD){
        if(sign == 1){
          if(degreePos < 0) degreePos += 360;
        } else if(sign == -1){
          if(degreePos > 0) degreePos -= 360;
        }
      }

      if(thetaSliderToCenterPoint <= degreePos){
        thetaSliderToCenterPoint += 10; // derajat antara titik 1 ke titik berikutnya
        if(thetaSliderToCenterPoint > degreePos) thetaSliderToCenterPoint = degreePos;
      } else if(thetaSliderToCenterPoint > degreePos){
        thetaSliderToCenterPoint -= 10;
        if(thetaSliderToCenterPoint < degreePos) thetaSliderToCenterPoint = degreePos;
      }
    }
  }
}

void readPosition(){
  oldPosition = encPosition; // simpan posisi yang lama;

  // for(int8_t i=8; i>=0; i--){
  //   pinState[i] = !digitalRead(inputPin[i]);
  // }
  for(int8_t i=9; i>=0; i--){
   pinState[i] = !digitalRead(inputPin[i]);
  }

  // encPosition = pinState[8]; // bit msb selalu sama
  // // lalu lakukan konversi untuk bit selanjutnya
  // for(int8_t i=7; i>=0; i--){
  //   encPosition = (encPosition << 1) | (pinState[i] ^ (encPosition & 0x1));
  // }
  encPosition = pinState[9];
  for(int8_t i=8; i>=0; i--){
   encPosition = (encPosition << 1) | (pinState[i] ^ (encPosition & 0x1));
  }

  if((encPosition == MAX_RESOLUTION && oldPosition == 0) || (encPosition == 0 && oldPosition == MAX_RESOLUTION)) return;
  if(dir == CW){      
    if(encPosition > oldPosition){
      realPosition = realPosition + (encPosition - oldPosition);
    } else if(encPosition < oldPosition && oldPosition > MAX_RESOLUTION/2 && encPosition < MAX_RESOLUTION/2) {
      realPosition = realPosition + (MAX_RESOLUTION - oldPosition) + encPosition;
    } else if(encPosition < oldPosition){
      realPosition = realPosition - (oldPosition - encPosition);
    }
  } else if(dir == CCW){
    if(encPosition < oldPosition) {
      realPosition = realPosition - (oldPosition - encPosition);
    } else if(encPosition > oldPosition && oldPosition < MAX_RESOLUTION/2 && encPosition > MAX_RESOLUTION/2) {
      realPosition = realPosition - oldPosition - (MAX_RESOLUTION - encPosition);
    } else if(encPosition > oldPosition){
      realPosition = realPosition + (encPosition-oldPosition);
    }
  }
  realPositionMm = realPosition / stepToMmRatio;
}

int PID(long ref, long act, float Kp, float Ki, float Kd, float *iError, float *dError, int outmin, int outmax){
  float _pError = ref-act;
  float _iError = *iError;
  float _dError = *dError;
  
  long pid = _pError*Kp + _iError*Ki + (_pError-_dError)*Kd;

  *dError = _pError;
  _iError += _pError;
  *iError = constrain(_iError, -400, 400);

  return constrain(pid, outmin, outmax);  
}
