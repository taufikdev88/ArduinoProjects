/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Declaration of Objects to Control AdRoiT V1

This program is free software: you can redistribute it and/or modify it under
the terms of the version 3 GNU General Public License as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*********************************************************/

#ifndef MYADROIT_H_INCLUDED
#define MYADROIT_H_INCLUDED


struct AdRoitFlag
{
    uint8_t FlagBelok:1;
    uint8_t InvertSensorGaris:1;
    uint8_t InvertSensorPertigaan:1;
    uint8_t FlagBalancing:1;
};

register    AdRoitFlag  af  asm("r3");

#if ModeBalancing==1
class myBalancing
{   public:
        myBalancing();
        void    Control(void);
        void    SetSpewed(int8_t Speed);   // -10 sampai dengan 10
        void    SetSteer(int8_t Steer);   // -125 sampai dengan 125
        void    Start();
        void    Stop();
    private:
        int8_t  SpeedB, SteerB;
}Balancing;
#endif // ModeBalancing


class myAdroit
{   public:
        void    MajuPWM(uint16_t MotorKiri, uint16_t MotorKanan);
        void    MundurPWM(uint16_t MotorKiri, uint16_t MotorKanan);
        void    PutarKananPWM(uint16_t MotorKiri, uint16_t MotorKanan);
        void    PutarKiriPWM(uint16_t MotorKiri, uint16_t MotorKanan);
        void    Stop(void);
#if UsePIDmotor ==1
        void    MajuPID(uint8_t MotorKiri, uint8_t MotorKanan);
        void    MundurPID(uint8_t MotorKiri, uint8_t MotorKanan);
        void    PutarKananPID(uint8_t MotorKiri, uint8_t MotorKanan);
        void    PutarKiriPID(uint8_t MotorKiri, uint8_t MotorKanan);
#endif // UsePIDmotor
};


#define PenambahanSoftStart 5  // semakin besar nilainya semakin cepat aselerasinya
#define LebarGaris          2   //2 atau 3 (dalam cm)
#define StartSpeed          80 // kecepatan scanning saat awal (rekomendasi <250, maksimum 400)
// asumsi program adalah pemasangan alamat sensor garis nol (0) adalah sensor depan, dan alamat sensor satu (1) adalah sensor belakang

class myScanGaris:public myAdroit
{   public:
        myScanGaris(void);      // konstriktor --> berisi nilai default yang di set
        void    Scan(uint16_t TopSpeed);
        void    ScanCounter(uint16_t TopSpeed, int16_t Counter);
        void    Scan3Kanan(uint16_t TopSpeed);
        void    Scan3Kiri(uint16_t TopSpeed);
        void    Scan2Kanan(uint16_t TopSpeed);
        void    Scan2Kiri(uint16_t TopSpeed);
        void    ScanSampaiSensor(uint16_t TopSpeed, uint8_t SensorData);
        void    ScanSelamaSensor(uint16_t TopSpeed, uint8_t SensorData);
        void    ScanBelokKanan(uint16_t TopSpeed, uint8_t BrakeSpeed, uint16_t Counter, uint8_t PidMotorKiri, uint8_t PidMotorKanan);
        void    ScanBelokKiri(uint16_t TopSpeed, uint8_t BrakeSpeed, uint16_t Counter, uint8_t PidMotorKiri, uint8_t PidMotorKanan);
        void    BelokKananGarisPWM(uint16_t MotorKiri, uint16_t MotorKanan);
        void    BelokKiriGarisPWM(uint16_t MotorKiri, uint16_t MotorKanan);
        void    SetArahRobot(uint8_t ArahRobot);
        void    UpdateSensorGaris(uint8_t WarnaGaris, uint8_t WarnaLantai);
        void    UpdateSensorPertigaan(uint8_t WarnaGaris, uint8_t WarnaLantai);
        void    Scan3Tengah(uint16_t TopSpeed);

        void    TraceUjungKanan(uint16_t TopSpeed);
        void    TraceUjungKiri(uint16_t TopSpeed);

#if UsePIDmotor ==1
        void    BelokKananGarisPID(uint8_t MotorKiri, uint8_t MotorKanan);
        void    BelokKiriGarisPID(uint8_t MotorKiri, uint8_t MotorKanan);
        void    BelokKananSampaiGarisPID(uint8_t MotorKiri, uint8_t MotorKanan, uint8_t DataSensor);
        void    BelokKiriSampaiGarisPID(uint8_t MotorKiri, uint8_t MotorKanan, uint8_t DataSensor);
#endif // UsePIDmotor
    private:
        uint8_t CekSensor(uint8_t WarnaGaris, uint8_t WarnaLantai, uint8_t *InvertSensor);
        void    TungguSensorKananBelok(void);
        void    TungguSensorKiriBelok(void);
        void    TungguSensorMasuk(uint8_t DataSensor);
        uint8_t ArahGerak;          // arah gerak scanning (maju atau mundur)
        uint8_t SensorGaris;        // warna sensor yang diaktifkan untuk scanning garis
        uint8_t SensorPertigaan;    // warna sensor yang diaktifkan untuk mendeteksi garis pertigaan/perempatan
        uint16_t LastSpeed;         // menyimpan kecepatan terakhir saat scanning garis
        int8_t  SensorOffSet;       // posisi sensor scanning (nilai 0 = tengah, nilai + geser ke kanan, nilai - geser ke kiri)
};

// fungsi - fungsi pada class myAdroit

void    myAdroit::MajuPWM(uint16_t MotorKiri, uint16_t MotorKanan)
{
    #if UsePIDmotor ==1
        myFlag.PIDMotorOn = 0;
    #endif // UsePIDmotor
    PwmMKi = MotorKiri;  MKi_CCW;
    PwmMKa = MotorKanan; MKa_CW;
    #if CekTeganganBatere == 1
    adc.CekBatere();        // cek tegangan batere apakah sudah di bawah threshold
    #endif // CekTeganganBatere
};

void    myAdroit::MundurPWM(uint16_t MotorKiri, uint16_t MotorKanan)
{
    #if UsePIDmotor ==1
        myFlag.PIDMotorOn = 0;
    #endif // UsePIDmotor
    PwmMKi = MotorKiri;  MKi_CW;
    PwmMKa = MotorKanan; MKa_CCW;
    #if CekTeganganBatere == 1
    adc.CekBatere();        // cek tegangan batere apakah sudah di bawah threshold
    #endif // CekTeganganBatere
};

void    myAdroit::PutarKananPWM(uint16_t MotorKiri, uint16_t MotorKanan)
{
    #if UsePIDmotor ==1
        myFlag.PIDMotorOn = 0;
    #endif // UsePIDmotor
    PwmMKi = MotorKiri;  MKi_CCW;
    PwmMKa = MotorKanan; MKa_CCW;
    #if CekTeganganBatere == 1
    adc.CekBatere();        // cek tegangan batere apakah sudah di bawah threshold
    #endif // CekTeganganBatere
};

void    myAdroit::PutarKiriPWM(uint16_t MotorKiri, uint16_t MotorKanan)
{
    #if UsePIDmotor ==1
        myFlag.PIDMotorOn = 0;
    #endif // UsePIDmotor
    PwmMKi = MotorKiri;  MKi_CW;
    PwmMKa = MotorKanan; MKa_CW;
    #if CekTeganganBatere == 1
    adc.CekBatere();        // cek tegangan batere apakah sudah di bawah threshold
    #endif // CekTeganganBatere
};

void    myAdroit::Stop(void)
{       roda.Stop();
        #if CekTeganganBatere == 1
        adc.CekBatere();        // cek tegangan batere apakah sudah di bawah threshold
        #endif // CekTeganganBatere
};

#if UsePIDmotor ==1
void    myAdroit::MajuPID(uint8_t MotorKiri, uint8_t MotorKanan)
{   roda.dMotorKi = MotorKiri;    MKi_CCW;
    roda.dMotorKa = MotorKanan;   MKa_CW;
    myFlag.PIDMotorOn = 1;
    #if CekTeganganBatere == 1
    adc.CekBatere();        // cek tegangan batere apakah sudah di bawah threshold
    #endif // CekTeganganBatere
};

void    myAdroit::MundurPID(uint8_t MotorKiri, uint8_t MotorKanan)
{   roda.dMotorKi = MotorKiri;    MKi_CW;
    roda.dMotorKa = MotorKanan;   MKa_CCW;
    myFlag.PIDMotorOn = 1;
    #if CekTeganganBatere == 1
    adc.CekBatere();        // cek tegangan batere apakah sudah di bawah threshold
    #endif // CekTeganganBatere
};

void    myAdroit::PutarKananPID(uint8_t MotorKiri, uint8_t MotorKanan)
{   roda.dMotorKi = MotorKiri;    MKi_CCW;
    roda.dMotorKa = MotorKanan;   MKa_CCW;
    myFlag.PIDMotorOn = 1;
    #if CekTeganganBatere == 1
    adc.CekBatere();        // cek tegangan batere apakah sudah di bawah threshold
    #endif // CekTeganganBatere
};

void    myAdroit::PutarKiriPID(uint8_t MotorKiri, uint8_t MotorKanan)
{   roda.dMotorKi = MotorKiri;    MKi_CW;
    roda.dMotorKa = MotorKanan;   MKa_CW;
    myFlag.PIDMotorOn = 1;
    #if CekTeganganBatere == 1
    adc.CekBatere();        // cek tegangan batere apakah sudah di bawah threshold
    #endif // CekTeganganBatere
};
#endif // UsePIDmotor


#if ModeBalancing==1
myBalancing::myBalancing()
{   SteerB = 0;
    SpeedB = 0;
};

// Start Kendali mode Balancing
void    myBalancing::Start()
{   imu.Start();
    af.FlagBalancing = 1;
};

// Stop Kendali mode Balancing
void    myBalancing::Stop()
{   af.FlagBalancing = 0;
    roda.Stop();
};

// balancing 5ms
#define     CenterOffset    10               // sudut pitch dari robot pada kondisi setimbang
#define     KpB             9.0F           // konstanta Kp dari kendali balancing robot
#define     KdB             0.02F           // konstanta Kd dari kendali balancing robot
#define     KiB             1.0F            // konstanta Ki dari kendali balancing robot
#define     sMax            5               // batas maksimal (+/-) dari integral error kecepatan robot

float Error;

void    myBalancing::Control(void)
{   float U, dError, KD=0.8;
    static float iError=0, lastError=0; //, iErrorSpeed=0;, Top=3, CenterB=0;//, LastError=0;
    int16_t dMKi, dMKa;
    if(af.FlagBalancing==1)        // balancing control akan dijalankan hanya saat FlagBalancing = 1
    {   // perhitungan PID balancing
        Error = (imu.getPitch()*RAD2DEG) + 97.4F;
        if((Error<50) && (Error >-50))
            {   //Error  = CenterB - imu.Pitch;
                dError = Error+lastError;
                iError += dError *0.4;
                lastError = Error;
                if      (iError>400)     iError = 400;
                else if (iError<-400)    iError = -400;
                KD = fabs(dError)*0.02+0.6;
                U = dError * KpB - imu.Gy*KD + iError;
                //LastError = Error;
                dMKi = (int16_t)U-SteerB;
                dMKa = (int16_t)U+SteerB;
                if      (dMKi>400)      dMKi=400;
                else if (dMKi<-400)     dMKi=-400;
                if      (dMKa>400)      dMKa=400;
                else if (dMKa<-400)     dMKa=-400;
                roda.SpeedPWM(dMKi, dMKa);
            }
        else
            {   roda.SpeedPWM(0,0);
                iError = 0;
            }
    }
};

/*
void    myBalancing::Control(void)
{   float Error, U, ErrorSpeed;
    static float iError=0, iErrorSpeed=0, Top=3, CenterB=0;//, LastErrorSpeed=0;
    static int16_t Rotary1=0, Rotary2=0;
    int16_t dMKi, dMKa;
    int8_t cSpeed;
    static uint8_t SpeedSampling = 0;
    if(af.FlagBalancing)        // balancing control akan dijalankan hanya saat FlagBalancing = 1
    {   // perhitungan PID kecepatan
        if(SpeedB!=0)
        {   if(++SpeedSampling==2)
            {   cSpeed = (int8_t)(Rotary1 - roda.EnkoderKi)+ (int8_t)(Rotary2-roda.EnkoderKa);
                ErrorSpeed = SpeedB - cSpeed;
                Rotary1 = roda.EnkoderKi;
                Rotary2 = roda.EnkoderKa;
                if (cSpeed>0) cSpeed = cSpeed;
                else          cSpeed = 0;
                iErrorSpeed +=ErrorSpeed * inverse((float)(2 + cSpeed))*0.258;
                if(iErrorSpeed>Top)         { iErrorSpeed = Top;    if (Top<15) Top+=0.023;}
                else if (iErrorSpeed<-Top)  { iErrorSpeed = -Top;   if (Top<15) Top+=0.023;}
                else
                {   if(Top>3)   Top-=0.1;
                }
                CenterB = ErrorSpeed*0.02F + iErrorSpeed;// + dErrorSpeed * 0.15F;
                SpeedSampling = 0;
            }
        }
        else
        {   CenterB = CenterOffset;
        }
        // perhitungan PID balancing
        if((imu.AccPitch<60) && (imu.AccPitch >-60))
            {   Error  = CenterB - imu.Pitch;
                iError += (Error);
                if      (iError>400)     iError = 400;
                else if (iError<-400)    iError = -400;
                U = Error * KpB - imu.GetGyroRateY() * KdB + iError;
                dMKi = (int16_t)U-SteerB;
                dMKa = (int16_t)U+SteerB;
                if      (dMKi>400)      dMKi=400;
                else if (dMKi<-400)     dMKi=-400;
                if      (dMKa>400)      dMKa=400;
                else if (dMKa<-400)     dMKa=-400;
                roda.SpeedPWM(dMKi, dMKa);
            }
        else
            {   roda.SpeedPWM(0,0);
                iError = 0;
                iErrorSpeed = 0;
                Top = 3;
            }
    }
};
*/
void    myBalancing::SetSpeed(int8_t Speed)
{   if (Speed>20)       SpeedB = 20;
    else if (Speed<-20) SpeedB = -20;
    else                SpeedB = Speed;
};

void    myBalancing::SetSteer(int8_t Steer)
{   if (Steer>125)      SteerB = 125;
    else if (Steer<-125)SteerB = -125;
    else                SteerB = Steer;
};
#endif // ModeBalancing

// fungsi - fungsi pada class myScanGaris
myScanGaris::myScanGaris()  // constructor
{   ArahGerak = GerakMaju;
    // warna defaul sensor = merah
    SensorGaris = Merah;
    SensorPertigaan = Merah;
    // backgound default putih
    af.InvertSensorGaris = 1;
    af.InvertSensorPertigaan = 1;
    af.FlagBelok = 1;
    LastSpeed = StartSpeed;
    SensorOffSet = 0;
};

// update warna sensor yang diaktifkan berdasarkan kombinasi warna lantai dan warna garis yang di-scanning
void myScanGaris::UpdateSensorGaris(uint8_t WarnaGaris, uint8_t WarnaLantai)
{   uint8_t InvertSensor=0;
    myScanGaris::SensorGaris = CekSensor(WarnaGaris , WarnaLantai, &InvertSensor);
    if  (InvertSensor)  af.InvertSensorGaris = 1;
    else                af.InvertSensorGaris = 0;
};

// update warna sensor yang diaktifkan berdasarkan kombinasi warna lantai dan warna garis pertigaan
void myScanGaris::UpdateSensorPertigaan(uint8_t WarnaGaris, uint8_t WarnaLantai)
{   uint8_t InvertSensor=0;
    myScanGaris::SensorPertigaan = CekSensor(WarnaGaris , WarnaLantai, &InvertSensor);
    if  (InvertSensor)  af.InvertSensorPertigaan = 1;
    else                af.InvertSensorPertigaan = 0;
};

// cek warna sensor yang diaktifkan berdasarkan kombinasi warna lantai dan warna garis
uint8_t myScanGaris::CekSensor(uint8_t WarnaGaris, uint8_t WarnaLantai, uint8_t *InvertSensor)
{   //uint8_t WarnaSensor = SensorMerah;
    switch  (WarnaLantai)
    {   case    Hitam:      // jika warna laintai hitam
            *InvertSensor = 0;
            if  (WarnaGaris==Merah|| WarnaGaris==Putih)
            {       return SensorMerah;     }
            else if  (WarnaGaris==Hijau)
            {       return SensorHijau;     }
            else if  (WarnaGaris==Biru)
            {       return SensorBiru;      }
            break;
        case    Putih:      // jika warna lantai Putih
            *InvertSensor = 1;
            if  (WarnaGaris==Hitam|| WarnaGaris==Biru || WarnaGaris==Hijau)
            {       return SensorMerah;     }
            else if  (WarnaGaris==Merah)
            {       return SensorBiru;      }
            break;
        case    Merah:      // jika warna lantai merah
            if  (WarnaGaris==Hitam|| WarnaGaris==Biru)
            {       *InvertSensor = 1;
                    return SensorMerah;     }
            else if  (WarnaGaris==Putih)
            {       *InvertSensor = 0;
                    return SensorBiru;      }
            else if  (WarnaGaris==Hijau)
            {       *InvertSensor = 1;
                    return SensorMerah;     }
            break;
        case    Hijau:      // jika warna lantai Hijau
            if      (WarnaGaris==Hitam)
            {       *InvertSensor = 0;
                    return SensorHijau;     }
            else if (WarnaGaris==Putih|| WarnaGaris==Merah)
            {       *InvertSensor = 0;
                    return SensorMerah;     }
            else if (WarnaGaris==Biru)
            {       *InvertSensor = 0;
                    return SensorHijau;     }
            break;
        case    Biru:      // jika warna lantai Biru
            if      (WarnaGaris==Hitam)
            {       *InvertSensor = 0;
                    return SensorBiru;      }
            else if (WarnaGaris==Putih||WarnaGaris==Merah)
            {       *InvertSensor = 0;
                    return SensorMerah;     }
            else if (WarnaGaris==Hijau)
            {       *InvertSensor = 0;
                    return SensorHijau;     }
            break;
    }
    return SensorMerah;
};

void    myScanGaris::Scan(uint16_t TopSpeed)
{   uint8_t dSensor;
    int16_t MKi, MKa, u;
    static int8_t  Error;
    int8_t dError;
    static int8_t LastError=0;//, buffer=0;
    // masuk pada program scan garis
    //lcd.GotoXY(0,0);
    //if(myScanGaris::ArahGerak==GerakMundur) TopSpeed = - TopSpeed;
            dSensor = sensor.BacaGaris(myScanGaris::ArahGerak ,myScanGaris::SensorGaris);
            if  (af.InvertSensorGaris) dSensor=~dSensor;
            lcd.GotoXY(0,0);    lcd.Biner(dSensor);
            switch (dSensor)
            {
            #if LebarGaris == 2         //cm
                //case    0B10000000: Error = -7;     break;
                case    0B11000000: Error = -6;     break;
                case    0B01000000: Error = -5;     break;
                case    0B01100000: Error = -4;     break;
                case    0B00100000: Error = -3;     break;
                case    0B00110000: Error = -2;     break;
                case    0B00010000: Error = -1;     break;
                case    0B00011000: Error = 0;     break;
                case    0B00001000: Error = 1;      break;
                case    0B00001100: Error = 2;      break;
                case    0B00000100: Error = 3;      break;
                case    0B00000110: Error = 4;      break;
                case    0B00000010: Error = 5;      break;
                case    0B00000011: Error = 6;      break;
                //case    0B00000001: Error = 7;      break;

            #elif   LebarGaris == 3     //cm
                case    0B10000000: Error = -7;     break;
                case    0B11000000: Error = -6;     break;
                case    0B11100000: Error = -5;     break;
                case    0B01100000: Error = -4;     break;
                case    0B01110000: Error = -3;     break;
                case    0B00110000: Error = -2;     break;
                case    0B00111000: Error = -1;     break;
                case    0B00011000: Error = -0;     break;
                case    0B00011100: Error = 1;      break;
                case    0B00001100: Error = 2;      break;
                case    0B00001110: Error = 3;      break;
                case    0B00000110: Error = 4;      break;
                case    0B00000111: Error = 5;      break;
                case    0B00000011: Error = 6;      break;
                case    0B00000001: Error = 7;      break;
            #endif // LebarGaris

            }
                //buffer = Error;
                Error += SensorOffSet;
                dError = Error-LastError;
                LastError = Error;
                // edit parameter kendali PD untuk scaning pada baris berikut
                u = (uint16_t)(10+(TopSpeed/30)) * (uint16_t)Error + 100 * (uint16_t)dError ;
                if(myScanGaris::ArahGerak==GerakMaju)
                {   MKi = TopSpeed + u; MKa = TopSpeed - u;
                }
                else    //(myScanGaris::ArahGerak==GerakMundur)
                {   MKi = -TopSpeed + u; MKa = -TopSpeed - u;
                }

                if (MKi>400)        MKi = 400;
                else if (MKi<-400)  MKi = -400;
                if (MKa>400)        MKa = 400;
                else if (MKa<-400)  MKa = -400;
                roda.SpeedPWM(MKi,MKa);
                //lcd.GotoXY(0,1);        lcd.Angka3(MKi);
                //lcd.GotoXY(8,1);        lcd.Angka3(MKa);
};


void    myScanGaris::TraceUjungKanan(uint16_t TopSpeed)
{   uint8_t dSensor;
    int16_t MKi, MKa, u;
    static int8_t  Error, lastSensor, counter_keluar=0;
    int8_t dError;
    static int8_t LastError=0;//, buffer=0;
    // masuk pada program scan garis
    //lcd.GotoXY(0,0);
    //if(myScanGaris::ArahGerak==GerakMundur) TopSpeed = - TopSpeed;
            dSensor = sensor.BacaGaris(myScanGaris::ArahGerak ,myScanGaris::SensorGaris);
            if  (af.InvertSensorGaris) dSensor=~dSensor;
            lcd.GotoXY(0,0);    lcd.Biner(dSensor);
            switch (dSensor)
            {
            #if LebarGaris == 2         //cm
                case    0B00000010: Error = -2;     lastSensor=Error;   counter_keluar = 0; break;
                case    0B00000011: Error = -1;     lastSensor=Error;   counter_keluar = 0; break;
                case    0B00000001: Error = 0;      lastSensor=Error;   counter_keluar = 0; break;
                case    0B00000000:
                        if(counter_keluar==0)
                        {   if(lastSensor==0)Error=3;
                            else             Error = LastError;
                        }
                        else
                        {   if(lastSensor==0)Error= 2+(float)counter_keluar/5.0;
                            else             Error = LastError-(float)counter_keluar/5.0;
                        }
                        if(counter_keluar<20)counter_keluar++;
                        break;

            #elif   LebarGaris == 3     //cm
                case    0B10000000: Error = -7;     break;
                case    0B11000000: Error = -6;     break;
                case    0B11100000: Error = -5;     break;
                case    0B01100000: Error = -4;     break;
                case    0B01110000: Error = -3;     break;
                case    0B00110000: Error = -2;     break;
                case    0B00111000: Error = -1;     break;
                case    0B00011000: Error = -0;     break;
                case    0B00011100: Error = 1;      break;
                case    0B00001100: Error = 2;      break;
                case    0B00001110: Error = 3;      break;
                case    0B00000110: Error = 4;      break;
                case    0B00000111: Error = 5;      break;
                case    0B00000011: Error = 6;      break;
                case    0B00000001: Error = 7;      break;
            #endif // LebarGaris

            }
                dError = Error-LastError;
                LastError = Error;
                // edit parameter kendali PD untuk scaning pada baris berikut
                u = (uint16_t)(10+(TopSpeed/30)) * (uint16_t)Error + 100 * (uint16_t)dError ;
                if(myScanGaris::ArahGerak==GerakMaju)
                {   MKi = TopSpeed + u; MKa = TopSpeed - u;
                }
                else    //(myScanGaris::ArahGerak==GerakMundur)
                {   MKi = -TopSpeed + u; MKa = -TopSpeed - u;
                }

                if (MKi>400)        MKi = 400;
                else if (MKi<-400)  MKi = -400;
                if (MKa>400)        MKa = 400;
                else if (MKa<-400)  MKa = -400;
                roda.SpeedPWM(MKi,MKa);
                lcd.GotoXY(0,1);        lcd.Angka3(MKi);
                lcd.GotoXY(8,1);        lcd.Angka3(MKa);
};




// Program scan garis untuk mode line tracer
// TopSpeed adalah kecepatan rata-rata saat scanning
// Counter adalah jarak yang ditempuh saat scanning (Counter berasal dari data penjumlahan Enkoder kiri dan Enkoder Kanan)
void    myScanGaris::ScanCounter(uint16_t TopSpeed, int16_t Counter)
{   roda.EnkoderKi = 0;    roda.EnkoderKa = 0;  // reset kondisi enkoder (cara ini tidak bisa digunakan apabila digunakan juga metode odometri pada saat yang sama)
    if (af.FlagBelok)   LastSpeed = StartSpeed;
    if(ArahGerak == GerakMaju)
    {   while((roda.EnkoderKi + roda.EnkoderKa)<Counter)
        {   if((LastSpeed<TopSpeed))LastSpeed+=PenambahanSoftStart;
            myScanGaris::Scan(LastSpeed);
        }
    }
    else
    {   while((roda.EnkoderKi + roda.EnkoderKa)>-Counter)
        {   if((LastSpeed<TopSpeed))LastSpeed+=PenambahanSoftStart;
            myScanGaris::Scan(LastSpeed);
        }
    }
    af.FlagBelok=0;
};

void    myScanGaris::ScanBelokKanan(uint16_t TopSpeed, uint8_t BrakeSpeed, uint16_t Counter, uint8_t PidMotorKiri, uint8_t PidMotorKanan)
{   SensorOffSet = 1;
    myScanGaris::ScanCounter(TopSpeed, Counter);
    myScanGaris::Scan3Kanan(BrakeSpeed);
    #if UsePIDmotor ==1
        myScanGaris::BelokKananGarisPID(PidMotorKiri, PidMotorKanan);
    #else
        myScanGaris::BelokKananGarisPWM(PidMotorKiri*16, PidMotorKanan*16);
    #endif // UsePIDmotor
    SensorOffSet = 0;
};

void    myScanGaris::ScanBelokKiri(uint16_t TopSpeed, uint8_t BrakeSpeed, uint16_t Counter, uint8_t PidMotorKiri, uint8_t PidMotorKanan)
{   SensorOffSet = -1;
    myScanGaris::ScanCounter(TopSpeed, Counter);
    myScanGaris::Scan3Kiri(BrakeSpeed);
    #if UsePIDmotor ==1
        myScanGaris::BelokKiriGarisPID(PidMotorKiri, PidMotorKanan);
    #else
        myScanGaris::BelokKiriGarisPWM(PidMotorKiri*16, PidMotorKanan*16);
    #endif // UsePIDmotor
    SensorOffSet = 0;
};

// melakukan proses scanniing garis sampai kondisi sensor = SensorData
void    myScanGaris::ScanSampaiSensor(uint16_t TopSpeed, uint8_t SensorData)
{   uint8_t dSensor;
    if (af.FlagBelok)   LastSpeed = StartSpeed;
    else if(LastSpeed>TopSpeed) LastSpeed = TopSpeed;
    do{  if((LastSpeed<TopSpeed))LastSpeed+=PenambahanSoftStart;
         myScanGaris::Scan(LastSpeed);
         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
    }while((dSensor&SensorData)!=SensorData);   // menungu sampai kondisi sensor belum terpenuhi
}

// melakukan proses scanning garis selama kondisi sensor = SensorData
void    myScanGaris::ScanSelamaSensor(uint16_t TopSpeed, uint8_t SensorData)
{   uint8_t dSensor;
    do{  myScanGaris::Scan(TopSpeed);
         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
    }while((dSensor&SensorData)==SensorData);   // menungu sampai kondisi sensor masih terpenuhi
}

void    myScanGaris::Scan3Kanan(uint16_t TopSpeed)
{   //uint8_t dSensor, toCounter=10;
    ScanSampaiSensor(TopSpeed, 0B00000111);
    ScanSelamaSensor(150, 0B00000111);

    //static uint16_t LastSpeed=100;
//    if (af.FlagBelok)   LastSpeed = StartSpeed;
//    else if(LastSpeed>TopSpeed) LastSpeed = TopSpeed;
//    do{  if((LastSpeed<TopSpeed))LastSpeed+=PenambahanSoftStart;
//         myScanGaris::Scan(LastSpeed);
//         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
//         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
//         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
//    }while((dSensor&0B00000111)!=0B00000111);    // menungu sampai tiga sensor kanan aktif   (di atas per3an)
//    do{  myScanGaris::Scan(100);                 // kecepatan sambil ngerem
//         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
//         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
//         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
//    }while((dSensor&0B00000111)==0B00000111 && toCounter--);    // menungu sampai ketiga sensor kanan tidak aktif (melewati per3an)
//    LastSpeed=TopSpeed;
    myAdroit::Stop();
    //roda.Stop();
    af.FlagBelok=0;
};

void    myScanGaris::Scan3Kiri(uint16_t TopSpeed)
{   //uint8_t dSensor, toCounter=10;
    ScanSampaiSensor(TopSpeed, 0B11100000);
    ScanSelamaSensor(150, 0B11100000);

    //static uint16_t LastSpeed=100;
//    if (af.FlagBelok)   LastSpeed = StartSpeed;
//    else if(LastSpeed>TopSpeed) LastSpeed = TopSpeed;
//    do{  if((LastSpeed<TopSpeed))LastSpeed+=PenambahanSoftStart;
//         myScanGaris::Scan(LastSpeed);
//         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
//         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
//         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
//    }while((dSensor&0B11100000)!=0B11100000);    // menungu sampai tiga sensor kanan aktif   (di atas per3an)
//    do{  myScanGaris::Scan(100);                 // kecepatan sambil ngerem
//         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
//         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
//    }while((dSensor&0B11100000)==0B11100000 && toCounter--);    // menungu sampai ketiga sensor kanan tidak aktif (melewati per3an)
    myAdroit::Stop();
    af.FlagBelok=0;
};


void    myScanGaris::Scan2Kanan(uint16_t TopSpeed)
{   //uint8_t dSensor, toCounter=10;
    ScanSampaiSensor(TopSpeed, 0B00000011);
    ScanSelamaSensor(150, 0B00000011);
    //static uint16_t LastSpeed=100;
//    if (af.FlagBelok)   LastSpeed = StartSpeed;
//    else if(LastSpeed>TopSpeed) LastSpeed = TopSpeed;
//    do{  if((LastSpeed<TopSpeed))LastSpeed+=PenambahanSoftStart;
//         myScanGaris::Scan(LastSpeed);
//         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
//         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
//         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
//    }while((dSensor&0B00000011)!=0B00000011);    // menungu sampai tiga sensor kanan aktif   (di atas per3an)
//    do{  myScanGaris::Scan(100);                 // kecepatan sambil ngerem
//         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
//         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
//         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
//    }while((dSensor&0B00000011)==0B00000011 && toCounter--);    // menungu sampai ketiga sensor kanan tidak aktif (melewati per3an)
//    //LastSpeed=TopSpeed;
    myAdroit::Stop();
    //roda.Stop();
    af.FlagBelok=0;
};

void    myScanGaris::Scan2Kiri(uint16_t TopSpeed)
{   //uint8_t dSensor, toCounter=10;
    ScanSampaiSensor(TopSpeed, 0B11000000);
    ScanSelamaSensor(150, 0B11000000);
    //static uint16_t LastSpeed=100;
//    if (af.FlagBelok)   LastSpeed = StartSpeed;
//    else if(LastSpeed>TopSpeed) LastSpeed = TopSpeed;
//    do{  if((LastSpeed<TopSpeed))LastSpeed+=PenambahanSoftStart;
//         myScanGaris::Scan(LastSpeed);
//         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
//         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
//         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
//    }while((dSensor&0B11000000)!=0B11000000);    // menungu sampai tiga sensor kanan aktif   (di atas per3an)
//    do{  myScanGaris::Scan(100);                 // kecepatan sambil ngerem
//         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
//         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
//         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
//    }while((dSensor&0B11000000)==0B11000000 && toCounter--);    // menungu sampai ketiga sensor kanan tidak aktif (melewati per3an)
    myAdroit::Stop();
    //roda.Stop();
    af.FlagBelok=0;
};


void    myScanGaris::Scan3Tengah(uint16_t TopSpeed)
{   uint8_t dSensor;
    //static uint16_t LastSpeed=100;
    if (af.FlagBelok)   LastSpeed = StartSpeed;
    else if(LastSpeed>TopSpeed) LastSpeed = TopSpeed;
    do{  if((LastSpeed<TopSpeed))LastSpeed+=PenambahanSoftStart;
         myScanGaris::Scan(LastSpeed);
         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
    }while(((dSensor&0B00111000)!=0B00111000) && ((dSensor&0B00011100)!=0B00011100) );    // menungu sampai tiga sensor tengah aktif (percabangan Y)
    do{  myScanGaris::Scan(100);                 // kecepatan sambil ngerem
         dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
         if  (af.InvertSensorPertigaan)dSensor=~dSensor;
         //lcd.GotoXY(0,1);lcd.Biner(dSensor);
    }while(((dSensor&0B00111000)==0B00111000) || ((dSensor&0B00011100)==0B00011100) );    // menungu sampai tiga sensor tengah aktif (percabangan Y)
    myAdroit::Stop();
    //roda.Stop();
    af.FlagBelok=0;
};

void    myScanGaris::TungguSensorMasuk(uint8_t DataSensor)
{   uint8_t dSensor, toCounter=3;
    do{
        do{ dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
            if  (af.InvertSensorPertigaan)dSensor=~dSensor;
        } while((dSensor&DataSensor)==0);
      } while(toCounter--);
    myAdroit::Stop();
    af.FlagBelok=1;
};

void    myScanGaris::TungguSensorKananBelok()
{   uint8_t dSensor, toCounter=10;
    // menunggu sensor kanan ke 0 s/d ke 2 keluar garis
    _delay_ms(10);
    do{ dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
        if  (af.InvertSensorPertigaan)dSensor=~dSensor;
        //lcd.GotoXY(0,1);          lcd.Biner(dSensor);
      } while((dSensor&0B00000011)!=0 && toCounter--);
    // menunggu sensor kanan ke 0 s/d ke 2 masuk garis
    myScanGaris::TungguSensorMasuk(0B00000011);
    //do{ dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
    //    if  (af.InvertSensorPertigaan)dSensor=~dSensor;
    //     //lcd.GotoXY(0,1);          lcd.Biner(dSensor);
    //  } while((dSensor&0B00000011)==0);
    //myAdroit::Stop();//roda.Stop();
    //af.FlagBelok=1;
};

void    myScanGaris::TungguSensorKiriBelok()
{   uint8_t dSensor, toCounter=10;
    // menunggu sensor kiri ke 7 s/d ke 5 keluar garis
    _delay_ms(10);
    do{ dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
        if  (af.InvertSensorPertigaan)dSensor=~dSensor;
         //lcd.GotoXY(0,1);      lcd.Biner(dSensor);
      } while((dSensor&0B11000000)!=0 && toCounter--);
    myScanGaris::TungguSensorMasuk(0B11000000);
    // menunggu sensor kiri ke 7 s/d ke 5 masuk garis
    //do{ dSensor = sensor.BacaGaris(myScanGaris::ArahGerak, myScanGaris::SensorPertigaan);
    //    if  (af.InvertSensorPertigaan)dSensor=~dSensor;
    //     //lcd.GotoXY(0,1);      lcd.Biner(dSensor);
    //  } while((dSensor&0B11000000)==0);
    //myAdroit::Stop();
    //af.FlagBelok=1;
};

void    myScanGaris::BelokKananGarisPWM(uint16_t MotorKiri, uint16_t MotorKanan)
{   if(myScanGaris::ArahGerak==GerakMaju)   myAdroit::PutarKananPWM(MotorKiri,MotorKanan);  // asumsi mengunakan gerakan putar (kiri maju, kanan mundur)
    else                                    myAdroit::PutarKananPWM(MotorKanan,MotorKiri);  // data kecepatan dibalik
    myScanGaris::TungguSensorKananBelok();
};

void    myScanGaris::BelokKiriGarisPWM(uint16_t MotorKiri, uint16_t MotorKanan)
{   if(myScanGaris::ArahGerak==GerakMaju)   myAdroit::PutarKiriPWM(MotorKiri,MotorKanan);   // asumsi mengunakan gerakan putar (kiri mundur, kanan maju)
    else                                    myAdroit::PutarKiriPWM(MotorKanan,MotorKiri);  // data kecepatan dibalik
    myScanGaris::TungguSensorKiriBelok();
};

void    myScanGaris::SetArahRobot(uint8_t ArahRobot)
{       myScanGaris::ArahGerak = ArahRobot;
};

#if UsePIDmotor ==1
void    myScanGaris::BelokKananGarisPID(uint8_t MotorKiri, uint8_t MotorKanan)
{   if(myScanGaris::ArahGerak==GerakMaju)   myAdroit::PutarKananPID(MotorKiri,MotorKanan);  // asumsi mengunakan gerakan putar (kiri maju, kanan mundur)
    else                                    myAdroit::PutarKananPID(MotorKanan,MotorKiri);  // data kecepatan dibalik
    myScanGaris::TungguSensorKananBelok();
};

void    myScanGaris::BelokKiriGarisPID(uint8_t MotorKiri, uint8_t MotorKanan)
{   if(myScanGaris::ArahGerak==GerakMaju)   myAdroit::PutarKiriPID(MotorKiri,MotorKanan);   // asumsi mengunakan gerakan putar (kiri mundur, kanan maju)
    else                                    myAdroit::PutarKiriPID(MotorKanan,MotorKiri);  // data kecepatan dibalik
    myScanGaris::TungguSensorKiriBelok();
};

void    myScanGaris::BelokKananSampaiGarisPID(uint8_t MotorKiri, uint8_t MotorKanan, uint8_t DataSensor)
{   if(myScanGaris::ArahGerak==GerakMaju)   myAdroit::PutarKananPID(MotorKiri,MotorKanan);  // asumsi mengunakan gerakan putar (kiri maju, kanan mundur)
    else                                    myAdroit::PutarKananPID(MotorKanan,MotorKiri);  // data kecepatan dibalik
    myScanGaris::TungguSensorMasuk(DataSensor);
};

void    myScanGaris::BelokKiriSampaiGarisPID(uint8_t MotorKiri, uint8_t MotorKanan, uint8_t DataSensor)
{   if(myScanGaris::ArahGerak==GerakMaju)   myAdroit::PutarKiriPID(MotorKiri,MotorKanan);   // asumsi mengunakan gerakan putar (kiri mundur, kanan maju)
    else                                    myAdroit::PutarKiriPID(MotorKanan,MotorKiri);  // data kecepatan dibalik
    myScanGaris::TungguSensorMasuk(DataSensor);
};


#endif // UsePIDmotor

#endif // MYADROIT_H_INCLUDED
