/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Pengaturan I/O sederhana pada ADROIT V1
          - Membaca data dari penekanan Push Button dan ditampilkan ke LED
*****************************************************************/

/*
Jika menggunakan Code::Block sebagai editor:
untuk optimasi supaya file hex hasil kompilasi menjadi kecil (fungsi yang tidak dipanggil tidak dimasukkan ke dalam
file hex), maka dilakukan pengaturan proses compile dan linker program sbb:
1. copy option berikut pada compiler setting    (Menu Settings,Compiler, Compiler settings, Other options:)
   -ffunction-sections
2. copy option berikut pada linker setting      (Menu Settings,Compiler, Linker settings, Other linker options:)
   -Wl,-gc-sections
*/

#include "myModule.h"               // memilih modul yang digunakan
#include "../AdRoiT_Files/mySystem.h"

int main(void)
{
    SystemInit();            // inisialisasi sistem
    lcd.Hapus();
    lcd.Tulis(0," Program I/O #1 ");
    lcd.Tulis(1,"PushButton-> LED");

    // untuk menampilkan data ke LEDn menggunakan perintah LEDn(x), dimana:
    // LED menyala saat nilai x=1
    // LED padam saat nilai x=0
    // n = nomor LED yang di akses (1 s/d 4)

    // Untuk Membaca penekanan tombol digunakan perintah Tomboln, dimana:
    // Tomboln akan bernilai 1 ketika push button ditekan, dan bernilai 0 ketika tidak ditekan
    // n = nomor Push Button yang diakses (1 s/d 4)
    while(1)
    {   LED1(Tombol1);    // menampilkan data penekanan Push Button 1 ke LED1
        LED2(Tombol2);    // menampilkan data penekanan Push Button 2 ke LED2
        LED3(Tombol3);    // menampilkan data penekanan Push Button 3 ke LED3
        LED4(Tombol4);    // menampilkan data penekanan Push Button 4 ke LED4
    }
}
