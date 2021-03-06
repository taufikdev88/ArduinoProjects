/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.3 Standard
Automatic Program Generator
? Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 11/4/2019
Author  : unknown
Company : 
Comments: 


Chip type               : ATmega128
Program type            : Application
AVR Core Clock frequency: 16.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 1024
*****************************************************/

#include <mega128.h>
#include <delay.h>
#include "adroit.h"
#include "adroit_LCD.h"

#define SetGlobalInterrupt #asm("sei")
#define ClrGlobalInterrupt #asm("cli")
int EnkoderKi, EnkoderKa;
unsigned int dCounterKi, dCounterKa;
unsigned char dSpeedKi, dSpeedKa;
signed short SpeedKa, SpeedKi;

#define MaxPWM  (signed short)  400

#define KPmKa   (signed short)  8
#define KImKa   (signed short)  10
#define KDmKa   (signed short)  1

#define KPmKi   (signed short)  8
#define KImKi   (signed short)  10
#define KDmKi   (signed short)  1

void HitungPIDmotorKa(void);
void HitungPIDmotorKi(void);

// External Interrupt 6 service routine
interrupt [EXT_INT6] void ext_int6_isr(void)
{
// Place your code here
    if(P_Enkoder1A){
        if(P_Enkoder1B) {
            ClrGlobalInterrupt;
            EnkoderKi--;
        } else {
            ClrGlobalInterrupt;
            EnkoderKi++;
        }
    } else {
        if(P_Enkoder1B) {
            ClrGlobalInterrupt;
            EnkoderKi++;
        } else {
            ClrGlobalInterrupt;
            EnkoderKa--;
        }
    }
    SetGlobalInterrupt;
    dCounterKi++;
}

// External Interrupt 7 service routine
interrupt [EXT_INT7] void ext_int7_isr(void)
{
// Place your code here
    if(P_Enkoder2A) {
        if(P_Enkoder2B) {
            ClrGlobalInterrupt;
            EnkoderKa++;
        } else {
            ClrGlobalInterrupt;
            EnkoderKa--;
        }
    } else {
        if(P_Enkoder2B) {
            ClrGlobalInterrupt;
            EnkoderKa--;
        } else {
            ClrGlobalInterrupt;
            EnkoderKa++;
        }
    }
    SetGlobalInterrupt;
    dCounterKa++;
}

// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Place your code here

}

// Timer1 input capture interrupt service routine
interrupt [TIM1_CAPT] void timer1_capt_isr(void)
{
// Place your code here

}

// Timer2 output compare interrupt service routine
interrupt [TIM2_COMP] void timer2_comp_isr(void)
{
// Place your code here
    static unsigned short step_timer = 0, step = 0;
    static unsigned char _dCounterKi = 0, _dCounterKa=0, Timer2Tick = 10;
    SetGlobalInterrupt;
    if(--Timer2Tick == 0) Timer2Tick = 10;
    switch(Timer2Tick){
        case 9:
            if(dCounterKi > _dCounterKi) dSpeedKi = dCounterKi - _dCounterKi;
            else dSpeedKi = 0xFF - _dCounterKi + dCounterKi + 1;
            _dCounterKi = dCounterKi;
            break;
        case 8:
            //HitungPIDmotorKi();
            break;
        case 4:
            if(dCounterKa > _dCounterKa) dSpeedKa = dCounterKa - _dCounterKa;
            else dSpeedKa = 0xFF - _dCounterKa + dCounterKa + 1;
            _dCounterKa = dCounterKa;
            break;
        case 3:
            HitungPIDmotorKa();
            break;
    }
        
    if(step_timer++ > 500){
        if(step == 0){  
            SpeedKa = 0;
            PwmMKi(0);
            step = 1;
        } else {
            step = 0; 
            SpeedKa = 18;
            PwmMKi(400);
        }
        step_timer = 0;
    }               
}

// Declare your global variables here

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
PORTA=0x00;
DDRA=0xFF;

// Port B initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=In Func1=In Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=P State1=T State0=P 
PORTB=0x05;
DDRB=0xF8;

// Port C initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=P State2=P State1=P State0=P 
PORTC=0x0F;
DDRC=0xF0;

// Port D initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0xF0;

// Port E initialization
// Func7=In Func6=In Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In 
// State7=P State6=P State5=0 State4=0 State3=0 State2=0 State1=T State0=T 
PORTE=0xC0;
DDRE=0x3C;

// Port F initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTF=0x00;
DDRF=0x00;

// Port G initialization
// Func4=In Func3=In Func2=In Func1=In Func0=In 
// State4=T State3=P State2=P State1=P State0=P 
PORTG=0x0F;
DDRG=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 250.000 kHz
// Mode: CTC top=OCR0
// OC0 output: Toggle on compare match
ASSR=0x00;
TCCR0=0x1C;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 2000.000 kHz
// Mode: Ph. & fr. cor. PWM top=ICR1
// OC1A output: Non-Inv.
// OC1B output: Non-Inv.
// OC1C output: Non-Inv.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: On
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR1A=0xA8;
TCCR1B=0x12;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x13;
ICR1L=0x87;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;
OCR1CH=0x00;
OCR1CL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 250.000 kHz
// Mode: CTC top=OCR2
// OC2 output: Disconnected
TCCR2=0x0B;
TCNT2=0x00;
OCR2=0xF9;

// Timer/Counter 3 initialization
// Clock source: System Clock
// Clock value: 16000.000 kHz
// Mode: Ph. correct PWM top=ICR3
// OC3A output: Inverted
// OC3B output: Inverted
// OC3C output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer3 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR3A=0xF2;
TCCR3B=0x11;
TCNT3H=0x00;
TCNT3L=0x00;
ICR3H=0x01;
ICR3L=0x90;
OCR3AH=0x00;
OCR3AL=0x00;
OCR3BH=0x00;
OCR3BL=0x00;
OCR3CH=0x00;
OCR3CL=0xC8;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
// INT3: Off
// INT4: Off
// INT5: Off
// INT6: On
// INT6 Mode: Any change
// INT7: On
// INT7 Mode: Any change
EICRA=0x00;
EICRB=0x50;
EIMSK=0xC0;
EIFR=0xC0;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0xA4;

ETIMSK=0x00;

// USART0 initialization
// USART0 disabled
UCSR0B=0x00;

// USART1 initialization
// USART1 disabled
UCSR1B=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC disabled
ADCSRA=0x00;

// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;

// Global enable interrupts
#asm("sei")

LCD_Init();

SpeedKa = 2;
SpeedKi = 2;

LCD_TulisF(0,"Kecepatan Ka:");
LCD_TulisF(1,"Kecepatan Ki:");

while (1)
      {
      // Place your code here
        LCD_GotoXY(13,0); LCD_Angka3(dSpeedKa); LCD_Data(' ');
        LCD_GotoXY(13,1); LCD_Angka3(dSpeedKi); LCD_Data(' ');
      }
}

void HitungPIDmotorKi(){
    signed char ErrorMKi, dErrorMKi;
    signed short U;
    static signed char iErrorMKi = 0, lastErrorMKi = 0;
    
    if(SpeedKi > 0) {
        ErrorMKi = (signed char) SpeedKi - (signed char) dSpeedKi;
        iErrorMKi += ErrorMKi;
        
        if(iErrorMKi > 40){  iErrorMKi = 40;}
        else if(iErrorMKi < -40){ iErrorMKi = -40;}
        
        dErrorMKi = lastErrorMKi - ErrorMKi;
        
        U = ErrorMKi * KPmKi + iErrorMKi * KImKi + dErrorMKi * KDmKi;
        
        if(U > MaxPWM) {PwmMKi(MaxPWM);}
        else if(U > 0) {PwmMKi(U);}
        else {PwmMKi(0);}
    } else {
        PwmMKi(0);
        iErrorMKi = 0;
        lastErrorMKi = 0;
    }
}

void HitungPIDmotorKa(){
    signed char ErrorMKa, dErrorMKa;
    signed short U;
    static signed char iErrorMKa = 0, lastErrorMKa = 0;
    
    if(SpeedKa > 0) {
        ErrorMKa = (signed char) SpeedKa - (signed char) dSpeedKa;
        iErrorMKa += ErrorMKa;
        
        if(iErrorMKa > 40)  {iErrorMKa = 40;}
        else if(iErrorMKa < -40)    {iErrorMKa = -40;}
        
        dErrorMKa = lastErrorMKa - ErrorMKa;
        
        U = ErrorMKa * KPmKa + iErrorMKa * KImKa + dErrorMKa * KDmKa;
        
        if(U > MaxPWM) {PwmMKa(MaxPWM); }
        else if(U > 0) {PwmMKa(U);}
        else {PwmMKa(0);}
    } else {
        PwmMKa(0);
        iErrorMKa = 0;
        lastErrorMKa = 0;
    }
}