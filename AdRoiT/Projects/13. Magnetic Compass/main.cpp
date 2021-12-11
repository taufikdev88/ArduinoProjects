/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Menampilkan data kompas (baik dengan dn tanpa kompensasi kemiringan)
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

#include "myModule.h"       // memilih modul yang digunakan
#include "../AdRoiT_Files/mySystem.h"

// Untuk mengaktifkan module Kompas, maka ada 2 modul yang harus diaktifkan, yaitu IMU dan Kompas itu sensiri
// untuk mengaktifkan imu opsi UseIMU = 1, dan untuk kompas opsinya adalah UseCompass = 1
// kedua opsi tersebut berada pada file "myModule.h"
// Selain itu orientasi dari board perlu diatur dengan mengatur opsi PosisiBoard pada pilihan TIDUR atau TEGAK


int main(void)
{   int8_t lastMode=0;
    SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"
    lcd.Hapus();

    if(Tombol4) kompas.HardIronCallibration();
    lcd.TulisTengah (0, "< Data Heading >");
    lcd.TulisKiri   (1, "W/Wo Kompensasi.");
    lcd.HapusTengah(0);
    lcd.HapusTengah(1);

    imu.Start();        // start sampling dan komputasi IMU (roll dan Pitch)
    kompas.Start();     // start sampling data magneto meter
    while(1)
    {   if(MODE==0)             // posisi dip switch    Off Off Off Off
        {   lcd.Tulis(0, "Flat  : ");
            lcd.Float(kompas.FlatHeading(),2);  // data kompas yang tidak ada kompensasi kemiringan
            lcd.Tulis(1, "Tilted: ");
            lcd.Float(kompas.TiltedHeading(),2);// data kompas yang dengan kompensasi kemiringan
        }
        else if(MODE==1)        // posisi dip switch    Off Off Off On
        {   lcd.Tulis(0, "MagData: ");
            lcd.Angka4(kompas.Mx-kompas.magOffset[xAxis]);
            lcd.GotoXY(0,1);
            lcd.Angka4(kompas.My-kompas.magOffset[yAxis]);
            lcd.GotoXY(9,1);
            lcd.Angka4(kompas.Mz-kompas.magOffset[zAxis]);
        }
        else if(MODE==2)        // posisi dip switch    Off Off Off On
        {   lcd.Tulis(0, "MagRaw : ");
            lcd.Angka4(kompas.Mx);
            lcd.GotoXY(0,1);
            lcd.Angka4(kompas.My);
            lcd.GotoXY(9,1);
            lcd.Angka4(kompas.Mz);
        }
        else if(MODE==3)        // posisi dip switch    Off Off Off On
        {   lcd.Tulis(0, "OffSet : ");
            lcd.Angka4(kompas.magOffset[xAxis]);
            lcd.GotoXY(0,1);
            lcd.Angka4(kompas.magOffset[yAxis]);
            lcd.GotoXY(9,1);
            lcd.Angka4(kompas.magOffset[zAxis]);
        }
        else if(MODE==4)        // posisi dip switch    Off Off On  Off
        {   lcd.Tulis(0, "mScale: ");
            lcd.Float(kompas.Scale[xAxis],4);
            lcd.GotoXY(0,1);
            lcd.Float(kompas.Scale[yAxis],4);
            lcd.GotoXY(8,1);
            lcd.Float(kompas.Scale[zAxis],4);
        }
        if (MODE!=lastMode) lcd.Hapus();    // hapus layar LCD jika posisi mode (Dipswitch) berubah
        lastMode = MODE;
    }
}
