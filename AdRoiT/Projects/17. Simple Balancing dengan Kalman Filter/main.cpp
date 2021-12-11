/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Mode balancing dengan pengaturan Kecepatan dan Steer secara manual
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
#include "myModule.h"       // memilih modul yang digunakan
#include "../AdRoiT_Files/mySystem.h"

int main(void)
{   SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"
    lcd.Hapus();
    // Untuk mengaktifkan mode Balancing, maka Mode tersebut dengan mengatur ModeBalancing = 1 (pada file "myModule.h")
    // Selain itu, opsi UseIMU juga harus di set = 1, karena mode Balancing membutuhkan kalkulasi IMU

    // Set kecepatan robot saat mode balancing
    // data kecepatan robot antara -20 sampai dengan 20
    // data -20 kecepatan maksimal gerakan mundur
    // data 20 kecepatam maksimal gerakan maju
    // data 0 seting kecepatan nol, apabila bergerak, maka terjadi drift/ pergeseran titik tengah robot.
    // titik tengah saat mode balancing dapat diatur dengan mengubah nilai CenterOffset yang di define pada file "myAdRoiT.h"
    // nilai default CenterOffset adalah 0. (atur nilainya supaya saat kecepatan no, robot tidak bergerak maju atau mundur)
    Balancing.SetSpeed(0);

    // Set data steering saat mode balancing
    // data steering antara -125 sampai dengan 125
    // -125 = maksimal belok/putar ke kiri
    // +125 = maksimal belok/putar ke kanan
    Balancing.SetSteer(0);

    lcd.TulisPinggir(0, ">> AdRoiT V1. <<");
    lcd.TulisTengah (1, "<Mode Balancing>");

    while(!Tombol1);    //Mode Balancing belum aktif selama tombol 1 tidak ditekan
    Balancing.Start();    // Start mode balancing

    while(!Tombol4)     //Mode Balancing aktif tetap selama tombol 4 tidak ditekan dan tegangn betere cukup
    {   adc.CekBatere();    // memeriksa tegangan batere apakah masih > 6.5V (kalau tidak, sistem akan berhenti)
    }
    Balancing.Stop();     // Stop mode balancing
    while(1);
}
