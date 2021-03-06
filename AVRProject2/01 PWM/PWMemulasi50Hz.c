/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.3 Standard
Automatic Program Generator
? Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 9/9/2019
Author  : unknown
Company : 
Comments: 


Chip type               : ATmega8535
Program type            : Application
AVR Core Clock frequency: 11.059200 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 128
*****************************************************/

#include <mega8535.h>
// Alias dari PORT
#define	PWM_1_OUT	PORTB.0
#define	PWM_2_OUT	PORTB.1
#define	PWM_3_OUT	PORTB.2
#define	PWM_4_OUT	PORTB.3
#define	PWM_5_OUT	PORTB.4
#define	PWM_6_OUT	PORTB.5
#define	PWM_7_OUT	PORTB.6
#define	PWM_8_OUT	PORTB.7
// Deklarasi variabel PWM
unsigned int 	pwm1, pwm2, pwm3, pwm4, pwm5, pwm6, pwm7, pwm8;


// Timer 0 output compare interrupt service routine
interrupt [TIM0_COMP] void timer0_comp_isr(void)
{
// Place your code here
    static int counter=0;
    if(++counter>799)	// jika hitungan counter sudah sampai di TOP (800)
    {  counter=0;		// reset nilai counter
       PORTB=0xFF;         
// semua nilai bit dari PORTB dibuat 1 (start sinyal PWM)
// perintah langsung mengakses ke port karena lebih efisien dibanding 
// membuat masing-masing bit menjadi 1     
    }
    if(pwm1==counter) PWM_1_OUT = 0;
    if(pwm2==counter) PWM_2_OUT = 0;
    if(pwm3==counter) PWM_3_OUT = 0;
    if(pwm4==counter) PWM_4_OUT = 0;
    if(pwm5==counter) PWM_5_OUT = 0;
    if(pwm6==counter) PWM_6_OUT = 0;
    if(pwm7==counter) PWM_7_OUT = 0;
    if(pwm8==counter) PWM_8_OUT = 0;
}

// Declare your global variables here

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0x00;

// Port B initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
PORTB=0x00;
DDRB=0xFF;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 1382.400 kHz
// Mode: CTC top=OCR0
// OC0 output: Disconnected
TCCR0=0x0A;
TCNT0=0x00;
OCR0=0x22;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=0x00;
MCUCSR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x02;

// USART initialization
// USART disabled
UCSRB=0x00;

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

// mengatur nilai duty cycle dari sinyal PWM (nilai 800 = maksimal, DC 100%)
pwm1 = 80;	//10/100*800 = (10%)
pwm2 = 160;	//10/100*800 = (20%)
pwm3 = 240;	//10/100*800 = (30%)
pwm4 = 320;	//10/100*800 = (40%)
pwm5 = 400;	//10/100*800 = (50%)
pwm6 = 480;	//10/100*800 = (60%)
pwm7 = 560;	//10/100*800 = (70%)
pwm8 = 640;	//10/100*800 = (80%)

while (1)
      {
      // Place your code here

      }
}
