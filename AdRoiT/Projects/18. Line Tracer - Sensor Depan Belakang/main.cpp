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

/* beberapa alis untuk mempersingkat pemanggilan fungsi scan garis
   parameter fungsi yang dipanggil bisa di sesuaikan dengan kasus di lapangan
*/

// opsi kecepatan dalam kurung (o) minimal 100, maksimal 400
#define skamaju(o)          {AdRoiT.SetArahRobot(Maju);     AdRoiT.Scan3Kanan(o);}              // scan percabangan 3 sensor kanan arah maju
#define skimaju(o)          {AdRoiT.SetArahRobot(Maju);     AdRoiT.Scan3Kiri(o);}               // scan percabangan 3 sensor kiri arah maju
#define skamaju2(o)         {AdRoiT.SetArahRobot(Maju);     AdRoiT.Scan2Kanan(o);}              // scan percabangan 2 sensor kanan arah maju
#define skimaju2(o)         {AdRoiT.SetArahRobot(Maju);     AdRoiT.Scan2Kiri(o);}               // scan percabangan 2 sensor kiri arah maju
#define stgmaju(o)          {AdRoiT.SetArahRobot(Maju);     AdRoiT.Scan3Tengah(o);}             // scan percabangan 3 sensor tengah arah maju
#define skamundur(o)        {AdRoiT.SetArahRobot(Mundur);   AdRoiT.Scan3Kiri(o);}               // scan percabangan 3 sensor kanan arah mundur
#define skimundur(o)        {AdRoiT.SetArahRobot(Mundur);   AdRoiT.Scan3Kanan(o);}              // scan percabangan 3 sensor kiri arah mundur
#define skamundur2(o)       {AdRoiT.SetArahRobot(Mundur);   AdRoiT.Scan2Kiri(o);}               // scan percabangan 2 sensor kanan arah mundur
#define skimundur2(o)       {AdRoiT.SetArahRobot(Mundur);   AdRoiT.Scan2Kanan(o);}              // scan percabangan 2 sensor kiri arah mundur
#define stgmundur(o)        {AdRoiT.SetArahRobot(Mundur);   AdRoiT.Scan3Tengah(o);}             // scan percabangan 3 sensor tengah arah maju

// opsi kecepatan dalam kurung (a, b) minimal 0, maksimal 20
// a=data roda kiri, sedangkan b=data roda kanan (posisi rodak dilihat dari belakang robot dengan tulisan dapat LCD terbaca / tidak terbalik)
// [erintah berikut untuk belokan model perempatan (90 derajat)
#define belkamaju(a,b)      {AdRoiT.SetArahRobot(Maju);     AdRoiT.BelokKananGarisPID(a,b);}    // belok kanan pada garis arah maju sampai ketemu garis lagi
#define belkamundur(a,b)    {AdRoiT.SetArahRobot(Mundur);   AdRoiT.BelokKiriGarisPID(b,a);}     // belok kanan pada garis arah mundur sampai ketemu garis lagi
#define belkimaju(a,b)      {AdRoiT.SetArahRobot(Maju);     AdRoiT.BelokKiriGarisPID(a,b);}     // belok kiri pada garis arah maju sampai ketemu garis lagi
#define belkimundur(a,b)    {AdRoiT.SetArahRobot(Mundur);   AdRoiT.BelokKananGarisPID(b,a);}    // belok kiri pada garis arah mundur sampai ketemu garis lagi
// perintah berikut untuk belokan serong kurang dari 90 derajat)
#define serkamaju(a,b)      {AdRoiT.SetArahRobot(Maju);     AdRoiT.BelokKananSampaiGarisPID(a,b, 0B00001111 );}  // belok serong kanan pada garis arah maju sampai ketemu garis lagi
#define serkimaju(a,b)      {AdRoiT.SetArahRobot(Maju);     AdRoiT.BelokKiriSampaiGarisPID(a,b, 0B11110000);}   // belok serong kiri pada garis arah mundur sampai ketemu garis lagi
#define serkamundur(a,b)    {AdRoiT.SetArahRobot(Mundur);   AdRoiT.BelokKiriSampaiGarisPID(b,a, 0B11110000);}   // belok serong kanan pada garis arah maju sampai ketemu garis lagi
#define serkimundur(a,b)    {AdRoiT.SetArahRobot(Mundur);   AdRoiT.BelokKananSampaiGarisPID(b,a, 0B00001111);}  // belok serong kiri pada garis arah mundur sampai ketemu garis lagi
// perintah berikut untuk berputar tanpa syarat (bisa dihentikan dengan waktu atau syarat tambahan)
#define putarka(a,b)        myAdroit::PutarKananPID(a,b);}      // robot berputar searah jarum jam
#define putarki(a,b)        myAdroit::PutarKiriPID(a,b);}       // robot berputar berlawanan arah jarum jam

#define stop                AdRoiT.Stop();


int main(void)
{   SystemInit();       // inisialisasi sistem (wajib bagi semua project)
                        // detail dari  modul yang diaktifkan pada setiap project bisa dilihat pada file "myModule.h"
    lcd.Hapus();
    myScanGaris AdRoiT; // mendeklarasikan obyek AdROiT sebagai obnyek dengan class myScanGaris
                        // lihat deklarasi class myScanGaris pada file "myADROIT.h"

    // untuk melakukan kalibrasi sensor depan, tekan dan tahan tombol 4 saat menyalakan robot
    if(Tombol4) sensor.KalibrasiRB(Depan);

    // untuk melakukan kalibrasi sensor belakang, tekan dan tahan tombol 3 saat menyalakan robot
    if(Tombol3) sensor.KalibrasiRB(Belakang);

    // perintah berikut digunakan untuk memilih warna sensor yang digunakan untuk tracking garis
    // asumsi warna garis = hitam dan warna lapangan putih
    AdRoiT.UpdateSensorGaris(Hitam, Putih); // untuk deklarasi warna bisa dilihat pada file "myDefinitions.h"

    // perintah berikut digunakan untuk memilih warna sensor yang digunakan untuk mendeteksi pertigaan
    // asumsi warna garis pertigaan = hitam dan warna lapangan putih
    AdRoiT.UpdateSensorPertigaan(Hitam, Putih);

    /*lcd.TulisPinggir(0, ">> AdRoiT V1. <<");
    lcd.TulisTengah (1, "Mode Line Tracer");

    delay_ms(500);
    lcd.HapusPinggir(0);
    lcd.HapusTengah(1);
*/
    lcd.Tulis(0,"S.Depan:");
    lcd.Tulis(1,"S.Blkg :");
    // menunggu penekanan tombol
    while(!Tombol1 && !Tombol2) // menampilkan data sensor merah dan biru selama belum ditekan tombol 1 atau tombol 2
    {   lcd.GotoXY(8,0);
        lcd.Biner(sensor.BacaGaris(SensorDepan, SensorMerah));
        lcd.GotoXY(8,1);
        lcd.Biner(sensor.BacaGaris(SensorBelakang, SensorMerah));
    }
    lcd.Hapus();
    if(Tombol1) // jika tombol 1 yang ditekan
    {   buzzer.Nada1();
        /*skamaju(200);
        skamaju(400);
        skamaju(300);
        skamaju(200);
        skamaju(150);       // poin tengah
        skamundur(200);
        skamundur(400);
        skamundur(400);
        skamundur(250);
        belkamundur(10,10);
        skamundur(200);
        skamundur(300);
        belkamundur(5,10);
        skamundur(250);
        skamundur(350);
        skamundur(200);
        skamundur(150);       // poin kanan dalam
        skimaju(200);
        skimaju(400);
        skimaju(300);
        belkimaju(10,10);
        skamaju(300);
        belkimaju(10,8);
        skamaju(300);
        skamaju(350);
        skamaju(200);
        skamaju(150);       // poin kanan luar
        skamundur(200);
        skamundur(400);
        skamundur(300);
        belkamundur(10,10);
        skamundur(200);
        skamundur(150);     // poin paling kanan
        skamaju(150);
        skamaju(350);
        skamaju(350);
        skamaju(300);
        skamaju(300);
        skamaju(350);
        skamaju(350);
        skamaju(200);
        skamaju(150);       // poin paling kiri

        skamundur(200);
        belkamundur(15,5);
        skamundur(250);
        skamundur(350);
        skamundur(200);
        skamundur(150);     // poin kiri luar
        skimaju(200);
        skimaju(400);
        skimaju(300);
        belkimaju(10,10);
        skimaju(375);
        belkimaju(10,10);
        skimaju(300);
        skimaju(350);
        skimaju(200);
        skimaju(150);       // poin kiri dalam
        skamundur(200);
        skamundur(400);
        skamundur(300);
        belkamundur(10,10);
        skimundur(400);
        skimundur(250);
        belkamaju(10,10);*/


        skamaju(200);
        skamaju2(300);
        belkamaju(15,10);
        skamaju(250);
        skamaju(150);           // poin paling kanan
        skimundur(300);
        belkimundur(10,10);
        stgmundur(300);
        belkimundur(8,8);
        skimundur(200);
        skimundur(250);
        skimundur(150);         // poin kanan luar
        skimaju(300);
        skimaju2(250);
        skimaju(220);
        skimaju(200);
        skimaju(150);           // poin kanan dalam
        skimundur(300);
        skimundur2(220);
        belkimundur(10,10);
        skimundur(300);
        belkimundur(10,10);
        skamundur2(250);
        skamundur2(300);
        belkamundur(10,10);
        skimundur(300);
        AdRoiT.ScanSampaiSensor(300, 0B00000111);

        AdRoiT.UpdateSensorGaris(Putih,Hitam);          // masuk daerah tengah, warna dibalik
        AdRoiT.UpdateSensorPertigaan(Putih,Hitam);
        skamundur2(250);
        skamundur(300);
        belkamundur(10,10);
        skamundur(250);
        skamundur(150);         // poin tengah
        skamaju(300);
        belkamaju(10,10);
        stgmaju(250);
        //AdRoiT.PutarKananPID(10,0); delay_ms(180);
        serkamaju(10,5)

        //skimaju(220);
        AdRoiT.ScanSampaiSensor(220, 0B11100000);

        AdRoiT.UpdateSensorGaris(Hitam,Putih);          // keluar daerah tengah, warna dibalik
        AdRoiT.UpdateSensorPertigaan(Hitam,Putih);
        skimaju(300);
        belkimaju(10,10);
        stgmaju(250);
        //AdRoiT.PutarKananPID(10,0); delay_ms(180);
        serkamaju(10,5);
        skimaju(230);
        skimaju(150);           // poin paling kiri
        skamundur(300);
        belkamundur(10,10);

        stgmundur(300);
        belkimundur(8,8);
        skimundur(200);
        skimundur(250);
        skimundur(150);         // poin kiri dalam
        skimaju(300);
        skimaju2(250);
        skimaju(220);
        skimaju(200);
        skimaju(150);           // poin kiriluar
        skimundur(300);
        skimundur2(220);
        belkimundur(10,10);
        skimundur(300);
        belkimundur(10,10);
        //start looping
        skamaju2(250);
        belkamaju(15,10);
        skamaju(275);
        skamaju(300);
        skamaju(300);
        skamaju2(300);
        belkamaju(15,10);
        skamaju(275);
        skamaju(150);
        skimundur(300);
        belkimundur(10,10);

        stgmundur(300);
        belkimundur(8,8);
        skimundur(200);
        skimundur(250);
        skimundur(150);         // poin kiri dalam
        skimaju(300);
        skimaju2(250);
        skimaju(220);
        skimaju(200);
        skimaju(150);           // poin kiriluar
        skimundur(300);
        skimundur2(220);
        belkimundur(10,10);
        skimundur(300);
        belkimundur(10,10);

























        //belkamaju(15,5);
        AdRoiT.Stop();
        while (1);
    }
    if(Tombol2) // jika tombol 2 yang ditekan
    {   buzzer.Nada2();
        skamundur(200);
        belkamundur(5,5);
        AdRoiT.Stop();
        while (1);
    }
    // memasitikan kondisi terakhir robot berhenti
    AdRoiT.Stop();  while(1);
}
