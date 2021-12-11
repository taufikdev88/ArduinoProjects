/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C
Code    : Pengaturan kecepatan motor DC (motor roda) dengan PWM
*****************************************************************/

/*
Jika menggunakan Code::Blocks sebagai editor:
untuk optimasi supaya file hex hasil kompilasi menjadi kecil (fungsi yang tidak dipanggil tidak dimasukkan ke dalam
file hex), maka dilakukan pengaturan proses compile dan linker program sbb:
1. copy option berikut pada compiler setting    (Menu Settings,Compiler, Compiler settings, Other options:)
   -ffunction-sections
2. copy option berikut pada linker setting      (Menu Settings,Compiler, Linker settings, Other linker options:)
   -Wl,-gc-sections
*/

#include "myModule.h"                   // edit module yang akan digunakan pada file ini
#include "../AdRoiT_Files/mySystem.h"

int main(void)
{
    SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"
    lcd.Hapus();
    lcd.TulisKanan (0, "Tekan PushButton");
    lcd.TulisKiri  (1, "AturPosisi Servo");
    buzzer.Nada1();

// pengaturan motor Servo dapat diakses melalui obyek servo yang dideklarasikan pada file "myMotor.h"
// pengaturan posisi motor servo diatur melalui fungsi ServoN(posisi)
// dimana posisi = data posisi sudut motor servo yang nilainya berkisar antara 0 sampai dengan 180 derajat
// sedangnkan N adalah nomor dari motor servo yang akan diatur, yaitu 1 sampai dengan 8

// Khusus untu Servo 1 sampai dengan 5, sebelum digunakan harus diaktifkan dahulu melalui pengaturan
// UseServoN = 1 (lihat file "myModule.h")
// Apabila tidak digunakan, sebaiknya motor servo tersebut (1-5) tidak diaktifkan pada file "myModule.h"
// Untuk servo 6-8 tidak perlu diaktifkan melalui file "myModule.h", karena secara default sudah aktif
// saat inisialisasi System

// Apabila julah motor servo yang dibutuhkan lebih sedikit, sebaiknya dipilih dahulu motor servo 6-8, baru
// kalau kurang, baru menambahkan kekurangannya dari servo 1-5

// Motor Servo 6-8 mempunyai resolusi sudut yang lebih besar, karena dibangkitkan melalui internal PWM
// dari timer 1. (proses pembangkitan sinyal PWM tidak membutuhkan komputasi CPU)

// Motor Servo 1-5 mempunyai resolusi sudut yang lebih kecil, karena sinyal PWM yang dihasilkan merupakan
// hasil emulasi dengan menggunakan interupsi timer. (proses pembangkitan sinyal PWM  membutuhkan komputasi
// CPU untuk mencacah sinyal PWM pada interupsi timer)

// Kalibrasi posisi sudut untuk motor Servo 1-5 dapat dilakukan dengan merubah parameter pada fungsi HitungSudut
// yang terdapat pada file "myMotor.h"

// Kalibrasi posisi sudut untuk motor Servo 6-8 dapat dilakukan dengan merubah parameter pada fungsi HitungSudut2
// yang terdapat pada file "myMotor.h"

// berikut contoh pengaturan sudut motor Servo 1 sampai dengan 8
// Motor Servo 1 sampai dengan 4 diatur dengan sudut yang sama dengan servo 5 sampai dengan 8
// Silakan diubah posisi sudutnya sesuai yang diinginkan untuk mencoba.
    while(1)
    {   if(Tombol1)                             // jika push button 1 ditekan
        {   servo.Servo1(0);
            servo.Servo5(0);
            servo.Servo2(45);
            servo.Servo6(45);
            servo.Servo3(90);
            servo.Servo7(90);
            servo.Servo4(135);
            servo.Servo8(135);
            lcd.Tulis(0, "Posisi Servo #1 ");
            lcd.Tulis(1, "0   45   90  135");
        }
        else if(Tombol2)                        // jika push button 2 ditekan
        {   servo.Servo1(45);
            servo.Servo5(45);
            servo.Servo2(90);
            servo.Servo6(90);
            servo.Servo3(135);
            servo.Servo7(135);
            servo.Servo4(180);
            servo.Servo8(180);
            lcd.Tulis(0, "Posisi Servo #2 ");
            lcd.Tulis(1, "45   90  135 180");
        }
        else if(Tombol3)                        // jika push button 3 ditekan
        {   servo.Servo1(90);
            servo.Servo5(90);
            servo.Servo2(135);
            servo.Servo6(135);
            servo.Servo3(180);
            servo.Servo7(180);
            servo.Servo4(90);
            servo.Servo8(90);
            lcd.Tulis(0, "Posisi Servo #3 ");
            lcd.Tulis(1, "90  135  180  90");
        }
        else if(Tombol4)                        // jika push button 4 ditekan
        {   servo.Servo1(135);
            servo.Servo5(135);
            servo.Servo2(180);
            servo.Servo6(180);
            servo.Servo3(45);
            servo.Servo7(45);
            servo.Servo4(0);
            servo.Servo8(0);
            lcd.Tulis(0, "Posisi Servo #4 ");
            lcd.Tulis(1, "135  180  45   0");
        }
    }
}
