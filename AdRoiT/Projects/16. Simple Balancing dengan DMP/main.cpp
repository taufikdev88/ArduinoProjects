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
    lcd.Hapus();

    // untuk mengaktifkan IMU, maka set opsi UseIMU = 1
    // opsi tersebut dapat dilihat pada file "myModule.h" (pada baris 31)
    // selain itu dapat dilih opsi untuk orientasi sudut 0 dari board AdRoiT memalui opsi PosisiBoard (TEGAK atau TIDUR)
    //lcd.Tulis(0, "Roll  = ");
    //lcd.Tulis(1, "Pitch = ");

    imu.setDMPEnabled(true);
    imu.resetFIFO();
    Balancing.Stop();
    imu.Start();        // perintah untuk mulai melakukan kalkulasi sudut Roll dan Pitch pada IMU
                        // IMU dikalkulasi menggunakan interupsi timer 1 setiap 4 atau 8 ms sekali
                        // program komputasi IMU dapat dilihat pada file "myIMU.h"
    Balancing.SetSpeed(0);
    lcd.Tulis(1,"Counting : ");
    int8_t c= 10;
    do{ lcd.GotoXY(11,1); lcd.Data(--c+48);_delay_ms(500);}while(c);
    lcd.Tulis(1,"Balancing Start.");
    buzzer.Beep(2000,200);
    Balancing.Start();
    while(1)
    {   lcd.GotoXY(0,0);    lcd.Float(Error,2);
        Balancing.SetSteer(50);
        delay_ms(2000);
        Balancing.SetSteer(-50);
        delay_ms(2000);

        /*lcd.GotoXY(8,0);    lcd.Angka5(imu.Gx);
        lcd.GotoXY(0,1);    lcd.Angka5(imu.Gy);
        lcd.GotoXY(8,1);    lcd.Angka5(imu.Gz);
        if (x<imu.Gy)x=imu.Gy;
        lcd.GotoXY(0,0);
        lcd.Angka4(imu.q.w*1000);

        lcd.GotoXY(8,0);
        lcd.Angka4(imu.q.x*1000);

        lcd.GotoXY(0,1);
        lcd.Angka4(imu.q.y*1000);

        lcd.GotoXY(8,1);
        lcd.Angka4(imu.q.z*1000);



        lcd.GotoXY(0,0);
        lcd.Angka3u(xxx);   if(myFlag.SerialTimeOut)lcd.Float(imu.getRoll()*RAD2DEG,1);

        lcd.GotoXY(8,0);    lcd.Data('R');
        lcd.Float(imu.getRoll()*RAD2DEG,2);

        lcd.GotoXY(0,1);    lcd.Data('P');
        lcd.Float(imu.getPitch()*RAD2DEG,2);

        lcd.GotoXY(8,1);    lcd.Data('Y');
        lcd.Float(imu.getYaw()*RAD2DEG,2);
*/
        adc.CekBatere();
        _delay_ms(50);
    }
}
