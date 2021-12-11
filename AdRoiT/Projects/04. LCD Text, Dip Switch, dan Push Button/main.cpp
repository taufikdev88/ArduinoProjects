/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Pengaturan I/O sederhana pada ADROIT V1
          - Menampilkan data Text dan Angka ke LCD
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
    SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"

    lcd.Hapus();
    lcd.TulisKanan(0,"Program LCD + IO");
    lcd.TulisKiri (1,">>>>>>>>>>>>>>>>");
    _delay_ms(1000);
    lcd.HapusKiri(1);
    lcd.HapusKanan(0);
    lcd.Tulis     (0, "T1: T2: T3: T4: ");      // template tampilan untuk push button (tombol)
    lcd.Tulis     (1, "DipSwitch = ");          // template tampilan untuk dip switch

    while(1)
    {   // menampilkan data push button ke LCD
        // tombol 1
        lcd.GotoXY    (3,0);            // menempatkan kursor pada  baris ke-1 kolom ke-4  (tambah 1 karena berawal dari 0)
        if(Tombol1) lcd.Data('1');      // menampilkan angka 1 jika tombol 1 ditekan
        else        lcd.Data('0');      // menampilkan angka 0 jika tombol tidak ditekan

        // tombol 2
        lcd.GotoXY    (7,0);            // menempatkan kursor pada  baris ke-1 kolom ke-8  (tambah 1 karena berawal dari 0)
        if(Tombol2) lcd.Data('1');      // menampilkan angka 1 jika tombol 2 ditekan
        else        lcd.Data('0');      // menampilkan angka 0 jika tombol tidak ditekan

        // tombol 3
        lcd.GotoXY    (11,0);           // menempatkan kursor pada  baris ke-1 kolom ke-12  (tambah 1 karena berawal dari 0)
        if(Tombol3) lcd.Data('1');      // menampilkan angka 1 jika tombol 3 ditekan
        else        lcd.Data('0');      // menampilkan angka 0 jika tombol tidak ditekan

        // tombol 4
        lcd.GotoXY    (15,0);           // menempatkan kursor pada  baris ke-1 kolom ke-16  (tambah 1 karena berawal dari 0)
        if(Tombol4) lcd.Data('1');      // menampilkan angka 1 jika tombol 4 ditekan
        else        lcd.Data('0');      // menampilkan angka 0 jika tombol tidak ditekan

        // menampilkan data dip switch dalam bentuk desimal
        lcd.GotoXY    (13,1);           // menempatkan kursor pada  baris ke-2 kolom ke-14  (tambah 1 karena berawal dari 0)
        lcd.uByte(MODE);                // menampilkan data dip switch dalam bentuk desimal

        // menampilkan data dip switch ke LED
        LED1(MODE1);                    // menampilkan data pushbutton ke-1 ke LED 1
        LED2(MODE2);                    // menampilkan data pushbutton ke-2 ke LED 2
        LED3(MODE3);                    // menampilkan data pushbutton ke-3 ke LED 3
        LED4(MODE4);                    // menampilkan data pushbutton ke-4 ke LED 4
    }
}
