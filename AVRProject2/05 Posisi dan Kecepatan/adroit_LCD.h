#ifndef _myLCD_
#define _myLCD_

// Fungsi LCD
#include "adroit.h"
#include <delay.h>
#define DelayLCD    50    // nilai delay untuk penulisan atau penghapusan LCD

#pragma used+
// sub fungsi pengiriman command ke LCD
void LCD_Init_Cmd(uint8_t xData)
{   LCD_PORT =  0B00000100 | (xData & 0xF0);
    delay_us(1);        LCD_EN(0);
}

// sub fungsi penulisan 1 byte perintah ke LCD
void LCD_Perintah(uint8_t xData)
{   //LCD_RS=0;   LCD_RW=0;   LCD_EN=1;       
    LCD_RS(0);
    LCD_PORT =  0B00001100 | (xData & 0xF0);
    delay_us(1);     LCD_EN(0);
    delay_us(5);
    LCD_PORT =  0B00001100 | (xData << 4);
    delay_us(1);     LCD_EN(0);
    delay_us(700);  
    LCD_EN(1);
}

// sub fungsi penulisan 1 byte data ke LCD
void LCD_Data(uint8_t xData)
{   //LCD_RS=1;   LCD_RW=0;   LCD_EN=1;
    LCD_RS(1);
    LCD_PORT =  0B00001101 | (xData & 0xF0);
    delay_us(1);     LCD_EN(0);
    delay_us(5);
    LCD_PORT =  0B00001101 | (xData << 4);
    delay_us(1);     LCD_EN(0);
    delay_us(40);
    LCD_EN(1);
}

// sub fungsi inisialisasi LCD dengan data transfer 4 bit
void LCD_Init(void)
{   delay_ms(50);   LCD_Init_Cmd(0x30);
    delay_ms(5);    LCD_Init_Cmd(0x30);
    delay_ms(1);    LCD_Init_Cmd(0x30);
    delay_ms(1);    LCD_Init_Cmd(0x20);
    LCD_Perintah(0x28);         //4-bit/2-line
    LCD_Perintah(0x10);         // Set cursor
    LCD_Perintah(0x0c);         // Display ON; Cursor off
    LCD_Perintah(0x06);         // Entry mode =increment, no shift
    LCD_Perintah(0x01);         // Hapus layar
}            

// fungi untuk mengatur posisi kursor dari LCD
void LCD_GotoXY(uint8_t x, uint8_t y)
{   uint8_t baris;
    if (y==0) LCD_Perintah(0x80 + x);          // baris 1 --> y=0
    else      LCD_Perintah(0x80 + x + 0x40);   // baris 2 --> y=1 
}

// fungi untuk menulis text yang disimpan ke ROM
void LCD_TextF(uint8_t flash *text)
{   while (*text!=0)
    { LCD_Data(*text);text++;}
}

// fungi untuk menulis text yang disimpan di RAM
void LCD_Text(uint8_t *text) 
{   while (*text!=0)
    { LCD_Data(*text);text++;}
}

// fungi untuk menulis text yang disimpan ke ROM ke LCD baris ke(0=baris 1, 1=Baris 2)
void LCD_TulisF(uint8_t Baris, uint8_t flash *text) 
{   LCD_GotoXY(0,Baris);LCD_TextF(text); 
}

// fungsi untuk menghapus layar LCD
void LCD_Hapus(void)              // menghapus seluruh layar
{   LCD_Perintah(0x01);
    delay_us(1);
}

// fungsi untuk menghapus LCD pada baris ke (0=baris 1, 1=Baris 2) secara langsung
void LCD_HapusBaris(uint8_t Baris)             // menghapus baris tanpa delay
{   uint8_t i;
    LCD_GotoXY(0,Baris);
    for(i=0;i<16;i++) LCD_Data(' ');
}

// fungsi untuk menghapus LCD pada baris ke (0=baris 1, 1=Baris 2) dari kiri layar
void LCD_HapusKiri(uint8_t Baris)     
{   uint8_t i;
    LCD_GotoXY(0,Baris);
    for(i=0;i<16;i++)   { LCD_Data(' '); delay_ms(DelayLCD);}
}              

// fungsi untuk menghapus LCD pada baris ke (0=baris 1, 1=Baris 2) dari kanan layar
void LCD_HapusKanan(uint8_t Baris)
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=15;i>-1;i--)
    { LCD_Perintah (j+i);   LCD_Data(' '); delay_ms(DelayLCD);}
}                

// menuliskan string (16 karakter)dari arah kiri dgn delay pada (0=baris 1, 1=Baris 2)
void LCD_TulisKiri(uint8_t Baris, uint8_t flash *text)   
{   uint8_t i;
    LCD_GotoXY(0,Baris);
    for(i=0;i<16;i++)
    { LCD_Data(*(text+i)); delay_ms(DelayLCD);}
}                                                                                           

// menuliskan string (16 karakter) dari arah kanan dgn delay
void LCD_TulisKanan(uint8_t Baris, uint8_t flash *text)     
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=15;i>-1;i--)
    { LCD_Perintah (j+i);   LCD_Data(*(text+i)); delay_ms(DelayLCD);}
}                

// menuliskan string (16 karakter) dari arah kanan dgn delay 
void LCD_TulisTengah(uint8_t Baris, uint8_t flash *text)    
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=7;i>-1;i--)
    {   LCD_Perintah (j+i);     LCD_Data(*(text+i));    delay_ms(DelayLCD);
        LCD_Perintah (j-i+15);  LCD_Data(*(text-i+15)); delay_ms(DelayLCD);
    }
}
                
// menuliskan string (16 karakter) dari arah kanan dgn delay 
void LCD_TulisPinggir(uint8_t Baris, uint8_t flash *text)    
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=0;i<8;i++)
    {   LCD_Perintah (j+i);     LCD_Data(*(text+i));    delay_ms(DelayLCD);
        LCD_Perintah (j-i+15);  LCD_Data(*(text-i+15)); delay_ms(DelayLCD);
    }
}                

// menuliskan nilai desimal dengan panjang 4 angka (-9999 sd 9999) 
void LCD_Angka4(int16_t x)
{   if(x<0){ x*=-1;  LCD_Data('-');} 
    LCD_Data(x/1000+0x30);          // menulis ribuan
    LCD_Data((x%1000)/100+0x30);    // menulis ratusan
    LCD_Data((x%100)/10+0x30);      // menulis puluhan
    LCD_Data(x%10+0x30);            // menulis satuan
}

// menuliskan nilai desimal dengan panjang 4 angka (-999 sd 999) 
void LCD_Angka3(int16_t x)
{   if(x<0){ x*=-1;  LCD_Data('-');} 
    LCD_Data(x/100+0x30);           // menulis ratusan
    LCD_Data((x%100)/10+0x30);      // menulis puluhan
    LCD_Data(x%10+0x30);            // menulis satuan
}// 

// menuliskan nilai desimal dengan jangkauan (-128 sd 127) 
void LCD_sByte(int8_t x)
{   if(x<0){ x*=-1;  LCD_Data('-');} 
    LCD_Data(x/100+0x30);           // menulis ratusan
    LCD_Data((x%100)/10+0x30);      // menulis puluhan
    LCD_Data(x%10+0x30);            // menulis satuan
}

// menuliskan nilai desimal positif dengan jangkauan (0 sd 255) 
void LCD_uByte(uint8_t x)
{   LCD_Data(x/100+0x30);           // menulis ratusan
    LCD_Data((x%100)/10+0x30);      // menulis puluhan
    LCD_Data(x%10+0x30);            // menulis satuan
}

// menuliskan nilai biner dari sebuah byte data (data 8 bit) 
void LCD_Biner(uint8_t x)
{   unsigned char i;
    for(i=0;i<8;i++) 
    {   if( (x&(1<<(7-i)))==0 ) 
        {LCD_Data('0');}
        else
        {LCD_Data('1');}
    }
}
#pragma used-                    
#endif