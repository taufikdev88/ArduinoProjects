#include "MazeSolvingGR.h"

void initmazerobot() {
  start(1,100);
  error(1,200);
  sensor(ff);
  linecolor(black);
}

void ambil(){
  rightenc(50,180,0);
  linefind(50,50,0);
  linedelay(30,800,0);
  take(300);
  motorcm(-100,15,0);
  leftenc(100,200,0);
}

void mode1(){
  step();
//  String perintah = raspidetectqr(bb, 60);
//  showonlcd(perintah);
//  delay_ms(3000);

  placeup(50);
  controller(4.0, 0.0);
  motorController(0.1,0.005,0.01); // atur pid untuk encoder motor
  step();

  motorcm(100, 20, 0);
  rightenc(100, 200, 0);
  step();

  linefind(50, 60, 0);
  leftline(100,1000);
  delay_maze(1000);
  step();

  leftline(100,750);
  delay_maze(1000);
  step();

//  leftline(100,800);
//  delay_maze(1000);
//  step();
//
//  ambil();
//  step();
//
//  leftline(100,2200,0);
}

void mode2(){
  step();
  placeup(50);
  controller(4.0, 0.0);
  motorController(0.1,0.005,0.01); // atur pid untuk encoder motor

  motorcm(100, 20, 0); // dari garis start maju 20 cm
  rightenc(100, 200, 0); // belok kanan 200 counter
  step(); // step 1
  
  motorrpm(300, 1300, 0); // maju 300 rpm selama 3 detik
  leftenc(100, 200, 0); // belok kiri 200 counter
  step(); // step 2
  
  motorrpm(300, 1100, 0); // maju 300 rpm selama 2 detik
  leftenc(100, 200, 0);
  step(); // step 3
  
  rightline(80, 800, 0); // menuju tempat 1
  //raspidetectqr(bb, 1);
  delay_maze(500);
  step();
  
  rightline(80, 900, 0); // menuju tempat 2
  raspidetectqr(bb, 1);
//  buzzerled();
  delay_maze(500);
  step();
  
  rightline(80, 900, 0); // menuju tempat 3
  //raspidetectqr(bb, 1);
  delay_maze(500);
  step();

  ambil(); // ambil
  step();

  leftline(100,2500); // menuju tempat letak
  leftenc(100,200,0); // belok kiri
  step();

  // kurang maju dan letakkan
  // linefind / linedelay
  // put
}

void mode3(){
  step();
  // setiap 2 detik ngirim perintah
  //raspidetectqr(bb, 1);
  delay_maze(2000);
  //raspidetectqr(bb, 1);
  delay_maze(2000);
  //raspidetectqr(bb, 1);
  delay_maze(2000);
  
  // baca sesuai kamera
//  showonlcd(raspidetectqr(bb, 60)); // deteq qr melalui raspi sensor belakang, percobaan 60 kali
}
