#ifndef _MYGLOBALVARS_
#define _MYGLOBALVARS_              1  

// Pengaktifan fitur pada board kontroler, isi 1 jika diaktifkan, isi 0 jika tidak
#define ModeBalancing           1           // 1, jika mode balancing robot diaktifkan
    
    // motor servo yang digenerate menggunakan emulasi PWM dari timer 3
    #define UseServo1                   0           // Mengaktifkan Servo 1 ?
    #define UseServo2                   0           // Mengaktifkan Servo 2 ?
    #define UseServo3                   0           // Mengaktifkan Servo 3 ?
    #define UseServo4                   0           // Mengaktifkan Servo 4 ?
    #define UseServo5                   0           // Mengaktifkan Servo 5 ?
    // servo 6, 7 dan 8 secara default aktif dengan PWM hardware dari board kontroler

    #if UseServo5 == 1 
        extern unsigned char dServo5; 
    #endif
    #if UseServo1 == 1 
        extern unsigned char dServo1; 
    #endif
    #if UseServo2 == 1 
        extern unsigned char dServo2; 
    #endif
    #if UseServo3 == 1 
        extern unsigned char dServo3; 
    #endif
    #if UseServo4 == 1 
        extern unsigned char dServo4; 
    #endif     
    
#if ModeBalancing == 1
    #define UseIMU		1  // mode ini membutuhkan IMU untuk diaktifkan 
    #define UsePIDmotor 	0  // fitur PID untuk kendali kecepatan roda (1=on)   
            extern int8_t TargetSpeedB;
    extern int16_t SteerB;
#else
    #define UseIMU            0  // apakah mengaktifkan fitur IMU? 
    #define UsePIDmotor       1  // fitur PID untuk kendali kecepatan roda (1=on) 
#endif
    
#if UseIMU == 1
    #define UseCompass              0
    // Pilih salah satu jenis filter yang diaktifkan
    #define KalmanFilter            1
    #define ComplementaryFilter     2
    #define MahonyFilter            3
    #define ImuFilter               KalmanFilter
    // Pilih salah satu posisi penempatan board pada robot
    #define TIDUR                   0           // posisi board tegak atau berdiri 
    #define TEGAK                   1           // Posisi board tidur
    #define PosisiBoard             TIDUR       // posisi board dalam robot 
    // data mentah dari gyroscope dan accelerometer    
    extern volatile int16_t Ax, Ay, Az, Gx, Gy, Gz;    
    extern volatile float GxOffset, GyOffset, GzOffset;  // offset dari gyroscope
    extern volatile float Roll, Pitch, Yaw;  
       	#endif
#if UsePIDmotor == 1
    extern volatile int8_t  dMotor1, dMotor2; 
    extern bit              PIDMotorOn;
#endif    
extern volatile int8_t      dSpeed1, dSpeed2;      	// data kecepatan motor
extern volatile uint8_t     dCounter1, dCounter2;       // data counter dari motor
extern bit                  LaguOn, FlagSerial;

// konstanta konversi radian dan derajat
#define RAD2DEG     (float) 57.295779513082320876798154814105  
#define DEG2RAD     (float) 0.01745329251994329576923690768489     
    #endif
