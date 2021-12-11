#define s0 A11
#define s1 A9
#define s2 A10
#define s3 A8
#define out A12

int colorFreq[4] = {0};
void tcsSetup(){
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
}

String scanWarna(){
  // merah
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  colorFreq[1] = pulseIn(out, LOW);

  // hijau
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  colorFreq[2] = pulseIn(out, LOW);

  // biru
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  colorFreq[3] = pulseIn(out, LOW);

  uint8_t color = 0;
  colorFreq[0] = 1000;
  for(int j=1; j<=3; j++){
    Serial.print(colorFreq[j]);
    Serial.print('\t');
  
    if(colorFreq[0] > colorFreq[j]) {
      colorFreq[0] = colorFreq[j];
      color = j;
    }
  }

  switch(color){
    case 0: return "NONE"; break;
    case 1: return "R"; break;
    case 2: return "G"; break;
    case 3: return "B"; break;
  }
}

void setup() {
  // put your setup code here, to run once:
  tcsSetup();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(scanWarna());
  delay(500);
}
