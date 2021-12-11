#ifndef MYMOTOR_H_INCLUDED
#define MYMOTOR_H_INCLUDED

/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Servo and Motor Access Routines (Equipped with optional PID controller)
*********************************************************/

#include "myGlobalVars.h"

#if UseServo5 ==1
//    register uint8_t dServo5 asm("r3");
    uint8_t dServo5;
#endif
#if UseServo1 ==1
//    register uint8_t dServo1 asm("r4");
    uint8_t dServo1;
#endif
#if UseServo2 ==1
//    register uint8_t dServo2 asm("r5");
    uint8_t dServo2;
#endif
#if UseServo3 ==1
//    register uint8_t dServo3 asm("r6");
    uint8_t dServo3;
#endif
#if UseServo4 ==1
//    register uint8_t dServo4 asm("r7");
    uint8_t dServo4;
#endif

// Deklarasi obyek Motor Servo yang PWMnya di generate dengan menggunakan emulasi dari Timer 3
// variable dServo1,2,3,4 dan 5 diusahakan untuk dialokasikan ke register
// karena interupsi untuk membangkitkan PWMnya adalah 40KHz

class myServo
{   public:
        myServo (void);     // konstruktor
     #if UseServo5 ==1
        void Servo5(uint8_t posisi)  { dServo5 = HitungSudut(posisi);}
    #endif
    #if UseServo1 ==1
        void Servo1(uint8_t posisi)  { dServo1 = HitungSudut(posisi);}
    #endif
    #if UseServo2 ==1
        void Servo2(uint8_t posisi)  { dServo2 = HitungSudut(posisi);}
    #endif
    #if UseServo3 ==1
        void Servo3(uint8_t posisi)  { dServo3 = HitungSudut(posisi);}
    #endif
    #if UseServo4 ==1
        void Servo4(uint8_t posisi)  { dServo4 = HitungSudut(posisi);}
    #endif
        void Servo6(uint8_t posisi)  { dServo6 = HitungSudut2(posisi);}
        void Servo7(uint8_t posisi)  { dServo7 = HitungSudut2(posisi);}
        void Servo8(uint8_t posisi)  { dServo8 = HitungSudut2(posisi);}
    private:
        uint8_t     HitungSudut(uint8_t sudut);  // perubahan terkecil = 180/40 = 4,5 derajat
        uint16_t    HitungSudut2(uint8_t sudut); // perubahan terkecil = 180/2000 = 0,09 derajat
}servo;

myServo::myServo(void)
{   // set sudut awal masing-masing motor servo
    #if UseServo1 ==1
        Servo1(SudutAwalServo1);
    #endif // UseServo1
    #if UseServo1 ==1
        Servo2(SudutAwalServo2);
    #endif // UseServo2
    #if UseServo1 ==1
        Servo3(SudutAwalServo3);
    #endif // UseServo3
    #if UseServo4 ==1
        Servo4(SudutAwalServo4);
    #endif // UseServo4
    #if UseServo5 ==1
        Servo5(SudutAwalServo5);
    #endif // UseServo5
        Servo6(SudutAwalServo6);
        Servo7(SudutAwalServo7);
        Servo8(SudutAwalServo8);
};

uint8_t myServo::HitungSudut(uint8_t sudut)  // perubahan terkecil = 180/40 = 4,5 derajat
    {   return (((uint16_t)sudut*16)/39 + 23);}
uint16_t myServo::HitungSudut2(uint8_t sudut)
    {   return (((uint16_t)sudut*10) + 600);}

//---------------  class/obyek untuk motor DC --> Motor Roda --------------------------------------

class   myMotor
{   public:
        myMotor(void);
        void      SpeedPWM(int16_t Ki, int16_t Ka);
        void      Stop();
    #if UsePIDmotor == 1
        void      SpeedPID(int8_t dmKa, int8_t dmKi);
        void      HitungPIDmotorKi(void);
        void      HitungPIDmotorKa(void);
        int8_t    dMotorKi, dMotorKa;     // data kecepatan motor dengan PID (0 s/d 20, tanpa memperhatikan arah putaran)
    #endif // UsePIDmotor
        int8_t    dSpeedKi, dSpeedKa;               // data kecepatan motor
        uint8_t   dCounterKi, dCounterKa;           // data counter dari motor
        int16_t   EnkoderKi, EnkoderKa;
    private:
    #if UsePIDmotor == 1
    #endif // UsePIDmotor
}roda;

myMotor::myMotor(void)
{
    #if UsePIDmotor == 1
    dMotorKi=0, dMotorKa=0;
    #endif
    PwmMKi = 0;
    PwmMKa = 0;
    dSpeedKi=0;
    dSpeedKa=0;
    dCounterKi=0;
    dCounterKa=0;
}
// Set data PWM dari Motor secara langsung
// data berkisar antara -400 sampai dengan +400
void myMotor::SpeedPWM(int16_t Ki, int16_t Ka)
{
    #if UsePIDmotor == 1
        myFlag.PIDMotorOn = 0;
    #endif
    if(Ki<0)    { PwmMKi=-Ki;    MKi_CW;}
    else        { PwmMKi=Ki;     MKi_CCW;}
    if(Ka<0)    { PwmMKa=-Ka;    MKa_CCW;}
    else        { PwmMKa=Ka;     MKa_CW;}
}

void myMotor::Stop()
{
    #if UsePIDmotor == 1
        myFlag.PIDMotorOn = 1;
        dMotorKi = 0;
        dMotorKa = 0;
        PwmMKi = 0;
        PwmMKa =0;
    #else
        PwmMKi = 0;
        PwmMKa =0;
    #endif // UsePIDmotor
}

//---------------- Fungsi kendali motor saat PID motor diaktifkan -----------------------
#if UsePIDmotor == 1

#define MaxPWM          (int16_t)   400
#define KPm             (int16_t)   8
#define KDm             (int16_t)   3
#define KIm             (int16_t)   10

// Set data data kecepatan dan arah putaran Motor dengan menggunakan PID
// data motor berkisar antara -20 sampai dengan +20
void myMotor::SpeedPID(int8_t dmKi, int8_t dmKa)
{   if(dmKi<0)  { dMotorKi = -dmKi;  MKi_CW; }
    else        { dMotorKi = dmKi;   MKi_CCW;  }
    if(dmKa<0)  { dMotorKa = -dmKa;  MKa_CCW;  }
    else        { dMotorKa = dmKa;   MKa_CW; }
    myFlag.PIDMotorOn = 1;
}

void myMotor::HitungPIDmotorKa(void)
{   int8_t ErrorMKa;
    int16_t U;
    static int8_t iErrorMKa=0;//, lErrorM2=0;
      // Menghitung PID Motor 2
    if(dMotorKa>0)
      {   ErrorMKa = (int8_t)dMotorKa-(int8_t)dSpeedKa;
          U = KPm * (ErrorMKa);//+lErrorM2);
          iErrorMKa+=ErrorMKa;//+lErrorM2);
          if(iErrorMKa>40)      iErrorMKa = 40;
          else if(iErrorMKa<-40)iErrorMKa = -40;
          U+= (int16_t)iErrorMKa * KIm;
          if (U>MaxPWM)
            {   PwmMKa = MaxPWM;}
          else if (U>0)
            {   PwmMKa = U;     }
          else
            {   PwmMKa = 0;     }
          //lErrorM2 = Error2;
      }
    else {PwmMKa = 0;    iErrorMKa=0;}
}

void myMotor::HitungPIDmotorKi(void)
{ int8_t ErrorMKi;
    int16_t U;
    static int8_t  iErrorMKi=0;//,lErrorM1=0;
      // Menghitung PID Motor 1
    if (dMotorKi>0)
      {   ErrorMKi = (int8_t)dMotorKi-(int8_t)dSpeedKi;
          U = KPm * (ErrorMKi);//+Error1);
          iErrorMKi+=ErrorMKi;//+lErrorM1);
          if(iErrorMKi>40)      iErrorMKi = 40;
          else if(iErrorMKi<-40)iErrorMKi = -40;
          U+= (int16_t)iErrorMKi * KIm;
          if (U>MaxPWM)
            {   PwmMKi = MaxPWM;}
          else if (U>0)
            {   PwmMKi = U;     }
          else
            {   PwmMKi = 0;     }
          //lErrorM1  = Error1;
      }
    else  {PwmMKi = 0;     iErrorMKi=0;}
}

#endif // UsePIDmotor

#endif // MYMOTOR_H_INCLUDED
