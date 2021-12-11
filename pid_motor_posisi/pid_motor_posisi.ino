#define lpwm 10
#define rpwm 11
#define lsMin 12
#define lsMax 13
#define pbMin 8
#define pbMax 9
int sensor = A1;

uint16_t sensorVal[2] = {0};
uint8_t pos = 90;

float i_err = 0, d_err = 0; 

float kp = 30;
float ki = 0.001;
float kd = 0.001;

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

//  Serial.println("Calibrating");
//  calibrate();
//  Serial.println("Finish");
  
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
    if(dataIn.indexOf('p') == 0){
      kp = dataIn.substring(1).toFloat();
      return;
    }
    if(dataIn.indexOf('i') == 0){
      ki = dataIn.substring(1).toFloat();
      return;
    }
    if(dataIn.indexOf('d') == 0){
      kd = dataIn.substring(1).toFloat();
      return;
    }
    
    if(dataIn.toInt() > 180 || dataIn.toInt() < 0) return;
    i_err = 0;
    pos = dataIn.toInt();
    return;
  }

//  uint8_t curPos = map(analogRead(sensor), sensorVal[0], sensorVal[1], 0, 180);
  int curPos = m * float(analogRead(sensor)) + b;
  
  float p_err = (float) pos - curPos;
  int pwm = (float)(kp*p_err + ki*i_err + kd*(p_err-d_err));
  
  i_err += p_err;
  d_err = p_err;
  i_err = constrain(i_err, -50000, 50000);

  pwm = constrain(pwm, -255, 255);

  static unsigned long TSample = millis();
  if((unsigned long) millis()-TSample >= 100){
//    Serial.print(analogRead(sensor)); // nilai sensor didapat dari adc
//    Serial.print('\t');
//    Serial.print(p_err); // nilai p error
//    Serial.print('\t');
//    Serial.print(i_err); // nilai i error
//    Serial.print('\t');
//    Serial.print(d_err); // nilai d error
//    Serial.print('\t');
//    Serial.print(pwm); // pwm yang didapat dari pid dan di keluarkan ke motor
//    Serial.print('\t');

    Serial.print("h,");
    Serial.print(pos); // posisi yg diinginkan
    Serial.print(',');
    Serial.print(curPos); // posisi sekarang
    Serial.print(",l");
    Serial.println();
    TSample = millis();
  }

  if(pwm < 0){
    mCCW(-pwm);
    if(isLsMin() == 0) mStop();
  } else if(pwm > 0) {
    mCW(pwm);
    if(isLsMax() == 0) mStop();
  } 
//  else {
//    mStop();
//  }
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
