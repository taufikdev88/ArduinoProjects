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
#include "../AdRoiT_Files/myADROIT.h"

#define SkaMaju(o)      {AdRoiT.SetArahRobot(Maju);     AdRoiT.Scan3Kanan(o);}
#define SkiMaju(o)      {AdRoiT.SetArahRobot(Maju);     AdRoiT.Scan3Kiri(o);}
#define SkaMundur(o)    {AdRoiT.SetArahRobot(Mundur);   AdRoiT.Scan3Kiri(o);}
#define SkiMundur(o)    {AdRoiT.SetArahRobot(Mundur);   AdRoiT.Scan3Kanan(o);}
#define BkaMaju(a,b)    {AdRoiT.SetArahRobot(Maju);     AdRoiT.BelokKananGarisPID(a,b);}
#define BkiMaju(a,b)    {AdRoiT.SetArahRobot(Maju);     AdRoiT.BelokKiriGarisPID(a,b);}
#define BkaMundur(a,b)  {AdRoiT.SetArahRobot(Mundur);   AdRoiT.BelokKiriGarisPID(a,b);}
#define BkiMundur(a,b)  {AdRoiT.SetArahRobot(Mundur);   dRoiT.BelokKananGarisPID(a,b);}


int main(void)
{   SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"
    lcd.Hapus();

    myScanGaris AdRoiT; // mendeklarasikan obyek AdROiT sebagai obnyek dengan class myScanGaris
                        // lihat deklarasi class myScanGaris pada file "myADROIT.h"

    // asumsi sensor yang digunakan sensor depan (sensor dengan alamat 0)
    if(Tombol4) sensor.KalibrasiRGB(Depan);  // kalibrasi sensor dilakukan dengan menekan push button 4 saat start
    // asumsi sensor yang digunakan sensor depan (sensor dengan alamat 0)
    if(Tombol3) sensor.KalibrasiRB(Belakang);  // kalibrasi sensor dilakukan dengan menekan push button 4 saat start

    // perintah berikut digunakan untuk memilih warna sensor yang digunakan untuk tracking garis
    // asumsi warna garis = hitam dan warna lapangan putih
    AdRoiT.UpdateSensorGaris(Hitam, Putih); // untuk deklarasi warna bisa dilihat pada file "myDefinitions.h"

    // perintah berikut digunakan untuk memilih warna sensor yang digunakan untuk mendeteksi pertigaan
    // asumsi warna garis pertigaan = hitam dan warna lapangan putih
    AdRoiT.UpdateSensorPertigaan(Hitam, Putih);
    // pengaturan arah geak robot
    AdRoiT.SetArahRobot(Maju);

    lcd.TulisPinggir(0, ">> AdRoiT V1. <<");
    lcd.TulisTengah (1, "Mode Line Tracer");

    delay_ms(500);
    lcd.HapusPinggir(0);
    lcd.HapusTengah(1);

    lcd.Tulis(0,"S.Hijau:");
    lcd.Tulis(1,"S.Merah:");
    while(!Tombol1 && !Tombol2) // menampilkan data sensor merah dan biru selama belum ditekan tombol 1
    {   lcd.GotoXY(8,0);
        lcd.Biner(~sensor.BacaGaris(SensorDepan, SensorHijau));
        lcd.GotoXY(8,1);
        lcd.Biner(~sensor.BacaGaris(SensorDepan, SensorMerah));
    }

    AdRoiT.UpdateSensorGaris(Hitam, Putih); // untuk deklarasi warna bisa dilihat pada file "myDefinitions.h"
    AdRoiT.UpdateSensorPertigaan(Hitam, Putih);
    lcd.Hapus();
    //SkaMaju(200);
    //BkaMaju(15,5);
    //SkaMaju(200);
    //SkaMaju(200);
    if(Tombol1) buzzer.Nada4();
    while(1)
    {
        while(((~sensor.BacaGaris(SensorDepan, SensorHijau) & 0B00100000) != 0B00100000)) AdRoiT.TraceUjungKanan(150);
        roda.Stop();buzzer.Nada4();
        while((~sensor.BacaGaris(SensorDepan, SensorHijau) & 0B00100000) == 0B00100000) AdRoiT.TraceUjungKanan(150);
        roda.Stop();buzzer.Nada3();
    }
    //if(Tombol2) while(1)SkaMundur(200);
    //SkaMundur(200);
    //BkaMundur(15,5);
    //SkaMundur(200);

    roda.Stop();
    lcd.TulisPinggir(0, ">>>>  Stop  <<<<");
    while(1);
}
