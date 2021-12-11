#ifndef MYSYSTEM_H_INCLUDED
#define MYSYSTEM_H_INCLUDED

/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : - System initialisation Routine
          - Interrupt Service Routines
*********************************************************/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "myModule.h"
#include "myDefinitions.h"
#include "myGlobalVars.h"
#include "myMotor.h"            // deklarasi obyek roda dan servo
#include "myBuzzer.h"           // deklarasi obyek buzzer
#include "mylcd.h"              // deklarasi obyek lcd
#include "myADC.h"              // deklarasi obyek adc
#include "myI2C.h"              // fungsi untuk mengakses komunikasi I2C (TWI)
#if UseIMU==1
    #include "myIMU.h"          // deklarasi obyek imu
#endif
#if UseCompass == 1
    #include "myCompass.h"      // deklarasi obyek kompas
#endif

// flag register global
register Flag    myFlag     asm("r2");
register Flag2   myFlag2    asm("r3");
int16_t xxx;      // variable global untuk pengecekan


// Perlu diperhatikan apabila resolusi enkoder yang tinggi bisa berakibat pulsa enkoder tidak terespon oleh interrupt
// Perhitungan Rotari Enkoder Motor
ISR(INT6_vect)
{
  if(P_Enkoder1A)
  {  if(P_Enkoder1B) {ClrGlobalInterrupt;   roda.EnkoderKi--;}
     else            {ClrGlobalInterrupt;   roda.EnkoderKi++;}
  }
  else
  {  if(P_Enkoder1B) {ClrGlobalInterrupt;   roda.EnkoderKi++;}
     else            {ClrGlobalInterrupt;   roda.EnkoderKi--;}
  }
  roda.dCounterKi++;
}

// Perhitungan Rotari Enkoder Motor 2
ISR(INT7_vect)
{ if(P_Enkoder2A)
  {  if(P_Enkoder2B) {ClrGlobalInterrupt;   roda.EnkoderKa++;}
     else            {ClrGlobalInterrupt;   roda.EnkoderKa--;}
  }
  else
  {  if(P_Enkoder2B) {ClrGlobalInterrupt;   roda.EnkoderKa--;}
     else            {ClrGlobalInterrupt;   roda.EnkoderKa++;}
  }
  roda.dCounterKa++;
}


// Interupsi dengan frekwensi 40KHz -- Timer 3 output compare C interrupt service routine
ISR(TIMER3_COMPC_vect)
{  static uint16_t ServoCounter=0;
   if (++ServoCounter<=104)      // data servo maksimalnya adalah 80 dari 800  (2ms dari 20ms)
   {
        #if UseServo1 == 1
            if ((uint8_t)ServoCounter==dServo1) {ClrP_Servo1;}
        #endif
        #if UseServo2 == 1
            if ((uint8_t)ServoCounter==dServo2) {ClrP_Servo2;}
        #endif
        #if UseServo3 == 1
            if ((uint8_t)ServoCounter==dServo3) {ClrP_Servo3;}
        #endif
        #if UseServo4 == 1
            if ((uint8_t)ServoCounter==dServo4) {ClrP_Servo4;}
        #endif
        #if UseServo5 == 1
            if ((uint8_t)ServoCounter==dServo5) {ClrP_Servo5;}
        #endif
   }
   else if (ServoCounter>1000)
   {    ServoCounter = 0;
        //P_Servo1_4T = P_Servo1_4B | 0xF0;
        #if UseServo1 == 1
            SetP_Servo1;
        #endif
        #if UseServo2 == 1
            SetP_Servo2;
        #endif
        #if UseServo3 == 1
            SetP_Servo3;
        #endif
        #if UseServo4 == 1
            SetP_Servo4;
        #endif
        #if UseServo5 == 1
            SetP_Servo5;
        #endif
   }
}

// Timer1 input capture interrupt service routine --> 4ms
// interrupt pada puncak hitungan dari TCNT1 (sinyal segi tiga dari mode phase correct PWM)
ISR(TIMER1_CAPT_vect)
{
// karena frekwensi PWM untuk motor Servo adalah 50Hz (20ms), maka pulsa diaktifkan hanya 1 dalam 5 kali interrupt
    static uint8_t ServoCounter=5;
#if UseMusic == 1
    static uint8_t LaguTick=0, TempoTick=0;
#endif // UseMusic
    if((--ServoCounter)==0)
    {   ServoCounter = 5;       // reload hitungan
        //TCCR1A=0xFC;          // mode phase correct PWM dengan output inverted
        TCCR1A=0xAA;            // mode phase correct PWM dengan output non inverted (center PWM adalah pada dasar dari TCNT1)
    }
    else
    {   TCCR1A=0x02;            // mode phase correct PWM dengan output disabled
        //ClrP_Servo6;          // output dibuat 0 ketika output PWM tidak diaktifkan
        //ClrP_Servo7;          // output dibuat 0 ketika output PWM tidak diaktifkan
        //ClrP_Servo8;          // output dibuat 0 ketika output PWM tidak diaktifkan
    }
    SetGlobalInterrupt;         // mengaktifkan interrupt dari event yang lain
// menyalakan lagu pada buzzer
#if UseMusic == 1
    if(myFlag.LaguOn)
    {   if(++TempoTick>=pgm_read_byte(&Tempo[LaguTick]))            // Apakah hitungan tempo terpenuhi
        {   TempoTick = 0;                                          // reset hitungan tempo
            buzzer.SetFrekwensi(pgm_read_dword(&Melodi[LaguTick])); // mengeluarkan nada ke buzzer
            if (++LaguTick>=JumlahNada) LaguTick=0;                 // reset kembali deretan nada ketika lagu selesai
        }
    }
#endif // UseMusic
}

// Interrupt setiap 4ms
// interrupt pada dasar hitungan dari TCNT1 (sinyal segi tiga dari mode phase correct PWM)
ISR(TIMER1_OVF_vect)
{
// Kalkulasi IMU
asm("sei"::);                                   // mengaktifkan interrupt dari event yang lain

#if UseCompass ==1
static unsigned char MagnetoSampling=0;

    if (myFlag.CompassStart)
    {   if(++MagnetoSampling>=8)    // sampling data magneto setiap 32ms (8x4ms)
        {   MagnetoSampling=0;
            kompas.ReadMagnetoMeter();
        }
    }
#endif // UseCompass
#if UseIMU == 1
    //unsigned char i;
    myFlag.ImuToggle = ~myFlag.ImuToggle;
    if (myFlag.ImuStart)                            // program ini dijalankan hanya ketika IMU diaktifkan
    {
    #if UseCompass ==1
        if (myFlag.CompassStart)
        {   if(MagnetoSampling)         // jika kompas di start, maka setiap kompas di sampling, maka
            imu.BacaAccelerometer();    // data accelerometer tidak dibaca (menggunakan data sebelumnya)
        }
        else
        {   imu.BacaAccelerometer();
        }

    #else
        imu.BacaAccelerometer();
    #endif // UseCompass
    #if ModeBalancing == 1                          // jika mode balancing diaktifkan
        imu.BacaGyroY();                            // untuk menghitung sudut pitch
        #if ImuFilter == KalmanFilter               // jika menggunakan kalman filter
        imu.HitungKalmanPitch();                    // menghitung sudut pitch dari robot menggunakan kalman filter
        #elif ImuFilter == ComplementaryFilter      // jika mengggunakan complementary filter
        imu.HitungComplementaryPitch();             // menghitung sudut pitch dari robot menggunakan complementary filter
        #endif    //ImuFilter
        AdRoiT.BalancingControl();                  // perhitungan kendali balancing robot
    #else                                           // jika mode balancing diaktifkan
        imu.BacaGyroScope();
        #if ImuFilter == KalmanFilter
            if(myFlag.ImuToggle)    imu.HitungKalmanRoll();         // menghitung sudut roll menggunakan kalman filter
            else                    imu.HitungKalmanPitch();        // menghitung sudut pitch menggunakan kalman filter
            imu.HitungYaw();                                        // menghitung sudut yaw
        #elif ImuFilter == ComplementaryFilter
            if(myFlag.ImuToggle)    imu.HitungComplementaryRoll();  // menghitung sudut roll menggunakan complementary filter
            else                    imu.HitungComplementaryPitch(); // menghitung sudut pitch menggunakan complementary filter
            imu.HitungYaw();                                        // menghitung sudut yaw
        #elif ImuFilter == MahonyFilter
            imu.HitungMahonyFilter();                               // menghitung sudut roll, pitch dan yaw dengan mahony filter
        #endif // ImuFilter
    #endif // ModeBalancing
        // menghitung jumlah tick dari total waktu eksekusi
        //if(SysTick>i)    WaktuEksekusi = SysTick - i;
        //else             WaktuEksekusi = 0xFF - i + SysTick + 1;
    }
#endif // UseIMU
}

// Interupsi setiap 1 ms
ISR(TIMER2_COMP_vect)
{   static uint8_t _dCounterKi=0, _dCounterKa=0, Timer2Tick=10;
    SetGlobalInterrupt;
    if(--Timer2Tick==0) Timer2Tick=10;
    switch(Timer2Tick)
    {   case    9:
            if(roda.dCounterKi>_dCounterKi) roda.dSpeedKi = roda.dCounterKi - _dCounterKi;               // hitung selisih saat tidak overflow
            else                            roda.dSpeedKi = 0xFF - _dCounterKi + roda.dCounterKi + 1;    // hitung selisih saat overflow
            _dCounterKi = roda.dCounterKi;
            break;
        case    8:
            #if UsePIDmotor == 1
            if(myFlag.PIDMotorOn) roda.HitungPIDmotorKi(); // menghitung PID motor 1 jika mode PID motor diaktifkan
            #endif
            break;
        case    4:
            if(roda.dCounterKa>_dCounterKa) roda.dSpeedKa = roda.dCounterKa - _dCounterKa;               // hitung selisih saat tidak overflow
            else                            roda.dSpeedKa = 0xFF - _dCounterKa + roda.dCounterKa + 1;    // hitung selisih saat overflow
            _dCounterKa = roda.dCounterKa;
            break;
        case    3:
            #if UsePIDmotor == 1
            if(myFlag.PIDMotorOn) roda.HitungPIDmotorKa(); // menghitung PID motor 2 jika mode PID motor diaktifkan
            #endif
            break;
    }
}

void SystemInit(void)
{   int16_t x;
    myFlag.LcdBacklight = 0;
    myFlag.SerialTimeOut = 0;
    myFlag2.FlagBelok = 0;
    myFlag.AdcBusy = 0;
#if UseIMU == 1
    myFlag.ImuStart = 0;
    myFlag.ImuToggle = 0;
#endif // UseIMU
#if UseMusic == 1
    myFlag.LaguOn = 0;
#endif // UseMusic
#if UsePIDmotor == 1
    myFlag.PIDMotorOn = 0;
#endif // UsePIDmotor

 //-------------- Inisialisasi Port -----------------------------------
    // Port A  = Koneksi ke LCD --> semua bit difungsikan sebagai output
    PORTA=0x00; DDRA=0xFF;

    // Port B0   = Input Pull-Up (Enkoder1B)
    // Port B1   = Input (SCK - ISP programing)
    // Port B2   = Input Pull-Up (Enkoder2B)
    // Port B3   = Output (Servo 5)
    // Port B4   = Output (Buzzer)
    // Port B5-7 = Output (Servo 6-8)
    PORTB=0x05; DDRB=0xF8;

    // Port C0-3 = Output (LED Active High)
    // Port C4-7 = Input Pull-Up (Push Button Active Low)
    PORTC=0xFF; DDRC=0xF0;

    // Port D0,1 = Input (I2C)
    // Port D2,3 = Input (USART1)
    // Port D2,3 = Input (USART1)
    // Port D4-7 = Output(Servo1-4)
    PORTD=0x00; DDRD=0xF0;

    // Port E0,1 = Input (USART0, ISP programming)
    // Port E2   = Output (DirM2)
    // Port E3   = Output (PwmM2)
    // Port E4   = Output (PwmM1)
    // Port E5   = Output (DirM2)
    // Port E6,7 = Input Pull-Up (Enkoder1A,2A)
    PORTE=0xC0; DDRE=0x3C;

    // Port F  = Input (Input ADC)
    PORTF=0x00; DDRF=0x00;

    // Port G  = Input Pull-Up (Saklar Mode Active Low)
    PORTG=0xFF;
    DDRG=0x00;

 //-------------- Inisialisasi Timer -----------------------------------
    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 250,000 kHz
    // Mode: CTC top=OCR0
    // OC0 output: Toggle on compare match
    ASSR=0x00;
    TCCR0=0x00; // sementara dimatikan (diaktifkan ketika fungsi FBuzzer(Frek) dipanggil)
    TCNT0=0x00;
    OCR0=0x00;


    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: 2000,000 kHz
    // Mode: Ph. correct PWM top=ICR1
    // OC1A output: Non-Inv.    --> Data Servo 6
    // OC1B output: Non-Inv.    --> Data Servo 7
    // OC1C output: Non-Inv.    --> Data Servo 8
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer1 Overflow Interrupt: On
    // Input Capture Interrupt: On
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    // Compare C Match Interrupt: Off
    TCCR1A=0xAA;
    TCCR1B=0x12;
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1 = 4000;    // interrupt pada sisi atas sinyal segi tiga = 2000000 / (2*4000) = 250KHz = 4ms
                    // frekwensi motor servo = frekwensi interupt = 4ms

    // Timer/Counter 2 initialization
    // Clock source: System Clock
    // Clock value: 62,500 kHz
    // Mode: CTC top=OCR2
    // OC2 output: Disconnected
    //TCCR2=0x0C;       // 64,500 kHz        -- interupsi 4 ms
    TCCR2=0x0B;         // 250,000 kHz       -- interupsi 1 ms
    TCNT2=0x00;
    OCR2=0xFA;

    // Timer/Counter 3 initialization
    // Clock source: System Clock
    // Clock value: 16000,000 kHz
    // Mode: Ph. correct PWM top=ICR3
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // OC3A output: Inverted
    // OC3B output: Inverted
    // OC3C output: Discon.
    // Timer 3 Overflow Interrupt: Off
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    // Compare C Match Interrupt: On
    TCCR3A=0xF2;
    TCCR3B=0x11;
    TCNT3H=0x00;
    TCNT3L=0x00;
    ICR3H=0x01;  // 0x190 = 400
    ICR3L=0x90;  // frekwensi PWM untuk motor roda = 16000000/(400*2) = 20KHz
    OCR3AH=0x00;
    OCR3AL=0x00;
    OCR3BH=0x00;
    OCR3BL=0x00;
    OCR3CH=0x00; // 0xc8 = 200 --> frekwesni interupsi = 16000000/400 = 40KHz
    OCR3CL=0xC8; // interupt setiap di tengah sisi naik dan sisi turun dari sinyal segitiga (TCNT3)
                 // saat isi OCR3C = TCNT3 --> interupsi digunakan untuk membangkitkan sinyal PWM dari Servo 1-5

    // External Interrupt(s) initialization
    // INT0: Off
    // INT1: Off
    // INT2: Off
    // INT3: Off
    // INT4: Off
    // INT5: Off
    // INT6: On                 // (model Any change membuat jumlah ppr akhir = 2x jumlah data/lubang enkoder)
    // INT6 Mode: Any change    // interupsi untuk mencacah sinyal enkoder motor 1
    // INT7: On
    // INT7 Mode: Any change    // interupsi untuk mencacah sinyal enkoder motor 2
    EICRA=0x00;
    EICRB=0x50;
    EIMSK=0xC0;
    EIFR=0xC0;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    //TIMSK=0x84; //
    TIMSK=0xA4;     // T2 compare match(bit-7), T1 input capture (bit-5), T1 Overflow((bit-2))
    ETIMSK=0x02;    // T3 compare match C(bit-1)

// ------------------------ Inisialisasi USART1 ----------------------
    // Communication Parameters: 8 Data, 1 Stop, No Parity
    // USART1 Receiver: On
    // USART1 Transmitter: On
    // USART1 Mode: Asynchronous
    // USART1 Baud rate: 9600
    UCSR1A=0x00;
    UCSR1B=0x98;
    UCSR1C=0x06;
    UBRR1H=0x00;
    UBRR1L=0x67;

// ------------  Inisialisasi Analog Comparator -----------------------
    // Analog Comparator: Off
    // Analog Comparator Input Capture by Timer/Counter 1: Off
    ACSR=0x80;
    SFIOR=0x00;

// ------------ Inisialisasi Peripheral Board ------------------------

    //LCD_Init(); // -------------------------------------- LCD Text 16x2
    //Stop();     // memastikan motor roda mati

// -------------------------------Reset Sudut Servo

 ClrGlobalInterrupt;

 lcd.Init();                // inisialisasi LCD
 lcd.BackLight(ON);         // menyalakan backlight dari LCD text

 // cek tegangan batere
 lcd.Tulis(0,"Tegangan Batere:");
 lcd.GotoXY(0,1);
 x = adc.BacaBatere();                  // membaca tegangan batere melalui ADC kanal 0
 lcd.Data(x/100 + 48);
 lcd.Data('.');
 lcd.Data((x%100)/10 + 48);
 lcd.Data(x%10 + 48);
 lcd.Text(" Volt");
 buzzer.Nada1();
 _delay_ms(1000);

 adc.CekBatere();           // memeriksa tegangan batere apakah masih > 6.5V

#if UseIMU == 1             // jika MPU6050 diaktifkan melalui opsi UseIMU
        i2c.Init();         // inisialisasi komunikasi I2C untuk berkomunikasi dengan MPU 6050 dan HMC5883L
        if (imu.Init())
            {   lcd.Tulis (0,"IMU Init OK >>>>");
                imu.KalibrasiGyroScope();
                lcd.Tulis(1,"Gyro Callibrated");
                buzzer.Nada1();
            }
        else
            {   lcd.TulisKiri   (0,"IMU Init Fail >>");
                lcd.TulisKiri   (1,"Check Hardware!!");
                buzzer.Nada2();
            }
        buzzer.Nada3();
#if UseCompass == 1     // jika Kompas HMC5883L diaktifkan melalui opsi UseCompass
        if (kompas.Init())
            {   lcd.Tulis   (0,"Magneto Init OK.");
            }
        else
            {   lcd.Tulis   (0,"MagnetoInit Fail");
            }
        lcd.TulisTengah (1,"<<<<<<<<>>>>>>>>");
        buzzer.Nada1();
#endif  // UseCompass
#endif  // UseIMU

// -------------- Global enable interrupts ----------------------------
    SetGlobalInterrupt;
}
#endif // MYSYSTEM_H_INCLUDED
