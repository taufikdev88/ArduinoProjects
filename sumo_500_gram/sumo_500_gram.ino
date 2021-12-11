#include <Psx.h>                                     
#define data_stickPin 12//abu2//MISO//PB_4
#define cmndPin 8//ungu//pwm//MOSI//PB_5
#define attPin 10//biru//pwm//SS//PA_15
#define clockPin 13//ijo//pwm//SCK//PB_3

#define mode1 digitalRead(2)
#define mode2 digitalRead(4)

#define pwm1 5
#define pwm2 6
#define pwm3 9
#define pwm4 11

int kec=0,kec1=0,kec2=0;
#define fst 150
#define sfst 255
#define slw 100

/*
 * offset untuk menyeimbangkan kecepatan motor kanan kiri, offset akan mengurangi dari pwm referensi
 */
#define slwKiOffset 5
#define fstKiOffset 5
#define sfstKiOffset 5

#define slwKaOffset 0
#define fstKaOffset 0
#define sfstKaOffset 0

Psx Psx;
int data_stick = 0;
int bunder =1024,atas=8,kanan=4,kiri=1,bawah=2,X=512,segitiga=2048,R1=4096,kotak=256;
const int R2 =        16384;
const int L2 =       -32768;
int j=150;

void setup() {
  Psx.setupPins(data_stickPin, cmndPin, attPin, clockPin, 0);
  Serial.begin(115200);
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(pwm3,OUTPUT);
  pinMode(pwm4,OUTPUT);
  //pinMode(sj1,INPUT_PULLUP);
  //pinMode(sj2,INPUT_PULLUP);
  //pinMode(sj3,INPUT_PULLUP);
  //pinMode(sj4,INPUT_PULLUP);
  //pinMode(sj5,INPUT_PULLUP);
  //pinMode(sg1,INPUT_PULLUP);
  //pinMode(sg2,INPUT_PULLUP);
  //pinMode(mode1,INPUT_PULLUP);
  //pinMode(mode2,INPUT_PULLUP);
}

void loop() {
  data_stick=Psx.read(); 
//  Serial.println(data_stick);
  delay(10);
  if(data_stick==0)                   {kec=slw;berhenti();}
  if(data_stick==X)                   {kec=fst;berhenti();}
  if(data_stick==atas)                {kec=slw;maju();}
  if(data_stick==bawah)               {kec=slw;mundur();}
  if(data_stick==kanan)               {kec=slw;belka();}
  if(data_stick==kiri)                {kec=slw;belki();}
  if(data_stick==kotak)               {kec=fst;putka();}
  if(data_stick==bunder)              {kec=fst;putki();}
  if(data_stick==kotak+R1)            {kec=sfst;putka();}
  if(data_stick==bunder+R1)           {kec=sfst;putki();}
  if(data_stick==atas+kiri)           {kec=slw;serki();}
  if(data_stick==atas+kanan)          {kec=slw;serka();}
  //if(data_stick==bawah+kiri)        {kec=slw;}
  //if(data_stick==bawah+kanan)       {kec=slw;}
  
  
  if(data_stick==atas+X)              {kec=fst;maju();}
  if(data_stick==bawah+X)             {kec=fst;mundur();}
  if(data_stick==kanan+X)             {kec=fst;belka();}
  if(data_stick==kiri+X)              {kec=fst;belki();}
  if(data_stick==atas+kiri+X)         {kec=fst;serki();}
  if(data_stick==atas+kanan+X)        {kec=fst;serka();}
  
  if(data_stick==0+R1)                {kec=sfst;berhenti();}
  if(data_stick==X+R1)                {kec=sfst;berhenti();}
  if(data_stick==atas+R1)             {kec=sfst;maju();}
  if(data_stick==bawah+R1)            {kec=sfst;mundur();}
  if(data_stick==kanan+R1)            {kec=sfst;belka();}
  if(data_stick==kiri+R1)             {kec=sfst;belki();}
  if(data_stick==atas+kiri+R1)        {kec=sfst;serki();}
  if(data_stick==atas+kanan+R1)       {kec=sfst;serka();}
  
  if(data_stick==R2)                  {kec=sfst;putkaback();}
  if(data_stick==R2+X)                {kec=sfst;putkaback();}
  if(data_stick==R2+R1)               {kec=sfst;putkaback();}
  
  if (data_stick==L2)                 {putkiback();}
  if (data_stick==L2+X)               {putkiback();}
  if (data_stick==L2+R1)              {putkiback();}
  
  if(data_stick==R2+R1+X)             {kec=sfst;putka();delay(j);berhenti();}
  //if(data_stick==bawah+kiri)          {kec=slw;}
  //if(data_stick==bawah+kanan)         {kec=slw;}
  
  
  if(data_stick==atas+X+R1)           {kec=sfst;maju();}
  if(data_stick==bawah+X+R1)          {kec=sfst;mundur();}
  if(data_stick==kanan+X+R1)          {kec=sfst;belka();}
  if(data_stick==kiri+X+R1)           {kec=sfst;belki();}
  if(data_stick==atas+kiri+X+R1)      {kec=sfst;serki();}
  if(data_stick==atas+kanan+X+R1)     {kec=sfst;serka();}

  if(data_stick==atas+bunder)         {kec=slw;putki();}
  if(data_stick==atas+bunder+X)       {kec=slw;putki();}
  if(data_stick==atas+bunder+R1)      {kec=slw;putki();}

  if(data_stick==atas+kotak)          {kec=slw;putka();}
  if(data_stick==atas+kotak+X)        {kec=slw;putka();}
  if(data_stick==atas+kotak+R1)       {kec=slw;putka();}
  
}

unsigned long timer = millis();
void berhenti(){
  kec1 = 0; // reset kec1 ke 0;
 
  analogWrite(pwm1,0);
  analogWrite(pwm2,0);
  analogWrite(pwm3,0);
  analogWrite(pwm4,0);
}
/*
 * kecepatan maju berdasar timer, timer direset setiap nilai kec lebih besar dari yang sekarang di jalankan
 * scale di dapat dari timer yang nantinya dikalikan dengan kecepatannya, nilai kec1 direset ke 0 saat berhenti 
 * lama softstart berdasarkan besarnya scale
 * contoh , jika pembagi scale adalah 1000 artinya
 * 1000 (ms) / 1000.0 = 1, jadinya dalam 1 detik menuju kecepatan referensi
 * 1000 (ms) / 2000.0 = 0.5, jadinya dalam 1 detik baru setengan dari kecepatan referensi
 * 1000 (ms) / 3000.0 = 0.33, jadinya perlu 1 detik baru 1 per 3 dari kecepatan referensi
 * note, berikan .0 untuk pembaginya , biar dieksekusi sebagai nilai float
 * 
 */
void maju(){
  float scale = (unsigned long) millis()-timer;
  scale /= 300.0; // ubah disini untuk mempengaruhi lama / cepatnya softstart
  scale = constrain(scale, 0, 1);

  /*
   * SECTION A
   * 
   * jika nilai kec yang terbaru lebih besar dari kecepatan yang sekarang(kec2)
   * kunci nilai kec yang baru ke kec1 agar timer tidak reset terus
   * 
   * jika nilai kec yang terbaru lebih kecil dari yang sekarang, ya update langsung saja agar 
   * timer tidak bekerja biar kecepatannya langsung turun,
   * perlu merubah nilai kec2 dan kec1 agar pwm juga turun
   */
  if(kec > kec1){ 
    kec2 = kec1;
    kec1 = kec;
    timer = millis();
  } else if(kec < kec1) {
    kec2 = kec;
    kec1 = kec;
  }

  /*
   * SECTION B
   * 
   * hanya eksekusi softstart di mode kecepatan fst dan sfst
   * rumusnya kecepatan yang lalu ditambah dengan selisih kecepatan yang terbaru dengan yang sekarang
   * 
   */
  switch(kec){
    case sfst:
//    Serial.print("sfst: ");
//    Serial.println(constrain((kec2+(kec1-kec2)*scale)-sfstKiOffset, 0, 255));
    
    analogWrite(pwm1, constrain((kec2+(kec1-kec2)*scale)-sfstKiOffset, 0, 255));
    analogWrite(pwm3, constrain((kec2+(kec1-kec2)*scale)-sfstKaOffset, 0, 255));
    break;
    case fst:
//    Serial.print("fst: ");
//    Serial.println(constrain((kec2+(kec1-kec2)*scale)-fstKiOffset, 0, 255));
    
    analogWrite(pwm1, constrain((kec2+(kec1-kec2)*scale)-fstKiOffset, 0, 255));
    analogWrite(pwm3, constrain((kec2+(kec1-kec2)*scale)-fstKaOffset, 0, 255));
    break;
    case slw:
//    Serial.print("slw: ");
//    Serial.println(constrain((kec2+(kec1-kec2)*scale)-slwKiOffset, 0, 255));
    
    analogWrite(pwm1, constrain((kec2+(kec1-kec2)*scale)-slwKiOffset, 0, 255));
    analogWrite(pwm3, constrain((kec2+(kec1-kec2)*scale)-slwKaOffset, 0, 255));
    break;
  }
  
  analogWrite(pwm2,0);
  analogWrite(pwm4,0);
  /*
   * PENJELASAN
   * 
   * case berhenti ke slw
   * 1. saat berhenti, nilai kec1 direset ke 0
   * 2. di SECTION A, penyimpanan nilai kec yang dulu dengan yang sekarang, kec2 di set ke nol, nilai kec1 di set ke kecepatan yang diinginkan
   * 3. karena di SECTION B , hanya untuk fst dan sfst maka pwm yang di gunakan adalah pwm kec secara langsung
   * 
   * case berhenti ke fst
   * 1. saat berhenti, nilai kec1 direset ke 0
   * 2. di SECTION A, nilai kec pasti lebih besar dari nol. kec2 di set ke nol, nilai kec1 di set ke fst
   * 3. di SECTION B, nilai kec2 + dengan selisih yang dikalikan scale. 0 + (fst-0) * scale
   * 
   * case dari slw ke fst
   * 1. saat mode slw karena nilai kec1 sudah sama dengan nilai kec
   * 2. di SECTION A, nilai kec pasti lebih besar dari kec1. kec2 diset ke slw, nilai kec1 di set ke fst
   * 3. di SECTION B, nilai slw + (fst-slw)*scale
   * 
   * case dari slw ke sfst
   * 1. saat mode slw, karena nilai kec1 sudah sama dengan nilai kec
   * 2. di SECTION A, nilai kec pasti lebih besar dari kec1. kec2 diset ke slw, nilai kec1 di set ke sfst
   * 3. di SECTION B, nilai slw + (sfst-slw)*scale
   * 
   * case dari fst ke sfst
   * 1. saat mode fst, nilai kec1 pasti sama dengan nilai fst
   * 2. di SECTION A, nilai kec pasti lebih besar dari kec1 (sfst > fst). kec2 diset ke fst, nilai kec1 di set ke sfst
   * 3. di SECTION B, nilai fst + (sfst-fst)*scale
   * 
   * case dari sfst ke fst berlaku untuk semua kec terbaru yang lebih kecil dari pada kec lama
   * 1. di SECTION A, saat nilai kec lebih kecil dari kec1, langsung update nilai kec1 ke nilai kec dan nilai kec2 ke nilai kec juga
   * 2. di SECTION B, saat modenya adalah fst atau sfst, rumusnya akan menjadi sprti berikut fst+(fst-fst)*scale = fst
   * 
   * untuk menambah bbrp tingkat kecepatan lagi, rumus ini akan langsung menyesuaikan, tambahkan saja pada SECTION B , jika mau dijalankan softstart
   * syarat penamaan dari tingkat kecepatan yaitu
   * #define namakecepatan 123123
   * lalu tambahkan ke SECTION B
   * case nama1:
   * break;
   */
}
void mundur(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,kec);
  analogWrite(pwm3,0);
  analogWrite(pwm4,kec-10);
}
void belka(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,50);
  analogWrite(pwm3,kec);
  analogWrite(pwm4,0);
}
void belki(){
  analogWrite(pwm1,kec);
  analogWrite(pwm2,0);
  analogWrite(pwm3,0);
  analogWrite(pwm4,50);
}
void serka(){
  analogWrite(pwm1,30);
  analogWrite(pwm2,0);
  analogWrite(pwm3,kec);
  analogWrite(pwm4,0);
}
void serki(){
  analogWrite(pwm1,kec);
  analogWrite(pwm2,0);
  analogWrite(pwm3,30);
  analogWrite(pwm4,0);
}
void putki(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,kec);
  analogWrite(pwm3,kec);
  analogWrite(pwm4,0);
}
void putka(){
  analogWrite(pwm1,kec);
  analogWrite(pwm2,0);
  analogWrite(pwm3,0);
  analogWrite(pwm4,kec);
}
void putkiback(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,255);
  analogWrite(pwm3,0);
  analogWrite(pwm4,50);
}
void putkaback(){
  analogWrite(pwm1,0);
  analogWrite(pwm2,50);
  analogWrite(pwm3,0);
  analogWrite(pwm4,255);
}
