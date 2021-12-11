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

int main(void)
{   SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"
    lcd.Hapus();

    // asumsi sensor yang digunakan sensor depan (sensor dengan alamat 0)
    if(Tombol4) sensor.KalibrasiRGB(Depan);  // kalibrasi sensor dilakukan dengan menekan push button 4 saat start

    lcd.TulisPinggir(0, ">> AdRoiT V1. <<");
    lcd.TulisTengah (1, "Sensor Garis RGB");

    _delay_ms(1000);
    lcd.HapusPinggir(0);
    lcd.HapusTengah(1);

    lcd.Tulis(0,"SensRGB:");
    while(1) // menampilkan data sensor merah, hijau dan biru (dalam bentuk biner)
    {   lcd.GotoXY(8,0);
        lcd.Biner(sensor.BacaGaris(SensorDepan, SensorMerah));
        lcd.GotoXY(0,1);
        lcd.Biner(sensor.BacaGaris(SensorDepan, SensorHijau));
        lcd.GotoXY(8,1);
        lcd.Biner(sensor.BacaGaris(SensorDepan, SensorBiru));
    }
}
