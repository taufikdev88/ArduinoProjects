#ifndef MYMODULE_H_INCLUDED
#define MYMODULE_H_INCLUDED

/***************************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Pengaturan aktif tidaknya modul serta mode kerja modul
*****************************************************************/

// Pengaktifan fitur pada board kontroler, isi 1 jika diaktifkan, isi 0 jika tidak
    #define ModeBalancing               1           // 1, jika mode balancing robot diaktifkan
// Pengaktifan fitur lagu (buzzer) pada board kontroler, isi 1 jika diaktifkan, isi 0 jika tidak
    #define UseMusic                    0           // 1, jika mode balancing robot diaktifkan

    // motor servo yang digenerate menggunakan emulasi PWM dari timer 3
    #define UseServo1                   0           // Mengaktifkan Servo 1 ?
    #define UseServo2                   0           // Mengaktifkan Servo 2 ?
    #define UseServo3                   0           // Mengaktifkan Servo 3 ?
    #define UseServo4                   0           // Mengaktifkan Servo 4 ?
    #define UseServo5                   0           // Mengaktifkan Servo 5 ?
    // servo 6, 7 dan 8 secara default aktif dengan PWM hardware dari board kontroler (OCR3x)

    #if ModeBalancing == 1
        #define UseIMU                  1           // mode ini membutuhkan IMU untuk diaktifkan
        #define UsePIDmotor             0           // fitur PID untuk kendalai kecepatan roda untuk mode ini dinonaktifkan
    #else
        #define UseIMU                  1           // apakah mengaktifkan fitur IMU?
        #define UsePIDmotor             0           // fitur PID untuk kendalai kecepatan roda untuk mode ini dinonaktifkan
    #endif

    #if UseIMU == 1
        #define ImuHighAccuration       0           // jika 1 maka proses inverse akan dilakukan dengan perhitungan (lebih lambat)
                                                    // jika 0 maka proses inverse dilakukan dengan aproximasi (lebih cepat)
        #define UseCompass              0           // Kompas apakah diaktifkan atau tidak
        // Pilih salah satu jenis filter yang diaktifkan
        #define ComplementaryFilter     1           // filter paling sederhana
        #define KalmanFilter            2           // implementasi masih terbatas pada sudut roll dan pitch
        #define DMP                     3           // memanfaatkan filter internal dari MPU6050 (lebih lengkap)
        #define ImuFilter               DMP
        #if ImuFilter==DMP
            #define UseGyroFiFo         1           // apakah data Gyroscope dikeluarkan pada FiFo
            #define UseAcceleroFiFo     0           // apakah data Accelerometer dikeluarkan pada FiFo
        #endif // ImuFilter
        // Pilih salah satu posisi penempatan board pada robot
        #define TIDUR                   0           // posisi board tegak atau berdiri
        #define TEGAK                   1           // Posisi board tidur
        #define PosisiBoard             TEGAK // posisi board dalam robot
        // Variabel yang digunakan
     #endif

//  mengaktifkan pemeriksaan tegangan batere saat robot berjalan
    #define CekTeganganBatere           1           // 1 = aktif

#endif // MYMODULE_H_INCLUDED
