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

float rpm = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  regresiLinear(Pengukuran, 5, &m, &b);
  attachInterrupt(digitalPinToInterrupt(2), cek, FALLING);
}

unsigned long timestart = 0;
unsigned long timestop = 0;
unsigned long kirim = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if(timestop > timestart){
    unsigned long dif = timestop - timestart;
    rpm = 60000.0 / dif;
  }


  if((unsigned long) millis()-kirim >= 500){
    kirim = millis();
    Serial.println(rpm,5);
  }
}

bool state = false;
void cek(){
  if(state){
    timestart = millis();
  } else {
    timestop = millis();
  }
  state = !state;
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
