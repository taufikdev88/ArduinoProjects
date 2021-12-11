#include "MazeSolvingGR.h"

void initMazeRobot(){ // fungsi untuk inisialisasi 
  start(1,100); // mulai robot dengan menggunakan buzzer dan set pcTime ke 100ms, bisa diatur sesuai kebutuhan
  error(1,200); // gunakan fungsi error agar saat robot keluar jalur lebih dari 200ms, robot akan berhenti
  
  // fungsi dibawah ini bisa diatur di dalam mode1,mode2 atau mode3
  sensor(ff); // gunakan sensor depan
  linecolor(black); // atur robot untuk memulai dengan garis berwarna hitam
}

void mode1(){
  step(); // 0
  controller(2.0, 0.0); // atur pd untuk sensor garis
  motorController(0.1,0.005,0.01); // atur pid untuk encoder motor
  camright(1);
  
  motorcm(100, 20, 0); // dari garis start maju 20 cm
  rightenc(100, 200, 0); // belok kanan 200 counter
  step(); // step 1
  motorrpm(300, 1500, 0); // maju 300 rpm selama 3 detik
  leftenc(100, 200, 0); // belok kiri 200 counter
  step(); // step 2
  motorrpm(300, 900, 0); // maju 300 rpm selama 2 detik
  leftenc(100, 200, 0);
  step(); // step 3
  // beri fungsi kamera noleh kanan
  int8_t colorid = motorrpmdetectcolor(300, 5000, true, 0); // maju 300 rpm, jika ada objek berwarna, akan berhenti
  rightenc(100, 200, 0); // belok kanan menghadap benda
  step(); // step 4
  linefind(30, 30, 0); // maju sampai menemui garis, waktu mengabaikan garis: 0 mili detik
  linedelay(30, 800, 0); // maju mengikuti garis selama 500 detik, waktu mundur: 0 mili detik
  take(300); // ambil benda
  step();
  // beri fungsi ambil beda
  motorcm(-100, 10, 0); // mundur speed 100 sejauh 15 cm
  leftenc(100, 150, 0); // belok kiri
  step();
  motorrpm(300, 2500, 0); // maju 300 rpm selama 2,5 detik asal tidak mentok garis
  motorcm(-100, 10, 0); // mundur setelah mentok garis
  leftenc(100, 130, 0); // belok kiri 90 derajat
  step();
  motorrpm(300, 1000, 0); // maju 300 rpm selama 1 detik
  leftenc(100, 200, 0);
  step();
  motorrpmdetectcolor(300, 5000, true, colorid);
  rightenc(100, 200, 0);
  step();
  linefind(30, 30, 0);
  linedelay(30, 800, 0);
  put(300); // taruh benda
  step();
  motorcm(-100, 10, 0); // mundur speed 100 sejauh 15 cm
};

void mode2(){
  step(); // 0
  controller(2.0, 0.0); // atur pd untuk sensor garis
  motorController(0.1,0.005,0.01); // atur pid untuk encoder motor
  camfront(10);
  linefind(30, 30, 0); // maju sampai menemui garis, waktu mengabaikan garis: 0 mili detik
  linedelay(30, 500, 0); // maju mengikuti garis selama 500 detik, waktu mundur: 0 mili detik
  take(300);
};

void mode3(){
  step(); // 0
//  controller(2.0, 0.0); // atur pd untuk sensor garis
//  motorController(0.1,0.005,0.01); // atur pid untuk encoder motor
//  camright(100);
//  linefind(30, 30, 0); // maju sampai menemui garis, waktu mengabaikan garis: 0 mili detik
//  linedelay(30, 500, 0); // maju mengikuti garis selama 500 detik, waktu mundur: 0 mili detik
//  put(300);
  pickdn(1000);
  pickup(1000);
  pickdn(1000);
  pickup(1000);
  placedn(1000);
  placeup(1000);
  placedn(1000);
  placeup(1000);
  slineleft(sensor1, 50, 0);
};

void customloop(){
  String qrdata = gm66detectqr(bb,10);
  if(qrdata == ""){
    showonlcd("kosong");
  } else {
    showonlcd(qrdata); 
  }
  buzzer(3,300,300);
}
