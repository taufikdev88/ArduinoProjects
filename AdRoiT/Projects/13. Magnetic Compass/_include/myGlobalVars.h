#ifndef MYGLOBALVARS_H_INCLUDED
#define MYGLOBALVARS_H_INCLUDED

/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Deklarasi variabel global
*********************************************************/

enum Warna  {Hitam=0, Merah, Hijau, Biru, Putih};

struct Flag
{
    uint8_t LcdBacklight:1;
    uint8_t SerialTimeOut:1;
    uint8_t AdcBusy:1;

#if UseCompass == 1
    uint8_t CompassStart:1;
#endif // UseCompass

#if UsePIDmotor == 1
    uint8_t PIDMotorOn:1;
#endif // UsePIDmotor

#if UseMusic == 1
    uint8_t LaguOn:1;
#endif // UseMusic

#if  UseIMU == 1
    uint8_t ImuStart:1;
    uint8_t ImuToggle:1;
#endif // UseIMU
};

struct Flag2
{
    uint8_t FlagBelok:1;
};


extern  Flag    myFlag;
extern  Flag2   myFlag2;
//extern  int16_t xxx;

#endif // MYGLOBALVARS_H_INCLUDED
