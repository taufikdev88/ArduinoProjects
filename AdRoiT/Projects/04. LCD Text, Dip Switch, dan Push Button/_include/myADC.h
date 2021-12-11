/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Program Inisialisasi ADC dan akses pada ADC
*********************************************************/
#ifndef MYADC_H_INCLUDED
#define MYADC_H_INCLUDED

#define VBATT       0
#define F1000KHz    0
#define F500KHz     1
#define F250KHz     2
#define F125KHz     3
#define AdcClock    F1000KHz        // penilihan clock ADC yang dipilih
#define ResolusiADC 8               // pemilihan resolusi ADC (8 atau 10 dalam bit)

#if ResolusiADC == 8
    #define ADC_VREF_TYPE   (uint8_t)   0x20
#else
    #define ADC_VREF_TYPE   (uint8_t)   0x00
#endif // ReselusiADC


class myADC
{   public:
        myADC(void);
    #if ResolusiADC == 8
            uint8_t Baca(uint8_t Kanal);
    #else
            uint16_t Baca(unsigned char Kanal);
    #endif // ReselusiADC
        int16_t BacaBatere(void);
        void    CekBatere(void);
}adc;

myADC::myADC(void)
{
// program inisialisasi ADC
// Frekuensi Clock ADC = 1000,000 kHz
// Tegangan referensi ADC yang digunakan = tegangan pada pin AREF
// resulusi data 8 bit
    ADMUX  = (ADC_VREF_TYPE);
    ADCSRA = 0x84 | AdcClock;
}

#if ResolusiADC == 8
uint8_t myADC::Baca(uint8_t Kanal)
{
    myFlag.AdcBusy = 1;
    ADMUX = Kanal | 0x20;
// delay sampai tegangan input analog menjadi stabil
    _delay_us(10);
// Start konsversi ADC
    ADCSRA|=0x40;
// Menunggu sampai proses konversi ADC selesai
    while ((ADCSRA & 0x10)==0);     // mode pooling
    ADCSRA|=0x10;
    myFlag.AdcBusy = 0;
    return ADCH;
}

// program membaca tegangan batere menggunakan ADC kanal 0
// nilai hasil pembacaaan adalah 100x tegangan yang terukur
int16_t myADC::BacaBatere(void)
{   while(myFlag.AdcBusy);      // menunggu konversi ADC sebelumnya selesai
    return ((int16_t) myADC::Baca(VBATT)*94/19);
}

#else
uint16_t myADC::Baca(unsigned char Kanal)
{
    myFlag.AdcBusy = 1;
    ADMUX = Kanal;
// delay sampai tegangan input analog menjadi stabil
    _delay_us(10);
// Start konsversi ADC
    ADCSRA|=0x40;
// Menunggu sampai proses konversi ADC selesai
    while ((ADCSRA & 0x10)==0);     // mode pooling
    ADCSRA|=0x10;
    myFlag.AdcBusy = 0;
    return (*(uint16_t *)&ADCL);
}

// program membaca tegangan batere menggunakan ADC kanal 0
// nilai hasil pembacaaan adalah 100x tegangan yang terukur
int16_t myADC::BacaBatere(void)
{   return ( (int16_t)myADC::Baca(VBATT)*47/38);
}
#endif // ReselusiADC

void myADC::CekBatere(void)     // menghentikan sistem jika tegangan kurang dari 6.5V (untuk batere 2 cell)
{
 if(myFlag.AdcBusy==0)          // lakukan pengecekan jika ADC sedang tidak melakukan proses konversi yang lain
 {
#if ResolusiADC == 8            // batas bawah tegangan batere untuk 2 cell --> 6,5V = data 650
    if(myADC::Baca(VBATT)<131)  // 131 = 650*19/94
#else
    if(myADC::Baca(VBATT)<525)  // 525 = 650*38/47
#endif // ResolusiADC
    {
        ClrGlobalInterrupt;
        roda.SpeedPWM(0,0);
        lcd.Hapus();
        lcd.Tulis(0,"Vs.Batere < 6.5V");
        lcd.Tulis(1,"Isi Ulang Batere");
        while(1)
        {   buzzer.Beep(2000,100);
            _delay_ms(100);
            buzzer.Beep(2000,100);
            _delay_ms(300);
            lcd.BackLight(ON);
            _delay_ms(500);
            lcd.BackLight(OFF);
        };
    }
 }
}

#endif // MYADC_H_INCLUDED
