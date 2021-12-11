/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 3/13/2014
Author  : Eko Henfri Binugroho  
Company : ER2C

Code    : Buzzer access and tone generator  
*********************************************************/

#ifndef _myBuzzer_
#define _myBuzzer_

#include <mega128.h>
#include <delay.h>

bit LaguOn=0;

// -----------------  Fungsi Buzzer ----------------------------
#pragma used+
void BuzzerOff();
void FBuzzer(uint16_t x);
void Buzzer(uint16_t Frek, uint16_t Tempo);
void Nada1(void);
void Nada2(void);
void Nada3(void);
void Nada4(void);

void BuzzerOff()
{ TCCR0=0x00; PORTB.4=0; ASSR=0x00; }

void FBuzzer(unsigned uint16_t x)
{ 
  if(x>=125 && x<=10000)
  {     if (x>=2000)
        { TCCR0=0x1A; TCNT0=0;
          OCR0 = 500000 / x; 
        }
        if (x>=1000)
        { TCCR0=0x1B; TCNT0=0;
          OCR0 = 250000 / x; 
        }
        else if (x>=500)
        { TCCR0=0x1C; TCNT0=0;
          OCR0 = 125000 / x; 
        }  
        else if (x>=250)
        { TCCR0=0x1D; TCNT0=0;
          OCR0 = 67500 / x; 
        }  
        else
        { TCCR0=0x1E; TCNT0=0;
          OCR0 = 33750 / x; 
        }
  }
  else
  { BuzzerOff();
  }
} 

void Buzzer(uint16_t Frek, uint16_t Tempo)
{  FBuzzer(Frek); delay_ms(Tempo);BuzzerOff();}

void Nada1(void)
{   Buzzer(1000,100);Buzzer(500,20);
    Buzzer(3000,50);Buzzer(500,20);
    Buzzer(3000,100);Buzzer(500,20);
    Buzzer(2000,50);
}

void Nada2(void)
{   Buzzer(3000,100);Buzzer(500,20);
    Buzzer(2500,150);Buzzer(500,20);
    Buzzer(1000,50);Buzzer(500,20);
    Buzzer(3000,50);
}

void Nada3(void)
{   Buzzer(3000,50);Buzzer(500,20);
    Buzzer(1000,150);Buzzer(500,20);
    Buzzer(3000,100);Buzzer(500,20);
    Buzzer(2000,50);
}

void Nada4(void)
{   Buzzer(2000,150);Buzzer(500,20);
    Buzzer(3000,50);Buzzer(500,20);
    Buzzer(3500,50);Buzzer(500,20);
    Buzzer(3000,100);
}
#pragma used-
 

//Super Mario --> 78 Nada
uint16_t flash Melodi[] = {
  E7, E7,  0, E7, 0, C7, E7, 0, G7,  0,  0,  0, G6,  0,  0, 0, 
  C7,  0,  0, G6, 0,  0, E6, 0,  0, A6,  0, B6,  0,AS6, A6, 0, 
  G6, E7, G7, A7, 0, F7, G7, 0, E7,  0, C7, D7, B6,  0,  0,
  C7,  0,  0, G6, 0,  0, E6, 0,  0, A6,  0, B6,  0,AS6, A6, 0, 
  G6, E7, G7, A7, 0, F7, G7, 0, E7,  0, C7, D7, B6,  0,  0};  

uint8_t flash Tempo[] = {
  30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, 
  30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, 
  35,35,35,24,30,30,30,30,30,30,30,30,30,30,30,
  30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, 
  35,35,35,24,30,30,30,30,30,30,30,30,30,30,30};

#define JumlahNada  78
#endif  