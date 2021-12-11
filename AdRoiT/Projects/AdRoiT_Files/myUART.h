/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Communication routines through UART in ADROIT-AVR

This program is free software: you can redistribute it and/or modify it under
the terms of the version 3 GNU General Public License as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*********************************************************/

#ifndef MYUART_H_INCLUDED
#define MYUART_H_INCLUDED

#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

// USART1 Receiver buffer
#define RX_BUFFER_SIZE1 16
volatile int8_t rx_buffer1[RX_BUFFER_SIZE1];

#if RX_BUFFER_SIZE1<256
volatile uint8_t rx_wr_index1,rx_rd_index1,rx_counter1;
#else
volatile uint8_t rx_wr_index1,rx_rd_index1,rx_counter1;
#endif

// This flag is set on USART1 Receiver buffer overflow
uint8_t rx_buffer_overflow1;

// USART1 Receiver interrupt service routine
ISR(USART1_RX_vect)
{
volatile char status;
volatile unsigned char data;

status=UCSR1A;
data=UDR1;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer1[rx_wr_index1]=data;
   if (++rx_wr_index1 == RX_BUFFER_SIZE1) rx_wr_index1=0;
   if (++rx_counter1 == RX_BUFFER_SIZE1)
      {
      rx_counter1=0;
      rx_buffer_overflow1=1;
      };
   };
}

// rutin membaca data dari port serial 1 dilengkapi dengan fungsi time out
// data gagal diterima apa bila nilai myFlag.SerialTimeOut = 0
uint8_t BacaSerial1(uint16_t TimeOut)
{   uint8_t data;
    do{ if (rx_counter1!=0)
        {   myFlag.SerialTimeOut = 0;       // tidak jadi timeout jika data ternyata diterima
            data=rx_buffer1[rx_rd_index1];
            if (++rx_rd_index1 == RX_BUFFER_SIZE1) rx_rd_index1=0;
            ClrGlobalInterrupt;   --rx_counter1;  SetGlobalInterrupt;
            return data;
            break;
        }
        _delay_us(1);
    }while (--TimeOut);
    myFlag.SerialTimeOut = 1;               // asumsi awal terjadi timeout
    return 0;
}

// fungsi untuk membaca karakter dari buufer komunikasi serial
// program akan terus menungu apabila tida data yang masuk ke buffer
uint8_t getchar1(void)
{   uint8_t data;
    while(rx_counter1==0);  // menunggu ada data serial masuk ke dalam buffer
    data=rx_buffer1[rx_rd_index1];
    if (++rx_rd_index1 == RX_BUFFER_SIZE1) rx_rd_index1=0;
    ClrGlobalInterrupt; --rx_counter1;  SetGlobalInterrupt;
    return data;
}

// Write a character to the USART1 Transmitter
void putchar1(int8_t c)
{   while ((UCSR1A & DATA_REGISTER_EMPTY)==0);  // menunggu buffer serial kosong
    UDR1=c;
}

// fungsi untuk mengirimkan data karakter bilangan bulat positif 3 digit dengan panjang tetap
void KirimAngka3u(uint16_t x)
{   putchar1(x/100 + 48);               // kirim ratusan
    putchar1((uint8_t)x%100/10 + 48);   // kirim puluhan
    putchar1((uint8_t)x%10 + 48);       // kirim satuan
}

// fungsi untuk mengirimkan data karakter bilangan bulat 3 digit dengan panjang tetap dengan tanda (+/-)
void KirimAngka3s(int16_t x)
{   if(x<0)
    {   putchar1('-');   x=-x;   }
    else
    {   putchar1('+');           }
    KirimAngka3u(x);
}

// fungsi untuk mengirimkan data karakter bilangan bulat positif 4 digit dengan panjang tetap
void KirimAngka4u(int16_t x)
{   putchar1(x/1000 + 48);      // kirim ribuan
    KirimAngka3u(x%1000);
}

// fungsi untuk mengirimkan data karakter bilangan bulat 4 digit dengan panjang tetap dengan tanda (+/-)
void KirimAngka4s(int16_t x)
{   if(x<0)
    {   putchar1('-');   x=-x;   }
    else
    {   putchar1('+');           }
    KirimAngka4u(x);
}

// fungsi untuk mengirimkan data karakter bilangan bulat positif 5 digit dengan panjang tetap
void KirimAngka5u(uint16_t x)
{   putchar1(x/10000 + 48);      // kirim puluhan ribuan
    KirimAngka4u(x%10000);
}

// fungsi untuk mengirimkan data karakter bilangan bulat 5 digit dengan panjang tetap dengan tanda (+/-)
void KirimAngka5s(int16_t x)
{   if(x<0)
    {   putchar1('-');   x=-x;   }
    else
    {   putchar1('+');           }
    KirimAngka5u(x);
}


#endif // MYUART_H_INCLUDED
