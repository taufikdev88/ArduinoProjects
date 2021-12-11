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
{   SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"
    // untuk mengaktifkan IMU, maka set opsi UseIMU = 1
    // opsi tersebut dapat dilihat pada file "myModule.h" (pada baris 31)
    // selanjutnya pada bagian ImuFilter, pilih opsi DMP
    // selain itu dapat dilih opsi untuk orientasi sudut 0 d`ari board AdRoiT memalui opsi PosisiBoard (TEGAK atau TIDUR)

    // program komputasi IMU dapat dilihat pada file "myIMU_DMP.h"
    int aPitch;
    imu.setDMPEnabled(true);
    imu.resetFIFO();
    imu.Start();
    uint8_t LastMode = 100; // nilai selain 0-15


    while(1)
    {
        aPitch = imu.getPitch()*RAD2DEG;
        if(aPitch==0)
        {
           roda.SpeedPWM(0,0);
           lcd.GotoXY(0,1);    lcd.Data('P');
           lcd.Float(imu.getPitch()*RAD2DEG,3);    lcd.Data(' ');
        }
        else if(aPitch<0)
        {
           roda.SpeedPWM(-100,-100);
           lcd.GotoXY(0,1);    lcd.Data('P');
           lcd.Float(imu.getPitch()*RAD2DEG,3);    lcd.Data(' ');
        }
        else if(aPitch>0)
        {
           roda.SpeedPWM(100,100);
           lcd.GotoXY(0,1);    lcd.Data('P');
           lcd.Float(imu.getPitch()*RAD2DEG,3);    lcd.Data(' ');
        }
        adc.CekBatere();
        _delay_ms(10);
}
}
