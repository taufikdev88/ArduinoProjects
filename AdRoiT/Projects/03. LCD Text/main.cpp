/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Menampilkan data Text dan Angka ke LCD
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
{   int8_t counter = -125;
    SystemInit();               // inisialisasi sistem
    lcd.Hapus();                // meghapus layar LCD secara keseluruhan


    // karena LCD yang digunakan adalah 16 karakter per baris, maka sebaiknya karakter yang ditulis pada
    // fungsi berikut dibuat panjangnya sebanyak 16 karakter
    // berikut beberapa metode/animasi penulisan dan penghapusan tampilan pada LCD
    lcd.TulisTengah (0,"Program LCD Text");     // menuliskan text dari tengah layar pada baris 1
    lcd.TulisPinggir(1,">>>>>>>><<<<<<<<");     // menuliskan text dari pinggir layar pada baris 2
    _delay_ms(1000);
    lcd.HapusKiri(0);                           // menghapus tulisan pada LCD baris 1 dari kiri layar
    lcd.HapusKanan(1);                          // menghapus tulisan pada LCD baris 2 dari kanan layar

    lcd.TulisKiri (0, "Tulis dari Kiri.");      // menuliskan text dari kiri layar pada baris 1
    lcd.TulisKanan(1, "Tulis dari Kanan");      // menuliskan text dari kanan layar pada baris 2
    _delay_ms(1000);
    lcd.HapusPinggir(0);                        // menghapus tulisan pada LCD baris 1 dari pinggir layar
    lcd.HapusTengah(1);                         // menghapus tulisan pada LCD baris 2 dari tengah layar

    lcd.Tulis     (0, "Tulis ke Baris 1");      // menuliskan text secara lengsung (tanpa delay) ke baris 1
    lcd.Tulis     (1, "Tulis ke Baris 2");      // menuliskan text secara lengsung (tanpa delay) ke baris 2
    _delay_ms(1000);
    lcd.HapusBaris(0);                          // menghapus tulisan pada LCD baris 1 tanpa delay
    lcd.HapusBaris(1);                          // menghapus tulisan pada LCD baris 2 tanpa delay

    lcd.Tulis     (0, "Desimal = ");
    lcd.Tulis     (1, "Biner = ");

    // untuk melihat lebih detail fungsi-fungsi untuk mengakses LCD text pada AdRoiT yang sudah disediakan,
    // dapat dilihat pada fule myLCD.h
    // untuk mengakses LCD dapat digunakan obyek "lcd".
    // fungsi-sungsi dapat diakses dengan menggunakan operator dor "."
    // kita tinggal mengetikkan lcd.  (selanjutnya akan muncul daftar fungsi-sungsi yang akan diakses)

    // berikut program untuk menampilkan data adlam bentuk angka (data dari variabel counter)
    while(1)
    {   lcd.GotoXY    (11,0);               // menempatkan kursor pada  baris ke-1 kolom ke-12  (tambah 1 karena berawal dari 0)
        lcd.sByte(counter);                 // menampilkan angka bilangan 8 bit dengan tanda (range antara -128 sampai dengan 127)
        lcd.GotoXY    (8,1);                // menempatkan kursor pada  baris ke-2 kolom ke-9   (tambah 1 karena berawal dari 0)
        lcd.Biner(counter);                 // menampilkan data 8 bit dalam bentuk biner (range antara 00000000 sampai dengan 11111111)
        if(++counter>125) counter = -125;   // me reset hitungan ke -125 ketika data counter melampaui hitungan 125
        _delay_ms(100);                     // delay 100 ms untuk menampilkan data hitungan
    }
}
