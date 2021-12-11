/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Menampilkan data sudut  Roll dan Pitch dari IMU ke LCD
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

    // untuk mengaktifkan IMU, maka set opsi UseIMU = 1
    // opsi tersebut dapat dilihat pada file "myModule.h" (pada baris 31)
    // selain itu dapat dilih opsi untuk orientasi sudut 0 dari board AdRoiT memalui opsi PosisiBoard (TEGAK atau TIDUR)
    lcd.Tulis(0, "Roll  = ");
    lcd.Tulis(1, "Pitch = ");

    imu.Start();        // perintah untuk mulai melakukan kalkulasi sudut Roll dan Pitch pada IMU
                        // IMU dikalkulasi menggunakan interupsi timer 1 setiap 4 atau 8 ms sekali
                        // program komputasi IMU dapat dilihat pada file "myIMU.h"
    while(1)
    {   lcd.GotoXY(8,0);
        lcd.Float(imu.Roll,3);
        lcd.GotoXY(8,1);
        lcd.Float(imu.Pitch,3);
    }
}
