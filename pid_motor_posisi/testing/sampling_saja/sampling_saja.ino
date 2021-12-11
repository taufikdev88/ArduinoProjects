#define lpwm 5
#define rpwm 6
#define lsMin 7
#define lsMax 8
#define pbMin 9
#define pbMax 10
int sensor = A1;

uint16_t sensorVal[2] = {0};
uint8_t pos = 90;

struct sample
{
  float sudut;
  float data;
};

sample Pengukuran[] = {
  {0, 249},
  {20, 330},
  {40, 400},
  {60, 480},
  {80, 550},
  {100, 615},
  {120, 690},
  {140, 770},
  {160, 855},
  {180, 926},
};

float m;
float b;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(lpwm, OUTPUT);
  pinMode(rpwm, OUTPUT);
  pinMode(lsMin, INPUT_PULLUP);
  pinMode(lsMax, INPUT_PULLUP);
  pinMode(pbMin, INPUT_PULLUP);
  pinMode(pbMax, INPUT_PULLUP);
  
  regresiLinear(Pengukuran, 10, &m, &b);
}

#define isLsMin() digitalRead(lsMin)
#define isLsMax() digitalRead(lsMax)
#define isPbMin() digitalRead(pbMin)
#define isPbMax() digitalRead(pbMax)
void loop() {
  if(!isPbMin()){
    Serial.println("pb min");
    mCCW(100);
    while(isLsMin());
    mStop();
    sensorVal[0] = analogRead(sensor);
    Serial.println((String) "min sens val = " + sensorVal[0]);
  }
  if(!isPbMax()){
    Serial.println("pb max");
    mCW(100);
    while(isLsMax());
    mStop();
    sensorVal[1] = analogRead(sensor);
    Serial.println((String) "max sens val = " + sensorVal[1]);    
  }
  
  // pressed
  if(Serial.available() > 0){
    mStop();
    String dataIn = Serial.readStringUntil('\n');    
    if(dataIn.toInt() > 180 || dataIn.toInt() < 0) return;
    pos = dataIn.toInt();
    return;
  }

  int curPos = m * float(analogRead(sensor)) + b;
  if(curPos < pos){
    mCW(255);
  }
  if(curPos > pos){
    mCCW(255);
  }
  if(curPos == pos){
    mStop();
  }
  
  static unsigned long TSample = millis();
  if((unsigned long) millis()-TSample >= 100){
    Serial.print(pos);
    Serial.print('\t');
    Serial.print(curPos);
    Serial.println();
    TSample = millis();
  }
}

void mCCW(uint8_t pwm){
  analogWrite(lpwm, pwm);
  analogWrite(rpwm, 0);
}
void mCW(uint8_t pwm){
  analogWrite(lpwm, 0);
  analogWrite(rpwm, pwm);
}
void mStop(){
  analogWrite(lpwm, 0);
  analogWrite(rpwm, 0);
}

void calibrate(){
  mCW(100);
  while(isLsMax());
  mStop();
  sensorVal[1] = analogRead(sensor);
  Serial.println((String) "max sens val = " + sensorVal[1]);
  
  mCCW(100);
  while(isLsMin());
  mStop();
  sensorVal[0] = analogRead(sensor);
  Serial.println((String) "min sens val = " + sensorVal[0]);
  pos = 0;
}

void regresiLinear(sample *sampling, float jumlahSampling, float *m, float *b)
{
  float jumlahSudut = 0;  // yi
  float jumlahData = 0; // xi
  float jumlahDataSudut = 0; //xiyi
  float jumlahData2 = 0; // xi2
  
  for (int i = 0; i < jumlahSampling; i++)
  {
    jumlahSudut += sampling[i].sudut;
    jumlahData += sampling[i].data;
    jumlahDataSudut += (sampling[i].sudut * sampling[i].data);
    jumlahData2 +=  (sampling[i].data *  sampling[i].data);
  }
  *m = (float)(jumlahSampling * jumlahDataSudut) - (jumlahData * jumlahSudut);
  *m /= (float)(jumlahSampling * jumlahData2) - (jumlahData * jumlahData);
 
  *b = (float)(jumlahData2 * jumlahSudut) - (jumlahData * jumlahDataSudut);
  *b /= (float)(jumlahSampling * jumlahData2) - (jumlahData * jumlahData);
}
