/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C
Code    : - Program untuk menampilkan data rotari enkoder (kecepatan dan posisi putaran)
          - dilengkapi degan Pengaturan kecepatan motor DC (motor roda) dengan PWM

This program is free software: you can redistribute it and/or modify it under
the terms of the version 3 GNU General Public License as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
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

    lcd.TulisKiri  (0, "Atur Kecp. Motor");
    lcd.TulisKiri  (1, "Dgn. Push Button");
    buzzer.Nada1();
    _delay_ms(500);
    lcd.HapusTengah(0);
    lcd.HapusPinggir(1);
    //              0123456789012345
    lcd.Tulis  (0, "VMKi:   VMKa:   ");
    lcd.Tulis  (1, "i:      a:      ");

    // data enkoder dapat dilihat dari data obyek roda yang dideklarasikan pada file "myMotor.h"
    // data enkoder yang dapat diakses antara lain
    // a. dCounterKx= data counter 8 bit yang sifatnya selalu positif (selalu bertambah setiap kali ada putaran/ tidak perduli arah putarannya)
    //                setiap perubahan data dCounter mengasilkan 1/(17.6*16) = 0,00355 putaran pada shaft motor
    // b. dSpeedKx  = data kecepatan 8 bit yang menandakan keceptan motor yang disampling setiap 10ms
    //                setiap perubahan data kecepatam dSpeed mewakili 100*60/(16*17.6) = 20,3 rpm pada putaran shaft
    // c. EnkoderKx = data enkoder 16 bit yang berisi hitungan putaran dari motor yang nilainya bisa positif dan negatif
    // jika putaran ke arah maju, maka data Enkoder akan bertambah, dan jika berputar mundur akan berkurang hitungannya
    // dimana x=i untuk motor kiri dan x=a untuk motor kanan

    while(1)
    {   if(Tombol1)                             // jika push button 1 ditekan
        {   roda.SpeedPWM(100,100);             // kecepatan motor 1/4 dari kecepatan maksimal ke arah depan
        }
        else if(Tombol2)                        // jika push button 2 ditekan
        {   roda.SpeedPWM(400,400);             // kecepatan motor  maksimal ke arah depan
        }
        else if(Tombol3)                        // jika push button 3 ditekan
        {   roda.SpeedPWM(-100,-100);           // kecepatan motor 1/4 dari kecepatan maksimal ke arah belakang
        }
        else if(Tombol4)                        // jika push button 4 ditekan
        {   roda.SpeedPWM(-400,-400);           // kecepatan motor  maksimal ke arah belakang
        }
        else                                    // jika tidak ada push button yang ditekan
        {   roda.SpeedPWM(0,0);                 // motor berhenti berputar dan dalam kondisi ngerem (brake)
        }
        lcd.GotoXY(5,0);    lcd.uByte(roda.dSpeedKi);   // menampilkan data kecepatan motor kiri
        lcd.GotoXY(12,0);   lcd.uByte(roda.dSpeedKa);   // menampilkan data kecepatan motor kanan
        lcd.GotoXY(2,1);    lcd.Angka5(roda.EnkoderKi); // menampilkan data posisi hitungan enkoder motor kiri
        lcd.GotoXY(10,0);   lcd.Angka5(roda.EnkoderKa); // menampilkan data posisi hitungan enkoder motor kanan
    }
}
