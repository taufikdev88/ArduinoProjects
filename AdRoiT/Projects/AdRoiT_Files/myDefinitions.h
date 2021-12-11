#ifndef MYDEFINITIONS_H_INCLUDED
#define MYDEFINITIONS_H_INCLUDED

/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Global Define and Global Functions used in ADROIT-AVR

This program is free software: you can redistribute it and/or modify it under
the terms of the version 3 GNU General Public License as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*********************************************************/

//  jika menggunakan CodeVision AVR
//    #define uint8_t unsigned char
//    #define int8_t  signed char
//    #define uint16_t unsigned short int
//    #define int16_t signed short int
//    #define uint32_t unsigned long int
//    #define int32_t signed long int

// konstanta konversi radian dan derajat
#define RAD2DEG     (float) 57.295779513082320876798154814105
#define DEG2RAD     (float) 0.01745329251994329576923690768489
#define Phi         (float) 3.1415926535897932384626433832795
#define DualPhi     (float) 6.283185307179586476925286766559

// protokol untuk sensor garis terbagi atas:
// 4 bit identifikasi perintah (bit 4-7)
// 4 bit identifikasi alamat sensor (bit 0-3) --> Alamat Sensor bernilai antara 0-15

#define ON          1
#define OFF         0

#define SetGlobalInterrupt        asm("sei"::)
#define ClrGlobalInterrupt        asm("cli"::)

#define pBacaSensorRGB  (uint8_t) 0B01110000
#define pBacaSensorRG   (uint8_t) 0B01100000
#define pBacaSensorRB   (uint8_t) 0B01010000
#define pBacaSensorGB   (uint8_t) 0B00110000
#define pBacaSensorR    (uint8_t) 0B01000000
#define pBacaSensorG    (uint8_t) 0B00100000
#define pBacaSensorB    (uint8_t) 0B00010000
#define pKalibrasiRGB   (uint8_t) 0B11110000
#define pKalibrasiRB    (uint8_t) 0B11010000
#define pKalibrasiRG    (uint8_t) 0B11100000
#define pKalibrasiHPR   (uint8_t) 0B11000000
#define pKalibrasiHPG   (uint8_t) 0B10100000
#define pKalibrasiHPB   (uint8_t) 0B10010000
#define pError          (uint8_t) 0B00000000

#define SensorMerah         pBacaSensorR
#define SensorHijau         pBacaSensorG
#define SensorBiru          pBacaSensorB

// definisi gerakan robot
#define SensorDepan         0
#define SensorBelakang      1
#define Depan               SensorDepan
#define Belakang            SensorBelakang
#define GerakMaju           0
#define GerakMundur         1
#define Maju                GerakMaju
#define Mundur              GerakMundur

// sudut awal dari masing-masing motor servo (0-180 derajat)
#define SudutAwalServo1     90
#define SudutAwalServo2     90
#define SudutAwalServo3     90
#define SudutAwalServo4     90
#define SudutAwalServo5     90
#define SudutAwalServo6     90
#define SudutAwalServo7     90
#define SudutAwalServo8     90

// Definisi Port LCD
#define LCD_PORT        PORTA
#define SetLCD_RS       (PORTA|=1)
#define ClrLCD_RS       (PORTA&=~1)
#define LCD_RS (o)      (o ? SetLCD_RS:ClrLCD_RS)

#define SetLCD_RW       (PORTA|=2)
#define ClrLCD_RW       (PORTA&=~2)
#define LCD_RW (o)      (o ? SetLCD_RW:ClrLCD_RW)

#define SetLCD_EN       (PORTA|=4)
#define ClrLCD_EN       (PORTA&=~4)
#define LCD_EN (o)      (o ? SetLCD_EN:ClrLCD_EN)

#define SetLCD_BL       (PORTA|=8)
#define ClrLCD_BL       (PORTA&=~8)
#define LCD_BL (o)      (o ? SetLCD_BL:ClrLCD_BL)

// Definisi Port Servo
#define SetP_Servo1     (PORTD|=128)
#define ClrP_Servo1     (PORTD&=~128)
#define P_Servo1 (o)    (o ? SetP_Servo1:ClrP_Servo1)

#define SetP_Servo2     (PORTD|=64)
#define ClrP_Servo2     (PORTD&=~64)
#define P_Servo2 (o)    (o ? SetP_Servo2:ClrP_Servo2)

#define SetP_Servo3     (PORTD|=32)
#define ClrP_Servo3     (PORTD&=~32)
#define P_Servo3 (o)    (o ? SetP_Servo3:ClrP_Servo3)

#define SetP_Servo4     (PORTD|=16)
#define ClrP_Servo4     (PORTD&=~16)
#define P_Servo4 (o)    (o ? SetP_Servo4:ClrP_Servo4)

#define SetP_Servo5     (PORTB|=8)
#define ClrP_Servo5     (PORTB&=~8)
#define P_Servo5 (o)    (o ? SetP_Servo5:ClrP_Servo5)

#define SetP_Servo6     (PORTB|=32)
#define ClrP_Servo6     (PORTB&=~32)
#define P_Servo6 (o)    (o ? SetP_Servo6:ClrP_Servo6)

#define SetP_Servo7     (PORTB|=64)
#define ClrP_Servo7     (PORTB&=~64)
#define P_Servo7 (o)    (o ? SetP_Servo7:ClrP_Servo7)

#define SetP_Servo8     (PORTB|=128)
#define ClrP_Servo8     (PORTB&=~128)
#define P_Servo8 (o)    (o ? SetP_Servo8:ClrP_Servo8)

//#define P_Servo6        PORTB.5 // OCR1A
//#define P_Servo7        PORTB.6 // OCR1B
//#define P_Servo8        PORTB.7 // OCR1C

#define dServo6 OCR1A
#define dServo7 OCR1B
#define dServo8 OCR1C

#define ByteL(a)    ((uint8_t) (a))
#define ByteH(a)    ((uint8_t) (((uint16_t) (a)) >> 8))
#define Bit(x,y)    (x & (1<<y))
//#define BacadServo8   peekw(&OCR1CL)
//#define BacaPwmM1     peekw(&OCR3BL)
//#define BacaPwmM2     peekw(&OCR3AL)


// Definisi Port Push Button
#define Tombol1         ((PINC&8)==0 ? 1:0)
#define Tombol2         ((PINC&4)==0 ? 1:0)
#define Tombol3         ((PINC&2)==0 ? 1:0)
#define Tombol4         ((PINC&1)==0 ? 1:0)

// Definisi LED  --> SetLEDx = menyalakan LEDx, ClrLEDx = mematikan LEDx
#define SetLED1         PORTC|=(1<<4)
#define ClrLED1         PORTC&=~(1<<4)
#define SetLED2         PORTC|=(1<<5)
#define ClrLED2         PORTC&=~(1<<5)
#define SetLED3         PORTC|=(1<<6)
#define ClrLED3         PORTC&=~(1<<6)
#define SetLED4         PORTC|=(1<<7)
#define ClrLED4         PORTC&=~(1<<7)

// menyalakan atau mematikan LED (o=1=On, o=0=Off)
#define LED1(o)         (o ? SetLED1:ClrLED1)
#define LED2(o)         (o ? SetLED2:ClrLED2)
#define LED3(o)         (o ? SetLED3:ClrLED3)
#define LED4(o)         (o ? SetLED4:ClrLED4)

// Definisi Port Motor
#define PwmMKi          OCR3B
#define SetDirMKi       PORTE|=(1<<5)
#define ClrDirMKi       PORTE&=~(1<<5)
#define DirMKi(o)       (o ? SetDirMKi:ClrDirMKi)
#define PwmMKa          OCR3A
#define SetDirMKa       PORTE|=(1<<2)
#define ClrDirMKa       PORTE&=~(1<<2)
#define DirMKa(o)       (o ? SetDirMKa:ClrDirMKa)
// definisi arah putaran motor roda
#define MKi_CCW          SetDirMKi
#define MKi_CW           ClrDirMKi
#define MKa_CW           SetDirMKa
#define MKa_CCW          ClrDirMKa

// Definisi Port Enkoder Motor
#define Set_P_Enkoder1A (PORTE|=(1<<6))                                 //PORTE.6 INT6
#define Clr_P_Enkoder1A (PORTE&=~(1<<6))
#define _P_Enkoder1A(o) (o ? Set_P_Enkoder1A:Clr_P_Enkoder1A)
#define P_Enkoder1A     ((PINE & (1<<6))==0 ?  0:1)

#define Set_P_Enkoder1B (PORTB|=(1<<0))                                 //PINB.0
#define Clr_P_Enkoder1B (PORTB&=~(1<<0))
#define _P_Enkoder1B(o) (o ? Set_P_Enkoder1B:Clr_P_Enkoder1B)
#define P_Enkoder1B     ((PINB & (1<<0))==0 ?  0:1)

#define Set_P_Enkoder2A (PORTE|=(1<<7))                                 //PORTE.7 INT7
#define Clr_P_Enkoder2A (PORTE&=~(1<<7))
#define _P_Enkoder2A(o) (o ? Set_P_Enkoder2A:Clr_P_Enkoder2A)
#define P_Enkoder2A     ((PINE & (1<<7))==0 ?  0:1)

#define Set_P_Enkoder2B (PORTB|=(1<<2))                                 //PINB.2
#define Clr_P_Enkoder2B (PORTB&=~(1<<2))
#define _P_Enkoder2B(o) (o ? Set_P_Enkoder2B:Clr_P_Enkoder2B)
#define P_Enkoder2B     ((PINB & (1<<2))==0 ?  0:1)


// Definisi Port Mode (dipswitch)
#define MODE            ((~PING) & 0B00001111)
#define MODE1           ((PING & 8) ==0 ? 1:0)
#define MODE2           ((PING & 4) ==0 ? 1:0)
#define MODE3           ((PING & 2) ==0 ? 1:0)
#define MODE4           ((PING & 1) ==0 ? 1:0)
// DipSwitchx adalah alias dari MODEx
#define DipSwitch       MODE
#define DipSwitch1      MODE1
#define DipSwitch2      MODE2
#define DipSwitch3      MODE3
#define DipSwitch4      MODE4


// Definisi Nada Buzzer (chord ke frekwensi)
#define C4      (uint16_t) 261
#define CS4     (uint16_t) 277
#define D4      (uint16_t) 293
#define DS4     (uint16_t) 311
#define E4      (uint16_t) 329
#define F4      (uint16_t) 349
#define FS4     (uint16_t) 370
#define G4      (uint16_t) 392
#define GS4     (uint16_t) 415
#define A4      (uint16_t) 440
#define AS4     (uint16_t) 466
#define B4      (uint16_t) 494

#define C5      (uint16_t) 523
#define CS5     (uint16_t) 554
#define D5      (uint16_t) 587
#define DS5     (uint16_t) 622
#define E5      (uint16_t) 659
#define F5      (uint16_t) 698
#define FS5     (uint16_t) 740
#define G5      (uint16_t) 783
#define GS5     (uint16_t) 830
#define A5      (uint16_t) 880
#define AS5     (uint16_t) 932
#define B5      (uint16_t) 987

#define C6      (uint16_t) 1046
#define CS6     (uint16_t) 1109
#define D6      (uint16_t) 1174
#define DS6     (uint16_t) 1244
#define E6      (uint16_t) 1318
#define F6      (uint16_t) 1370
#define FS6     (uint16_t) 1480
#define G6      (uint16_t) 1568
#define GS6     (uint16_t) 1661
#define A6      (uint16_t) 1760
#define AS6     (uint16_t) 1864
#define B6      (uint16_t) 1975

#define C7      (uint16_t) 2093
#define CS7     (uint16_t) 2217
#define D7      (uint16_t) 2349
#define DS7     (uint16_t) 2489
#define E7      (uint16_t) 2639
#define F7      (uint16_t) 2793
#define FS7     (uint16_t) 2960
#define G7      (uint16_t) 3136
#define GS7     (uint16_t) 3322
#define A7      (uint16_t) 3520
#define AS7     (uint16_t) 3729
#define B7      (uint16_t) 3951

// fungsi delay dengan nilai variabel
void    delay_ms(uint16_t x)
{   do{_delay_ms(1);} while(--x);
}

// union untuk mapping tipe data berdasarkan komposisi alokasi memori
// perlu diperhatikan posisi endian (big atau little endian untuk pengaturan mapping per-byte-nya)
union   Data16Bit
{   int8_t      d8s[2];     // 2 buah data signed char
    uint8_t     d8u[2];     // 4 buah data unsigned char
    int16_t     d16s;       // 1 buah data signed short integer
    uint16_t    d16u;       // 1 buah data unsigned short integer
}__attribute__ ((__packed__));

union   Data32Bit
{   int8_t      d8s[4];     // 4 buah data signed char
    uint8_t     d8u[4];     // 4 buah data unsigned char
    int16_t     d16s[2];    // 2 buah data signed short integer
    uint16_t    d16u[2];    // 2 buah data unsigned short integer
    int32_t     d32s;       // 1 buah data signed long integer
    uint32_t    d32u;       // 1 buah data unsigned long integer
    float       df;         // 1 buah data float
}__attribute__ ((__packed__));

// fungsi global
float invSqrt(float x)
    {	Data32Bit   y,z;
        /*typedef union
        {   float       f;
            uint32_t    i;
        }m;*/
        y.df = x;
        z.d32u = y.d32u;
        z.d32u = 0x5f3759df - ((z.d32u)>>1);
        z.df = z.df * (1.5f - (x * 0.5f * z.df * z.df));
        return z.df;
    }

float inverse(float x)
    { Data32Bit   y;
      /* typedef union
        {   float       f;
            uint32_t    i;
        }m;
      m y;*/
      y.df = x;
      y.d32u = 0x7EEEEEEE - y.d32u;
      return y.df;
    }

// fungsi absolute bilangan pecahan (float)
float   fabs(float x)
{   if(x<0) return -x;
    else    return x;
}

// fungsi absolute bilangan bulat 16 bit
uint16_t iabs(int16_t x)
{   if(x<0) return -x;
    else    return x;
}

// fungsi absolute bilangan bulat 8 bit
uint16_t cabs(int8_t x)
{   if(x<0) return -x;
    else    return x;
}

#endif // MYDEFINITIONS_H_INCLUDED
