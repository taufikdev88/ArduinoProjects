/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.3 Standard
Automatic Program Generator
© Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 5/16/2019
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

#include <delay.h>

#define LED         PORTB
#define DipSwitch   PINC
#define PwmA        OCR1A
#define PwmB        OCR1B

#ifndef RXB8
#define RXB8 1
#endif

#ifndef TXB8
#define TXB8 0
#endif

#ifndef UPE
#define UPE 2
#endif

#ifndef DOR
#define DOR 3
#endif

#ifndef FE
#define FE 4
#endif

#ifndef UDRE
#define UDRE 5
#endif

#ifndef RXC
#define RXC 7
#endif

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

// USART Receiver buffer
#define RX_BUFFER_SIZE 12
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE <= 256
unsigned char rx_wr_index,rx_rd_index,rx_counter;
#else
unsigned int rx_wr_index,rx_rd_index,rx_counter;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSRA;
data=UDR;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer[rx_wr_index++]=data;
#if RX_BUFFER_SIZE == 256
   // special case for receiver buffer size=256
   if (++rx_counter == 0) rx_buffer_overflow=1;
#else
   if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
   if (++rx_counter == RX_BUFFER_SIZE)
      {
      rx_counter=0;
      rx_buffer_overflow=1;
      }
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter==0);
data=rx_buffer[rx_rd_index++];
#if RX_BUFFER_SIZE != 256
if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
#endif
#asm("cli")
--rx_counter;
#asm("sei")
return data;
}
#pragma used-
#endif

// Standard Input/Output functions
#include <stdio.h>

#define FIRST_ADC_INPUT 0
#define LAST_ADC_INPUT 7
unsigned char adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
#define ADC_VREF_TYPE 0x20

// ADC interrupt service routine
// with auto input scanning
interrupt [ADC_INT] void adc_isr(void)
{
static unsigned char input_index=0;
// Read the 8 most significant bits
// of the AD conversion result
adc_data[input_index]=ADCH;
// Select next ADC input
if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
   input_index=0;
ADMUX=(FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff))+input_index;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
}

// Declare your global variables here
#define Header1 '!'
#define Header2 '@'
#define Header3 '#'
#define Header4 '$'
#define Error   '?'
#define Tail    '~'

void KirimData8Bit(unsigned char x)
{   putchar (x/100+48);
    putchar ((x/10)%10+48);
    putchar (x%10+48);
}

void main(void)
{
// Declare your local variables here
unsigned char dSerial;  // data yang dibaca dari buffer serial
unsigned char xData[8]; // buffer data serial untuk proses parsing
unsigned char index=0;  // data index dari buffer serial saat proses parsing
bit flagHeader=0;       // penanda apakah header sudah tertangkap / belum saat parsing
unsigned char hasil;    

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
// State7=P State6=P State5=P State4=P State3=P State2=P State1=P State0=P 
PORTC=0xFF;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=0 State4=0 State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x30;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 11059.200 kHz
// Mode: Fast PWM top=0x00FF
// OC1A output: Non-Inv.
// OC1B output: Non-Inv.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0xA1;
TCCR1B=0x09;
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
TIMSK=0x00;

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=0x00;
UCSRB=0x98;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x47;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 86.400 kHz
// ADC Voltage Reference: AREF pin
// ADC High Speed Mode: Off
// ADC Auto Trigger Source: ADC Stopped
// Only the 8 most significant bits of
// the AD conversion result are used
ADMUX=FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff);
ADCSRA=0xCF;
SFIOR&=0xEF;

// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;

// Global enable interrupts
#asm("sei")

while (1)
      {
      // Place your code here
      dSerial = getchar();    // membaca karakter dari buffer komunikasi serial
        if(flagHeader==0)     // jika header belum ketemu
        {   if(dSerial==Header1 || dSerial==Header2 || dSerial==Header3 || dSerial==Header4)
            {   index = 0;              // index direset
                xData[index]=dSerial;   // header disimpan pada xData index ke-0
                flagHeader=1;           // flag header aktif
            }
        }
        else                        	// jika header sudah ketemu
        {   if (dSerial==Tail)      	// apakah tail data sudah ketemu?
            {   // cek data dari header yang tertangkap
                switch(xData[0])
                {   case Header1:   	// digital input
                        		// dengan data dipswitch di-invert menjadi active high
                      	putchar(Header1); 
KirimData8Bit(~DipSwitch);
                     	putchar(Tail); 
break;    
                    case Header2:   	// digital output
                        // parsing data dari string ke desimal
                        if(index==3) 	// data benar apabila data =3 karakter
                        {  	putchar(Header2); 
  			     hasil = (xData[1]-48)*100;
                            hasil+= (xData[2]-48)*10;
                            hasil+= (xData[3]-48);
                            // dengan data LED di-invert menjadi active high
                            LED = ~hasil;
                        }
                        else putchar(Error);
                        break;
                    case Header3:
                        if(index==0)	// kalau membaca data semua kanal
                        {   putchar(Header3); 
	 KirimData8Bit(adc_data[0]);
                            KirimData8Bit(adc_data[1]);
                            KirimData8Bit(adc_data[2]);
                            KirimData8Bit(adc_data[3]);
                            KirimData8Bit(adc_data[4]);
                            KirimData8Bit(adc_data[5]);
                            KirimData8Bit(adc_data[6]);
                            KirimData8Bit(adc_data[7]);
	 putchar(Tail);
                        }                          
                        else if (xData[1]>='0' && xData[1]<='7')	// kalau salah satu kanal
                        {   putchar(Header3); 
	 		      KirimData8Bit(adc_data[xData[1]-48]);
	 putchar(Tail); 
                        }     
                        else putchar(Error);
                        break;
                    case Header4:
                        if(index==6)// menulis senua kanal
                        {   putchar(Header4); 
	 		      hasil = (xData[1]-48)*100;
                            hasil+= (xData[2]-48)*10;
                            hasil+= (xData[3]-48);
                            PwmA = hasil;
                            hasil = (xData[4]-48)*100;
                            hasil+= (xData[5]-48)*10;
                            hasil+= (xData[6]-48);
                            PwmB = hasil;
                        }
                        else if (xData[1]>='0' && xData[1]<='1')	// kalau salah satu kanal
                        {   if(index==4)
                            {   putchar(Header4); 
	 		      	  hasil = (xData[2]-48)*100;
                                hasil+= (xData[3]-48)*10;
                                hasil+= (xData[4]-48);
                                if (xData[1]=='0') PwmA = hasil;
                                else               PwmB = hasil;
                            }
                        }
                        else putchar(Error);
                        break;
                }
                flagHeader=0;	// flag disreset setiap ketemu tail (mengharap header baru)
            }
            else           	// selain tail, maka data disimpan
            {   if(++index>8)  flagHeader=0;// asumsi data terlalu panjang(error),flag direset
                xData[index] = dSerial;     // selama belum error data disimpan
            }
        }

      }
}
