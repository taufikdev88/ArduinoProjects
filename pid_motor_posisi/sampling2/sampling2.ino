//*******************************************************
struct sample
{
  float tegangan;
  float rpm;
};
sample Pengukuran[] = {
  {3, 378.2148},
  {3.5, 526.0132},
  {4, 634.5824},
  {4.5, 746.6706},
  {5, 869.7479}
};
float m;
float b;
void regresiLinear(sample *sampling, float jumlahSampling, float *m, float *b){
  float jumlahSudut = 0;  // yi
  float jumlahData = 0; // xi
  float jumlahDataSudut = 0; //xiyi
  float jumlahData2 = 0; // xi2
  
  for (int i = 0; i < jumlahSampling; i++)  {
    jumlahSudut += sampling[i].tegangan;
    jumlahData += sampling[i].rpm;
    jumlahDataSudut += (sampling[i].tegangan * sampling[i].rpm);
    jumlahData2 +=  (sampling[i].rpm *  sampling[i].rpm);
  }
  *m = (float)(jumlahSampling * jumlahDataSudut) - (jumlahData * jumlahSudut);
  *m /= (float)(jumlahSampling * jumlahData2) - (jumlahData * jumlahData);
 
  *b = (float)(jumlahData2 * jumlahSudut) - (jumlahData * jumlahDataSudut);
  *b /= (float)(jumlahSampling * jumlahData2) - (jumlahData * jumlahData);
}

//*******************************************************
volatile unsigned long encoder;
unsigned long lastInt = 0;
void interruptKi(){
//  if((unsigned long) millis() - lastInt > 1){
    encoder++;
//    lastInt = millis();
//  }
}

//*******************************************************
void setup() {
  attachInterrupt(digitalPinToInterrupt(2), interruptKi, RISING);
  regresiLinear(Pengukuran, 5, &m, &b);
  Serial.begin(115200);
}

float rpm;
unsigned long time;
unsigned long oldtime;
void loop() {
  delay(30);
//  Serial.print(encoder);
//  Serial.print('\t');
  
  detachInterrupt(digitalPinToInterrupt(2));
  time=(unsigned long)millis()-oldtime;        //finds the time 
  
  rpm=((float) (encoder/20.0)*(60000.0/time));         //calculates rpm
  
  oldtime=millis();             //saves the current time 
  encoder=0;
  attachInterrupt(digitalPinToInterrupt(2), interruptKi, RISING);

  float tegangan = m * rpm + b;
  Serial.print(tegangan);
  Serial.print('\t');
  Serial.println(rpm,5);
}
