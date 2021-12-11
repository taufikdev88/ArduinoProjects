

// Deklarasi PIN MCU
#define LCD_EN        PORTB.2
#define LCD_RS        PORTB.3
#define LCD_PORT    PORTB

// deklarasi tipe data (jika sudah pernah bisa dihapus) 
#define int8_t signed char
#define uint8_t unsigned char
#define int16_t signed short
#define uint16_t unsigned short
        
#define DelayLCD    10        // delay animasi dalam mili second
#pragma used+
 
// Menuliskan init command pada LCD
void LCD_Init_Cmd(uint8_t xData)
{   LCD_PORT =  0B00000100 | (xData & 0xF0);
    delay_us(1);        LCD_EN = 0;
}

// Penulisan command (secara umum) ke LCD
void LCD_Perintah(uint8_t xData)
{   //LCD_RS=0;   LCD_RW=0;   LCD_EN=1;       
    LCD_RS=0;
    LCD_PORT =  0B00000100 | (xData & 0xF0);
    delay_us(1);     LCD_EN = 0;
    delay_us(5);
    LCD_PORT =  0B00000100 | (xData << 4);
    delay_us(1);     LCD_EN = 0;
    delay_us(700);  
    LCD_EN = 1;
}

// Menuliskan data ke LCD
void LCD_Data(uint8_t xData)
{   //LCD_RS=1;   LCD_RW=0;   LCD_EN=1;
    LCD_RS=1;
    LCD_PORT =  0B00001100 | (xData & 0xF0);
    delay_us(1);     LCD_EN = 0;
    delay_us(5);
    LCD_PORT =  0B00001100 | (xData << 4);
    delay_us(1);     LCD_EN = 0;
    delay_us(40);
    LCD_EN = 1;
}
 
// Inisialisasi LCD (mode 4 bit data)
void LCD_Init(void)
{   
    delay_ms(50);   LCD_Init_Cmd(0x30);
    delay_ms(5);    LCD_Init_Cmd(0x30);
    delay_ms(1);    LCD_Init_Cmd(0x30);
    delay_ms(1);    LCD_Init_Cmd(0x20);
    LCD_Perintah(0x28);         //4-bit/2-line
    LCD_Perintah(0x10);         // Set cursor
    LCD_Perintah(0x0c);         // Display ON; Cursor off
    LCD_Perintah(0x06);         // Entry mode =increment, no shift
    LCD_Perintah(0x01);         // Hapus layer    
    delay_ms(1);     
}

// Program penulisan pengaturan cursor LCD
void LCD_GotoXY(uint8_t x, uint8_t y)
{   uint8_t baris;
    if (y==0) LCD_Perintah(0x80 + x);              // baris 1 --> y=0
    else      LCD_Perintah(0x80 + x + 0x40);       // baris 2 --> y=1 
}
 
// Program penulisan text pada LCD dari flash
void LCD_TextF(uint8_t flash *text)            // menuliskan string ke LCD
{   while (*text!=0)
    { LCD_Data(*text);text++;}
}
 
// Program penulisan text pada LCD dari variabel
void LCD_Text(uint8_t *text)            // menuliskan string ke LCD
{   while (*text!=0)
    { LCD_Data(*text);text++;}
}
// Program penulisan text pada LCD dengan pemilihan baris
void LCD_TulisF(uint8_t Baris, uint8_t flash *text) // menuliskan string ke LCD
{   LCD_GotoXY(0,Baris);LCD_TextF(text); 
}        
// Program penulisan text pada LCD dengan pemilihan baris             
void LCD_Tulis(uint8_t Baris, uint8_t *text) // menuliskan string ke LCD
{   LCD_GotoXY(0,Baris);LCD_Text(text); 
}
// Program menghapus layar LCD
void LCD_Hapus(void)              // menghapus seluruh layar
{   LCD_Perintah(0x01); 
    delay_ms(1);    
}
 
// Program menghapus data pada baris LCD
void LCD_HapusBaris(uint8_t Baris)             // menghapus baris tabpa delay
{   uint8_t i;
    LCD_GotoXY(0,Baris);
    for(i=0;i<16;i++) LCD_Data(' ');
}
 
// Program menghapus data pada baris LCD dari sebelah kiri
void LCD_HapusKiri(uint8_t Baris)   // menghapus layar dari arah kiri dgn delay pada(0=baris 1, 1=Baris 2)
{   uint8_t i;
    LCD_GotoXY(0,Baris);
    for(i=0;i<16;i++)   { LCD_Data(' '); delay_ms(DelayLCD);}
}              
 
// Program menghapus data pada baris LCD dari sebelah kanan
void LCD_HapusKanan(uint8_t Baris)  // menghapus layar dari arah kanan dgn delay pada(0=baris 1, 1=Baris 2)
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=15;i>-1;i--)
    { LCD_Perintah (j+i);   LCD_Data(' '); delay_ms(DelayLCD);}
}

// Program menulis data pada baris LCD dari sebelah kiri
void LCD_TulisKiri(uint8_t Baris, uint8_t flash *text)    
// menuliskan string dari arah kiri dgn delay pada (0=baris 1, 1=Baris 2)
{   uint8_t i;
    LCD_GotoXY(0,Baris);
    for(i=0;i<16;i++)
    { LCD_Data(*(text+i)); delay_ms(DelayLCD);}
}                                                                                           
 
// Program menulis data pada baris LCD dari sebelah kanan
void LCD_TulisKanan(uint8_t Baris, uint8_t flash *text)    
// menuliskan string dari arah kanan dgn delay 
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=15;i>-1;i--)
    { LCD_Perintah (j+i);   LCD_Data(*(text+i)); delay_ms(DelayLCD);}
}

// Program menulis data pada baris LCD dari tengah
void LCD_TulisTengah(uint8_t Baris, uint8_t flash *text)    
// menuliskan string dari arah kanan dgn delay 
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=7;i>-1;i--)
    {   LCD_Perintah (j+i);     LCD_Data(*(text+i));    delay_ms(DelayLCD);
        LCD_Perintah (j-i+15);  LCD_Data(*(text-i+15)); delay_ms(DelayLCD);
    }
}                
 
// Program menulis data pada baris LCD dari sebelah pinggir
void LCD_TulisPinggir(uint8_t Baris, uint8_t flash *text)    
// menuliskan string dari arah kanan dgn delay 
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=0;i<8;i++)
    {   LCD_Perintah (j+i);     LCD_Data(*(text+i));    delay_ms(DelayLCD);
        LCD_Perintah (j-i+15);  LCD_Data(*(text-i+15)); delay_ms(DelayLCD);
    }
}
// Program menulis data angka pada posisi kursor LCD (lebar 4 angka desimal)
void LCD_Angka4(int16_t x)
{   if(x<0){ x*=-1;  LCD_Data('-');} 
    LCD_Data(x/1000+0x30);          // menulis ribuan
    LCD_Data((x%1000)/100+0x30);    // menulis ratusan
    LCD_Data((x%100)/10+0x30);      // menulis puluhan
    LCD_Data(x%10+0x30);            // menulis satuan
}
// Program menulis data angka pada posisi kursor LCD (lebar 3 angka desimal)
void LCD_Angka3(int16_t x)
{   if(x<0){ x*=-1;  LCD_Data('-');} 
    LCD_Data(x/100+0x30);           // menulis ratusan
    LCD_Data((x%100)/10+0x30);      // menulis puluhan
    LCD_Data(x%10+0x30);            // menulis satuan
}

// Program menulis data angka pada posisi kursor LCD (lebar 2 angka desimal)
void LCD_Angka2(int16_t x)
{   if(x<0){ x*=-1;  LCD_Data('-');} 
    LCD_Data((x%100)/10+0x30);      // menulis puluhan
    LCD_Data(x%10+0x30);            // menulis satuan
}
 
// Program menulis data angka (byte) pada posisi kursor LCD (signed)
void LCD_sByte(int8_t x)
{   if(x<0){ x*=-1;  LCD_Data('-');} 
    LCD_Data(x/100+0x30);           // menulis ratusan
    LCD_Data((x%100)/10+0x30);      // menulis puluhan
    LCD_Data(x%10+0x30);            // menulis satuan
}

// Program menulis data angka (byte) pada posisi kursor LCD (unsigned)
void LCD_uByte(uint8_t x)
{   LCD_Data(x/100+0x30);           // menulis ratusan
    LCD_Data((x%100)/10+0x30);      // menulis puluhan
    LCD_Data(x%10+0x30);            // menulis satuan
}
#pragma used-
