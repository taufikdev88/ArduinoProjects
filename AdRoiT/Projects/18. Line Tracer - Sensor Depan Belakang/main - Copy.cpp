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

int main(void)
{   SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"
    lcd.Hapus();

    myScanGaris AdRoiT; // mendeklarasikan obyek AdROiT sebagai obnyek dengan class myScanGaris
                        // lihat deklarasi class myScanGaris pada file "myADROIT.h"

    // asumsi sensor yang digunakan sensor depan (sensor dengan alamat 0)
    if(Tombol4) sensor.KalibrasiRB(Depan);  // kalibrasi sensor dilakukan dengan menekan push button 4 saat start

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

    _delay_ms(1000);
    lcd.HapusPinggir(0);
    lcd.HapusTengah(1);

    lcd.Tulis(0,"S.Merah:");
    lcd.Tulis(1,"S.Biru :");
    while(!Tombol1) // menampilkan data sensor merah dan biru selama belum ditekan tombol 1
    {   lcd.GotoXY(8,0);
        lcd.Biner(sensor.BacaGaris(SensorDepan, SensorMerah));
        lcd.GotoXY(8,1);
        lcd.Biner(sensor.BacaGaris(SensorDepan, SensorBiru));
    }

    while(!Tombol4) // line tracer akan berjalan sampai denga push button 4 ditekan
    {   // robot melakukan scanning menggunakan sensor kanan
        // kecepatan scanning  = 200 (dari maksimal 400)
        // kecepatan ketika melakukan pengereman = 100 (dari maksimal 400)
        // pengereman akan dilakukan setelah jumlah data counter kiri dan kanan >= 250 hitungan
        // kecepatan belok kanan adalah 15 untuk roda kiri dan 5 untuk motor kanan
        // kecepatan motor saat belok maksimal adalah 20
        // karena belok kanan, maka arah putaran motor kiri = maju (kedepan),
        // sedangkan motor kanan adalah mundur (kebelakang)
        AdRoiT.ScanBelokKanan(200,100,250,15,5);
        // lintasan robot adalah seperti berikut

        //      --->>>>>>------------------------
        //      |  posisi start robot           |
        //      |                               |
        //      |                               |
        //      |                               |
        //      |                               |
        //      |                               |
        //      |                               |
        //      |                               |
        //      |                               |
        //      |                               |
        //      |                               |
        //      ---------------------------------
        // robot akan melakukan looping scan garis sampai Tombol4 di tekan
    }
    AdRoiT.Stop();
    while(1);
}
