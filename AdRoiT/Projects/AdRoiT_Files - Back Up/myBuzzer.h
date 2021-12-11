/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Buzzer access and tone generator in ADROIT-AVR

This program is free software: you can redistribute it and/or modify it under
the terms of the version 3 GNU General Public License as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*********************************************************/

#ifndef MYBUZZER_H_INCLUDED
#define MYBUZZER_H_INCLUDED

// -----------------  Fungsi Buzzer ----------------------------

class myBuzzer
{  public:
        void SetFrekwensi(uint16_t x);
        void Off();
        void Beep(uint16_t Frek, uint16_t Waktu);
        void Nada1(void);
        void Nada2(void);
        void Nada3(void);
        void Nada4(void);
        void NadaError(void);
#if UseMusic == 1
        void Lagu(uint8_t OnOff);
#endif // UseMusic
}buzzer;

void myBuzzer::Off()
{ TCCR0=0x00; ASSR=0x00; PORTB&=~(1<<4);}

void myBuzzer::SetFrekwensi(uint16_t x)
{ if((x>=125) && (x<=10000))
  {     if (x>=1000)           // clock 500KHz
        { TCCR0=0x1B; TCNT0=0;
          OCR0 = (uint8_t)((uint32_t)250000 / x);
        }
        else if (x>=500)            // clock 250KHz
        { TCCR0=0x1C; TCNT0=0;
          OCR0 = (uint8_t)((uint32_t)125000 / x);
        }
        else if (x>=250)            // clock 125KHz
        { TCCR0=0x1D; TCNT0=0;
          OCR0 = (uint8_t)((uint32_t)67500 / x);
        }
        else                        // clock 62,500KHz
        { TCCR0=0x1E; TCNT0=0;
          OCR0 = (uint8_t)((uint16_t)33750 / x);
        }
  }
  else
  { myBuzzer::Off();
  }
}

void myBuzzer::Beep(uint16_t Frek, uint16_t Waktu)
{
    #if UseMusic == 1
    Lagu(OFF);
    #endif // UseMusic
    myBuzzer::SetFrekwensi(Frek);
    delay_ms(Waktu);
    myBuzzer::Off();
};

void myBuzzer::Nada1(void)
{
    #if UseMusic == 1
    Lagu(OFF);
    #endif // UseMusic
    Beep(1000,200);     Beep(500,50);
    Beep(3000,100);     Beep(500,50);
    Beep(3000,200);     Beep(500,50);
    Beep(2000,100);
}

void myBuzzer::Nada2(void)
{
    #if UseMusic == 1
    Lagu(OFF);
    #endif // UseMusic
    Beep(3000,200);     Beep(500,50);
    Beep(2500,250);     Beep(500,50);
    Beep(1000,100);     Beep(500,50);
    Beep(3000,100);
}

void myBuzzer::Nada3(void)
{
    #if UseMusic == 1
    Lagu(OFF);
    #endif // UseMusic
    Beep(3000,100);     Beep(500,50);
    Beep(1000,250);     Beep(500,50);
    Beep(3000,200);     Beep(500,50);
    Beep(2000,100);
}

void myBuzzer::Nada4(void)
{
    #if UseMusic == 1
    Lagu(OFF);
    #endif // UseMusic
    Beep(2000,250);     Beep(500,50);
    Beep(3000,100);     Beep(500,50);
    Beep(3500,100);     Beep(500,50);
    Beep(3000,200);
}

void myBuzzer::NadaError(void)
{
    #if UseMusic == 1
    Lagu(OFF);
    #endif // UseMusic
    Beep(2000,250);     _delay_ms(500);
    Beep(2000,250);     _delay_ms(500);
    Beep(2000,250);

}

#if UseMusic == 1

void myBuzzer::Lagu(uint8_t OnOff)
{   if(OnOff)   myFlag.LaguOn = 1;
    else        myFlag.LaguOn = 0;
}

//Super Mario --> 78 Nada
    const uint16_t Melodi[] PROGMEM= {
        E7, E7,  0, E7, 0, C7, E7, 0, G7,  0,  0,  0, G6,  0,  0, 0,
        C7,  0,  0, G6, 0,  0, E6, 0,  0, A6,  0, B6,  0, AS6, A6, 0,
        G6, E7, G7, A7, 0, F7, G7, 0, E7,  0, C7, D7, B6,  0,  0,
        C7,  0,  0, G6, 0,  0, E6, 0,  0, A6,  0, B6,  0, AS6, A6, 0,
        G6, E7, G7, A7, 0, F7, G7, 0, E7,  0, C7, D7, B6,  0,  0};

     const uint8_t  Tempo[] PROGMEM= {
        40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,
        40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,
        50,50,50,33,40,40,40,40,40,40,40,40,40,40,40,
        40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,
        50,50,50,33,40,40,40,40,40,40,40,40,40,40,40};

    #define JumlahNada  78
#endif // UseMusic

#endif // MYBUZZER_H_INCLUDED
