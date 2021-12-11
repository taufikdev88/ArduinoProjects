#ifndef MYGLOBALVARS_H_INCLUDED
#define MYGLOBALVARS_H_INCLUDED

/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Declaration of Global Variables and Global Type

This program is free software: you can redistribute it and/or modify it under
the terms of the version 3 GNU General Public License as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

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

extern  Flag    myFlag;

#if UseCompass==1
    uint16_t EEMEM magOffSetRom[3];     // data offset dari magnetometer X, Y dan Z
    float    EEMEM magScale[3];         // data penskalaan magnetometer X, Y dan Z terhadap nilai rata2
                                        // dari semua axis
#endif // UseEEPROM

#endif // MYGLOBALVARS_H_INCLUDED
