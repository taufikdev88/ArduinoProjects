#include "MazeSolvingGR.h"

void initMazeRobot(){ // fungsi untuk inisialisasi 
  start(1,100); // mulai robot dengan menggunakan buzzer dan set pcTime ke 100ms, bisa diatur sesuai kebutuhan
  error(1,200); // gunakan fungsi error agar saat robot keluar jalur lebih dari 200ms, robot akan berhenti
  
  // fungsi dibawah ini bisa diatur di dalam mode1,mode2 atau mode3
  sensor(ff); // gunakan sensor depan
  linecolor(black); // atur robot untuk memulai dengan garis berwarna hitam
}

void mode1(){
  /*
   * Program SMP ambil warna benda
   */
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

void ambilbenda(){
  rightenc(100, 200, 0); // belok kanan 200 counter
  linefind(30, 30, 0);
  linedelay(30, 500, 0);
  put(300);
}

void mode2(){
  /*
   * Program SMA, baca qr
   */
  step(); // 0
  controller(2.0, 0.0); // atur pd untuk sensor garis
  motorController(0.1,0.005,0.01); // atur pid untuk encoder motor
  placeup(1);
  camfront(1);
  
  String perintah = ""; // siapkan variable penyimpan perintah
  while (perintah == ""){
    // selama perintah belum didapatkan
    perintah = raspidetectqr(bb,10); // request baca qr
  }
  
  motorcm(100, 20, 0); // dari garis start maju 20 cm
  rightenc(100, 200, 0); // belok kanan 200 counter
  step(); // step 1
  motorrpm(300, 1500, 0); // maju 300 rpm selama 3 detik
  leftenc(100, 200, 0); // belok kiri 200 counter
  step(); // step 2
  motorrpm(300, 900, 0); // maju 300 rpm selama 2 detik
  leftenc(100, 200, 0);
  step(); // step 3

  camright(1);
  String qrdetect = ""; // siapkan variable penyimpan qr yang dideteksi
  qrdetect = motorrpmdetectqr(200, 5000, true); // berbeda dengan fungsi detect color yang bisa terus maju jika parameter ke empat diisi id warna, fungsi ini akan langsung berhenti jika menemukan qrcocde
//  qrdetect = rightlinedetectqr(200, 5000, true); // detect qr dengan fungsi follow garis kanan
  if(qrdetect == perintah){ // jika menemukan data yang sama
    ambilbenda(); // jalankan fungsi ambil benda yang bisa anda definisikan sendiri diatas
    goto skipdetect; // langsung loncat ke label skipdetect
  }
  step(); // step 4
  qrdetect = motorrpmdetectqr(200, 5000, true);
//  qrdetect = rightlinedetectqr(200, 5000, true);
  if(qrdetect == perintah){
    ambilbenda();
    goto skipdetect;
  }
  step(); // step 5
  qrdetect = motorrpmdetectqr(200, 5000, true);
//  qrdetect = rightlinedetectqr(200, 5000, true);
  if(qrdetect == perintah){
    ambilbenda();
  }
  step(); // step 6
skipdetect:

  motorrpm(300, 2500, 0); // lanjut maju 300 rpm selama 2,5 detik asal tidak mentok garis
  motorcm(-100, 10, 0); // mundur setelah mentok garis
  leftenc(100, 130, 0); // belok kiri 90 derajat
  step();
  motorrpm(300, 1000, 0); // maju 300 rpm selama 1 detik
  leftenc(100, 200, 0);
  step();
  
};

void mode3(){
  step(); // 0
  motorController(0.1,0.005,0.01); // atur pid untuk encoder motor
  controller(2.0, 0.0); // atur pd untuk sensor garis

//  String qr = raspidetectqr(bb, 5); // 5 percobaan setiap 1 detik
  placeup(1);
  camfront(1);
  
  motorcm(100, 20, 0); // dari garis start maju 20 cm
  rightenc(100, 200, 0); // belok kanan 200 counter
  step(); // step 1
  motorrpm(300, 1300, 0); // maju 300 rpm 
  leftenc(100, 200, 0); // belok kiri 200 counter
  step(); // step 2
  motorrpm(300, 1000, 0); // maju 300 rpm selama 1 detik
  leftenc(100, 200, 0);
  step(); // step 3

  camright(1);
  leftline(50, 4000, 0); // speed berbeda dengan rpmspeed
  step();
  
  motorrpm(300, 2500, 0); // lanjut maju 300 rpm selama 2,5 detik asal tidak mentok garis
  motorcm(-100, 10, 0); // mundur setelah mentok garis
  leftenc(100, 130, 0); // belok kiri 90 derajat
  step();
  motorrpm(300, 1000, 0); // maju 300 rpm selama 1 detik
  leftenc(100, 200, 0);
  step();
  
};

void customloop(){
  delay(1000);
}
