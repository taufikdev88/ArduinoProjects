/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : - Routines to access MPU6050 (3axis Accelerometer + Gyroscope)
          - Filter Routines to calculate IMU data

This program is free software: you can redistribute it and/or modify it under
the terms of the version 3 GNU General Public License as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*********************************************************/
#ifndef myMPU6050_H_INCLUDED
#define myMPU6050_H_INCLUDED

// Register dari MPU6050
#include "myMPU6050reg.h"

    //------------------------------- Fungsi dari IMU -----------------------------------------------------------------------

    //definitions
#define MPU6050_ADDR (0x68 <<1)

    //Skala pilihan dari gyro and acc
#define MPU6050_GYRO_FS         MPU6050_GYRO_FS_1000    // seting skala 2000 derajat/detik
#define MPU6050_ACCEL_FS        MPU6050_ACCEL_FS_4 // seting skala percepatan 4 G

#if MPU6050_GYRO_FS == MPU6050_GYRO_FS_250
      #define MPU6050_GGAIN MPU6050_GYRO_LSB_250
#elif MPU6050_GYRO_FS == MPU6050_GYRO_FS_500
      #define MPU6050_GGAIN MPU6050_GYRO_LSB_500
#elif MPU6050_GYRO_FS == MPU6050_GYRO_FS_1000
      #define MPU6050_GGAIN MPU6050_GYRO_LSB_1000
#elif MPU6050_GYRO_FS == MPU6050_GYRO_FS_2000
      #define MPU6050_GGAIN MPU6050_GYRO_LSB_2000
#endif

    // Pilihan Skala dari Accelerometer
#define MPU6050_ACCEL_LSB_2     16384.0                 // maksimal 2 g (1 g = 9.8 m/detik^2)#define MPU6050_ACCEL_LSB_4     8192.0                  // maksimal 4 g
#define MPU6050_ACCEL_LSB_8     4096.0                  // maksimal 8 g
#define MPU6050_ACCEL_LSB_16    2048.0                  // maksimal 16 g

#if MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_2
    #define MPU6050_AGAIN MPU6050_ACCEL_LSB_2
#elif MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_4
    #define MPU6050_AGAIN MPU6050_ACCEL_LSB_4
#elif MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_8
    #define MPU6050_AGAIN MPU6050_ACCEL_LSB_8
#elif MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_16
    #define MPU6050_AGAIN MPU6050_ACCEL_LSB_16
#endif

    //functions
float invSqrt(float x);

class   myMPU6050
{   public:
        uint8_t Init();
        void    Start();
        void    Stop();
        void    KalibrasiGyroScope(void);
        void    BacaAccelerometer(void);
        void    BacaGyroScope(void);
        float   GetGyroRateX();
        float   GetGyroRateY();
        float   GetGyroRateZ();
        void    BacaGyroX(void);
        void    BacaGyroY(void);
        void    BacaGyroZ(void);
#if ImuFilter == KalmanFilter
        void    HitungKalmanRoll(void);
        void    HitungKalmanPitch(void);
        void    HitungYaw();
#elif ImuFilter == ComplementaryFilter
        void    HitungComplementaryRoll(void);
        void    HitungComplementaryPitch(void);
        void    HitungYaw();
#elif ImuFilter == MahonyFilter
        void    HitungMahonyFilter(void);
#endif // ImuFilter
        float   Roll, Pitch, Yaw, AccRoll, AccPitch;
    private:
        void    SleepDisabled();
        void    SleepEnabled();
        uint8_t TestConnection();
        void    StartBacaRegister(uint8_t Register);
        float   GxOffset, GyOffset, GzOffset;
        int16_t Ax, Ay, Az, Gx, Gy, Gz;
}imu;

    void    myMPU6050::Start()
    {   myFlag.ImuStart = 1;
    }

    void    myMPU6050::Stop()
    {   myFlag.ImuStart = 0;
    }

    // mode sleep di non aktifkan
    void myMPU6050::SleepDisabled() {
        i2c.WriteBit(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 0);
    }

    // mode sleep diaktifkan
    void myMPU6050::SleepEnabled() {
        i2c.WriteBit(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 1);
    }

    // Memeriksa koneksi ke MPU6050
    uint8_t myMPU6050::TestConnection() {
        uint8_t buffer;
        i2c.ReadBits(MPU6050_ADDR, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, &buffer);
        if(buffer == 0x34)   return 1;
        else                 return 0;
    }

    // Inisialisasi MPU6050
    uint8_t myMPU6050::Init(void)
    {   //menunggu start-up dari mpu6050
        _delay_ms(50);
        //mode sleep di non-aktifkan
        SleepDisabled();
        //wake up delay
        _delay_ms(10);
        if(TestConnection())
        {
        // pengaturan sumber clock dari MPU6050
        // direkomendasikan menggunakan salah satu dari gyroscopes (atau mengunakan sumber clock eksternal)
            i2c.WriteBits(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, MPU6050_CLOCK_PLL_XGYRO);
        //mengeset bandwidth dari  DLPF pada frekwensi 42Hz
            i2c.WriteBits(MPU6050_ADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, MPU6050_DLPF_BW_42);
        // pengaturan frekwensi sampling internal
            i2c.WriteByte(MPU6050_ADDR, MPU6050_RA_SMPLRT_DIV, 9); //1khz / (1 + 4) = 200Hz
        //pengaturan range dari gyroscope
            i2c.WriteBits(MPU6050_ADDR, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, MPU6050_GYRO_FS);
        //pengaturan range dari accelerometer
            i2c.WriteBits(MPU6050_ADDR, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, MPU6050_ACCEL_FS);

        #if UseCompass == 1
        // mem-bypass I2c bus slave untuk koneksi pada sensor magnetometer (HMC5883L)
        // menonaktifkan mode I2C master dari MPU6050
            i2c.WriteBit(MPU6050_ADDR, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, 0);
        // mengaktifkan mode I2C slave bypass dari MPU6050
            i2c.WriteBit(MPU6050_ADDR, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, 1);
        #endif
            return 1;
        }
        else
        {   return 0;
        }
    }

    void myMPU6050::StartBacaRegister(uint8_t Register)
    {   i2c.Start(MPU6050_ADDR | I2C_WRITE);
        i2c.Write(Register);
        _delay_us(5);
        i2c.Start(MPU6050_ADDR | I2C_READ);
    }

    // Membaca data Accelerometer 3 Axis
    void myMPU6050::BacaAccelerometer(void)
    {   Data16Bit Acc;
        StartBacaRegister(MPU6050_RA_ACCEL_XOUT_H);
        Acc.d8u[1] = i2c.ReadAck();  // AVR = little-endian, dan MPU6050 = big-endian
        Acc.d8u[0] = i2c.ReadAck();
        Ax = Acc.d16s;
        Acc.d8u[1] = i2c.ReadAck();
        Acc.d8u[0] = i2c.ReadAck();
        Ay = Acc.d16s;
        Acc.d8u[1] = i2c.ReadAck();
        Acc.d8u[0] = i2c.ReadNak();
        Az = Acc.d16s;
        i2c.Stop();
    }

    // Membaca data Gyroscope 3 Axis
    void myMPU6050::BacaGyroScope(void)
    {   Data16Bit   Gyro;
        StartBacaRegister(MPU6050_RA_GYRO_XOUT_H);
        Gyro.d8u[1] = i2c.ReadAck();  // AVR = little-endian, dan MPU6050 = big-endian
        Gyro.d8u[0] = i2c.ReadAck();
        Gx = Gyro.d16s;
        Gyro.d8u[1] = i2c.ReadAck();
        Gyro.d8u[0] = i2c.ReadAck();
        Gy = Gyro.d16s;
        Gyro.d8u[1] = i2c.ReadAck();
        Gyro.d8u[0] = i2c.ReadNak();
        Gz = Gyro.d16s;
        i2c.Stop();
    }

    void myMPU6050::BacaGyroX(void)
    {   Data16Bit   Gyro;
        StartBacaRegister(MPU6050_RA_GYRO_XOUT_H);
        Gyro.d8u[1] = i2c.ReadAck();  // AVR = little-endian, dan MPU6050 = big-endian
        Gyro.d8u[0] = i2c.ReadNak();
        Gx = Gyro.d16s;
        i2c.Stop();
    }

    void myMPU6050::BacaGyroY(void)
    {   Data16Bit   Gyro;
        StartBacaRegister(MPU6050_RA_GYRO_YOUT_H);
        Gyro.d8u[1] = i2c.ReadAck();  // AVR = little-endian, dan MPU6050 = big-endian
        Gyro.d8u[0] = i2c.ReadNak();
        Gy = Gyro.d16s;
        i2c.Stop();
    }

    void myMPU6050::BacaGyroZ(void)
    {   Data16Bit   Gyro;
        StartBacaRegister(MPU6050_RA_GYRO_ZOUT_H);
        Gyro.d8u[1] = i2c.ReadAck();  // AVR = little-endian, dan MPU6050 = big-endian
        Gyro.d8u[0] = i2c.ReadNak();
        Gz = Gyro.d16s;
        i2c.Stop();
    }

    void myMPU6050::KalibrasiGyroScope(void)
    {   uint8_t i=100;
        GxOffset = 0;           GyOffset = 0;       GzOffset = 0;
        do
        {   BacaGyroScope();
            GxOffset+=Gx;       GyOffset+=Gy;       GzOffset+=Gz;
            _delay_ms(1);
        }while(--i);
        GxOffset*=0.01;         GyOffset*=0.01;     GzOffset*=0.01;     // nilai 10000 x nilai asli
    }

#if ImuFilter == KalmanFilter

    //  axx_sudut_x =(float) 57.295*atan((float)axx_y/ sqrt(pow((float)axx_z,2)+pow((float)axx_x,2)));
    //  axx_sudut_y =(float) 57.295*atan((float)-axx_x/ sqrt(pow((float)axx_z,2)+pow((float)axx_y,2)));
    //  axx_sudut_z =(float) 57.295*atan(sqrt(pow((float)axx_x,2)+pow((float)axx_y,2))/(float)axx_z);

    // Kalman filter variables
    // Q_acclero    = Process noise variance for the accelerometer
    // Q_gyro;      = Process noise variance for the gyro bias
    // R_measure    = Measurement noise variance - this is actually the variance of the measurement noise

    // angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
    // bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
    // rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate
    // S; // Estimate error
    // P[2][2]; // Error covariance matrix - This is a 2x2 matrix
    // K[2]; // Kalman gain - This is a 2x1 vector
    // y; // Angle difference

#define     Q_accelero      0.001f
#define     Q_gyro          0.002f
#define     R_measure       0.03f
 //definitions for attitude 1 function estimation
#if ModeBalancing == 1
        #define WaktuSampling       0.004f // periode frekuensi dalam detik
#else
        #define WaktuSampling       0.008f // periode frekuensi dalam detik
#endif  //ModeBalancing
    void myMPU6050::HitungKalmanRoll(void)
    {   float   buffer;
        static float P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0;
        float K_0, K_1;

        buffer = WaktuSampling * P_11;
        P_00 +=  WaktuSampling * (buffer - P_10 - P_01 + Q_accelero);
        P_01 -=  buffer;
        P_10 -=  buffer;
        P_11 +=  WaktuSampling * Q_gyro;

    #if ImuHighAccuration == 0
            buffer = inverse(P_00 + R_measure);                     // lebih cepat
    #else
            buffer = 1.0f/(P_00 + R_measure);      // buffer = S    // lebih akurat
    #endif // ImuHighAccuration
        K_0 = P_00 * buffer;
        K_1 = P_10 * buffer;

    #if PosisiBoard == TIDUR
        // Roll ClockWise = positif, CounterClockWise = negatif
        Roll +=((float)Gx-GxOffset)*(WaktuSampling/MPU6050_GGAIN);  //0.000609756097561f;    //WaktuSampling/GXGAIN
            if(Az<=0)
            {   AccRoll = atan((float)-Ay * invSqrt((float)((uint32_t)Az*(uint32_t)Az + (uint32_t)Ax*(uint32_t)Ax)))*RAD2DEG;}
            else
            {   //AccRoll  = atan2((float)-Ay, (float)-Az) * RAD2DEG;
                if(Ay>=0)   {   AccRoll = -90.0f - atan((float)Az * invSqrt((float)((uint32_t)Ay*(uint32_t)Ay + (uint32_t)Ax*(uint32_t)Ax)))*RAD2DEG;   }
                else        {   AccRoll = 90.0f + atan((float)Az * invSqrt((float)((uint32_t)Ay*(uint32_t)Ay + (uint32_t)Ax*(uint32_t)Ax)))*RAD2DEG;    }
            }
    #else                   //TEGAK
        Roll +=((float)Gz-GzOffset)*(WaktuSampling/MPU6050_GGAIN);  //0.000609756097561f;    //WaktuSampling/GXGAIN
            if(Ax>=0)
            {   AccRoll = atan((float)-Ay * invSqrt((float)((uint32_t)Az*(uint32_t)Az + (uint32_t)Ax*(uint32_t)Ax)))*RAD2DEG;   }

            else
            {   if(Ay>=0)
                {   AccRoll = atan((float)Ax * invSqrt((float)((uint32_t)Az*(uint32_t)Az + (uint32_t)Ay*(uint32_t)Ay)))*RAD2DEG - 90.0f;   }
                else
                {   AccRoll = 90.0f - atan((float)Ax * invSqrt((float)((uint32_t)Az*(uint32_t)Az + (uint32_t)Ay*(uint32_t)Ay)))*RAD2DEG;   }
            }
    #endif  // PosisiBoard
        buffer = AccRoll - Roll;     // buffer = y
        Roll +=  K_0 * buffer;
        GxOffset +=  K_1 * buffer;
        P_00 -= K_0 * P_00;
        P_01 -= K_0 * P_01;
        P_10 -= K_1 * P_00;
        P_11 -= K_1 * P_01;
    }

    // Perhitungan sudut Pitch (Perputaran dgn poros sumbu X) dengan menggunakan Kalman Filter
    void myMPU6050::HitungKalmanPitch(void)
    {   float   buffer;
        static float P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0;
        float K_0, K_1;

        buffer = WaktuSampling * P_11;
        P_00 +=  WaktuSampling * (buffer - P_10 - P_01 + Q_accelero);
        P_01 -=  buffer;
        P_10 -=  buffer;
        P_11 +=  (WaktuSampling * Q_gyro);

    #if ImuHighAccuration == 0
            buffer = inverse(P_00 + R_measure);                     // lebih cepat
    #else
            buffer = 1.0f/(P_00 + R_measure);      // buffer = S    // lebih akurat
    #endif // ImuHighAccuration
        K_0 = P_00 * buffer;
        K_1 = P_10 * buffer;

    #if PosisiBoard==0  // TIDUR
            Pitch +=((float)Gy-GyOffset)*(float)(WaktuSampling/MPU6050_GGAIN);  //0.000609756097561f;
            if(Az<=0)
            {   AccPitch = atan((float)Ax * invSqrt(((uint32_t)Az*(uint32_t)Az + (uint32_t)Ay*(uint32_t)Ay)))*RAD2DEG;}
            else
            {   //AccPitch = atan2((float)Ax, (float)-Az) * RAD2DEG;
                if(Ax>=0){   AccPitch = 90.0f + atan((float)Az * invSqrt(((uint32_t)Ax*(uint32_t)Ax + (uint32_t)Ay*(uint32_t)Ay)))*RAD2DEG;    }
                else        {   AccPitch = -90.0f - atan((float)Az * invSqrt(((uint32_t)Ax*(uint32_t)Ax + (uint32_t)Ay*(uint32_t)Ay)))*RAD2DEG;   }
            }
    #else               // TEGAK
            Pitch +=((float)Gy-GyOffset)*(float)(WaktuSampling/MPU6050_GGAIN);  //0.000609756097561f;
            if(Ax>0)
            {   AccPitch = atan((float)Az * invSqrt((float)((uint32_t)Ax*(uint32_t)Ax + (uint32_t)Ay*(uint32_t)Ay)))*RAD2DEG;}
            else
            {   if(Az>0){   AccPitch = 90.0f - atan((float)Ax * invSqrt((float)((uint32_t)Az*(uint32_t)Az + (uint32_t)Ay*(uint32_t)Ay)))*RAD2DEG;   }
                else    {   AccPitch = atan((float)Ax * invSqrt((float)((uint32_t)Az*(uint32_t)Az + (uint32_t)Ay*(uint32_t)Ay)))*RAD2DEG - 90.0f;   }
            }
    #endif  //PosisiBoard

        buffer = AccPitch - Pitch;     // buffer = y
        Pitch +=  K_0 * buffer;
        //Pitch = AccPitch;
        GyOffset +=  K_1 * buffer;
        P_00 -= K_0 * P_00;
        P_01 -= K_0 * P_01;
        P_10 -= K_1 * P_00;
        P_11 -= K_1 * P_01;
    }
    void myMPU6050::HitungYaw(void)
    {
        #if PosisiBoard==0  // TIDUR
            Yaw +=((float)Gz-GzOffset)*(WaktuSampling/MPU6050_GGAIN);
        #else
            Yaw +=((float)Gx-GxOffset)*(WaktuSampling/MPU6050_GGAIN);
        #endif
    }
#elif ImuFilter == ComplementaryFilter


    void myMPU6050::HitungComplementaryRoll(void)
    {
    }

    void myMPU6050::HitungComplementaryPitch(void)
    {
    }

    void myMPU6050::HitungYaw(void)
    {
        #if PosisiBoard == TIDUR
            Yaw +=((float)Gz-GzOffset)*(float)(WaktuSampling/MPU6050_GGAIN);
        #elif PosisiBoard == TEGAK
            Yaw +=((float)Gx-GxOffset)*(float)(WaktuSampling/MPU6050_GGAIN);
        #endif
    }
#elif ImuFilter == MahonyFilter
    void myMPU6050::HitungMahonyFilter(void)
    {
    }
#endif

float myMPU6050::GetGyroRateX()
{   return ((float)Gx-GxOffset);
};

float myMPU6050::GetGyroRateY()
{   return ((float)Gy-GyOffset);
};

float myMPU6050::GetGyroRateZ()
{   return ((float)Gz-GzOffset);
};


#endif // myMPU6050_H_INCLUDED
