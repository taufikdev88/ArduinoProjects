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

    lcd.TulisKanan (0, "Push Button : x ");
    lcd.TulisKiri  (1, "Atur Kecp. Motor");
    buzzer.Nada1();

    // Modul AdRoiT AVR dilengkapi dengan buzzer yang bisa diakses melalui obyek buzzer
    // detail dari fungsi-fungsi pada obyek buzzer bisa dilihat pada file "myBuzzer.h"
    // Selain fungsi buzzer, user juga bisa mengaktifkan mode lagu yang dieksekusi dengan menggunakan
    // timer 1 capture event (lihat file "mySystem.h" untuk lebih detailnya).
    // untuk mengaktifkan lagu ini, maka module lagu harus diaktifkan dahulu dengan membuat nilai UseMusic = 1
    // lihat detailnya pada file "myModule.h"

    // Data lagu yang dimainkan dapat diganti dengan mengganti kumpulan data yang dideklarasikan data Melodi[] dan
    // Tempo[] yang dideklarasikan pada file "myBuzzer.h"

    while(1)
    {   if(Tombol1)                             // jika push button 1 ditekan
        {   buzzer.Nada1();                     // menyalakan sekuen Nada1
        }
        else if(Tombol2)                        // jika push button 2 ditekan
        {   buzzer.Beep(1000,1000);             // mengeluarkan nada dengan frek 1KHz selama 1 detik
        }
        else if(Tombol3)                        // jika push button 3 ditekan
        {   buzzer.Nada2();                     // menyalakan sekuen Nada2
        }
        else if(Tombol4)                        // jika push button 4 ditekan
        {   buzzer.Lagu(ON);                    // memainkan data lagu (super mario), sampai tombol lain di tekan
        }
    }
}
