/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Pengaturan I/O sederhana pada ADROIT V1
          - Membaca data dari Dip Switch dan ditampilkan ke LED
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

#include "myModule.h"                   // edit module yang akan digunakan pada file ini
#include "../AdRoiT_Files/mySystem.h"

int main(void)
{
    SystemInit();               // inisialisasi sistem
    lcd.Hapus();                // meghapus layar LCD
    lcd.Tulis(0," Program I/O #1 ");
    lcd.Tulis(1,"Dipswitch -> LED");

    // untuk menampilkan data ke LEDn menggunakan perintah LEDn(x), dimana:
    // LED menyala saat nilai x=1
    // LED padam saat nilai x=0
    // n = nomor LED yang di akses (1 s/d 4)

    // Untuk Membaca data dip switch digunakan perintah DipSwitchn, dimana:
    // DipSwitchn akan bernilai 1 ketika dip switch ON (posisi ke atas), dan bernilai 1 ketika OFF (posisi ke bawah)
    // n = nomor Dip Switch yang diakses (1 s/d 4)

    while(1)
    {   LED1(DipSwitch1);    // menampilkan data dip switch 1 ke LED1
        LED2(DipSwitch2);    // menampilkan data dip switch 2 ke LED2
        LED3(DipSwitch3);    // menampilkan data dip switch 3 ke LED3
        LED4(DipSwitch4);    // menampilkan data dip switch 4 ke LED4
    }

    // Selain dengan menggunakan Perintah DipSwitchn, data dip switch dapat juga diakses dengan perintah MODEn
    // Data dip switch juga bisa diakses secara 4 bit (dengan menggunakan perintah MODE)
    // data MODE akan bernilai antara 0 sampai dengan 15 (kombinasi biner 0000 s/d 1111)

    /*
    while(1)
    {   LED1(MODE1);    // menampilkan data dip switch 1 ke LED1
        LED2(MODE2);    // menampilkan data dip switch 2 ke LED2
        LED3(MODE3);    // menampilkan data dip switch 3 ke LED3
        LED4(MODE4);    // menampilkan data dip switch 4 ke LED4
    }
    */

}
