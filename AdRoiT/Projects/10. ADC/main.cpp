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
    lcd.TulisKiri (0, ">> Program ADC >");
    lcd.TulisKiri  (1,"<< Kanal 0 s/d 8");
    buzzer.Nada1();

// Inisialisasi Penggunaan ADC pada ADROIT-AVR sudah diaktifkan melalui file "mySystem.h"
// Sedangkan seting frekwensi clock dan resolusi dari ADC dapat diatur pada class myADC yang dideklarasikan
// pada file "myADC.h"
// Kanal ADC yang bisa digunakan untuk keperluan umum hanya ada 7, yaitu kanal 1 sampai dengan kanal 7
// hal ini dikarenakan kanal 0 digunakan oleh sistem monitor tegangan sumber (batere).
// akan tetapi, data data kanal 0 tetap bisa ditampilkan.

    while(1)
    {   if(MODE1)   // saat dip-switch satu ON
        {   lcd.Tulis(0, "Ch0:Ch1:Ch2:Ch3:");
            lcd.GotoXY(0,1);            // pindah kursor ke kolom 1, baris ke-2
            lcd.Angka4u(adc.Baca(0));   // menampilkan data ADC kanal 0 (fix 4 angka positif)
            lcd.Angka4u(adc.Baca(1));   // menampilkan data ADC kanal 1 (fix 4 angka positif)
            lcd.Angka4u(adc.Baca(2));   // menampilkan data ADC kanal 2 (fix 4 angka positif)
            lcd.Angka4u(adc.Baca(3));   // menampilkan data ADC kanal 3 (fix 4 angka positif)
        }
        else
        {   lcd.Tulis(0, "Ch4:Ch5:Ch6:Ch7:");
            lcd.GotoXY(0,1);            // pindah kursor ke kolom 1, baris ke-2
            lcd.Angka4u(adc.Baca(4));   // menampilkan data ADC kanal 4 (fix 4 angka positif)
            lcd.Angka4u(adc.Baca(5));   // menampilkan data ADC kanal 5 (fix 4 angka positif)
            lcd.Angka4u(adc.Baca(6));   // menampilkan data ADC kanal 6 (fix 4 angka positif)
            lcd.Angka4u(adc.Baca(7));   // menampilkan data ADC kanal 7 (fix 4 angka positif)
        }
    }
}
