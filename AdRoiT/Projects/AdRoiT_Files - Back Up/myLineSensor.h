#ifndef MYLINESENSOR_H_INCLUDED
#define MYLINESENSOR_H_INCLUDED

/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Line Sensor Access and Callibration Routines

This program is free software: you can redistribute it and/or modify it under
the terms of the version 3 GNU General Public License as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*********************************************************/

#include "mylcd.h"
#include "myBuzzer.h"
#include "myUART.h"

class mySensorGaris
{   public:
        uint8_t BacaGaris(uint8_t Posisi, uint8_t Warna);
        int8_t  KalibrasiRGB(uint8_t Alamat);
        int8_t  KalibrasiRB(uint8_t Alamat);
    private:
        //void    BacaSensor(uint8_t Alamat, uint8_t Protokol);
        uint8_t TungguTombolKalibrasi(uint8_t Alamat, uint8_t Protokol);
        void    PesanKalibrasiBerhasil(void);
        void    PesanKalibrasiGagal(void);
        void    PesanSensor(void);
}sensor;

uint8_t mySensorGaris::BacaGaris(uint8_t Posisi, uint8_t Warna)
{   putchar1(Posisi|Warna);;
    if(BacaSerial1(5000)==(Posisi|Warna))
    {   return BacaSerial1(5000);
    }
    else return 0;
}

// Fungsi Baca Sensor lewat komunikasi Serial
//void mySensorGaris::BacaSensor(uint8_t Alamat, uint8_t Protokol)
//{   putchar1(Protokol | Alamat);
//}

uint8_t mySensorGaris::TungguTombolKalibrasi(uint8_t Alamat, uint8_t Protokol)
{   while (!Tombol1 && !Tombol4);   //Menunggu penekanan Tombol 1 untuk lanjut, tombol 4 untuk cancel
    if(Tombol1)     {   putchar1(Protokol | Alamat); return 1;}
    else            {   putchar1(0);                 return 0;}    // jika tombol 4 ditekan, berarti proses dihentikan
}


void mySensorGaris::PesanSensor(void)
{   lcd.Hapus();    // Hapus layar
    lcd.Tulis       (0, "Kalibrasi Sensor");
};

void mySensorGaris::PesanKalibrasiBerhasil(void)
{   PesanSensor();
    lcd.TulisKiri   (1, "Selesai >>>>>>>>");
}

void mySensorGaris::PesanKalibrasiGagal(void)
{   PesanSensor();
    lcd.TulisKiri   (1, "Dihentikan >>>>>");
}

int8_t mySensorGaris::KalibrasiRGB(uint8_t Alamat)
{   PesanSensor();
    lcd.Tulis       (1, "Merah-Hijau-Biru");
    buzzer.Beep(3000,500);
    _delay_ms(1000);
    lcd.Hapus();         // Hapus layar
    putchar1(pKalibrasiRGB | Alamat);
    lcd.TulisKiri   (0, "1.Warna Merah >>");
    lcd.TulisKanan  (1, "Tekan Tombol 1<<");
    if(TungguTombolKalibrasi(Alamat, pKalibrasiRGB))
    {   buzzer.Beep(3000,500);
        lcd.HapusKanan(0);
        lcd.TulisKiri   (0, "2.Warna Hijau >>");
        if(TungguTombolKalibrasi(Alamat, pKalibrasiRGB))
        {   buzzer.Beep(3000,500);
            lcd.HapusKanan(0);
            lcd.TulisKiri  (0, "3.Warna Biru >>>");
            if(TungguTombolKalibrasi(Alamat, pKalibrasiRGB))
            {   buzzer.Beep(3000,500);
                lcd.HapusKanan(0);
                lcd.TulisKiri  (0, "4.Warna Hitam >>");
                if(TungguTombolKalibrasi(Alamat, pKalibrasiRGB))
                {   buzzer.Beep(3000,200);
                    if(BacaSerial1(5000)==pKalibrasiRGB)
                    {  PesanKalibrasiBerhasil();
                       return(1);
                    }
                }
            }
        }
    }
    PesanKalibrasiGagal();
    return(0);
}

int8_t mySensorGaris::KalibrasiRB(uint8_t Alamat)
{   PesanSensor();
    lcd.Tulis       (1, " Merah dan Biru ");
    buzzer.Beep(3000,500);
    _delay_ms(1000);
    lcd.Hapus();         // Hapus layar
    putchar1(pKalibrasiRB | Alamat);
    lcd.TulisKiri   (0, "1.Warna Merah >>");
    lcd.TulisKanan  (1, "Tekan Tombol 1<<");
    if(TungguTombolKalibrasi(Alamat, pKalibrasiRB))
    {   buzzer.Beep(3000,200);
        lcd.HapusKanan(0);
        lcd.TulisKiri   (0, "2.Warna Biru  >>");
        if(TungguTombolKalibrasi(Alamat, pKalibrasiRB))
        {   buzzer.Beep(3000,200);
            lcd.HapusKanan(0);
            lcd.TulisKiri  (0, "3.Warna Hitam >>>");
            if(TungguTombolKalibrasi(Alamat, pKalibrasiRB))
            {   buzzer.Beep(3000,200);
                if(BacaSerial1(5000)==(pKalibrasiRB|Alamat))
                {  PesanKalibrasiBerhasil();
                   return(1);
                }
            }
        }
    }
    PesanKalibrasiGagal();
    return(0);
}



#endif // MYLINESENSOR_H_INCLUDED
