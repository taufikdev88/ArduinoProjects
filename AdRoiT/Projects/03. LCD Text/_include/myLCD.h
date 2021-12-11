#ifndef MYLCD_H_INCLUDED
#define MYLCD_H_INCLUDED

/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
Company : ER2C

Code    : Routines to Text LCD 16x2
*********************************************************/
// Fungsi LCD

#define DelayLCD    25

class myLCD
{   public:
        void Init(void);
        void Data(uint8_t xData);
        void GotoXY(uint8_t x, uint8_t y);
        void Text(const char *text);
        void Tulis(uint8_t Baris, const char *text);
        void Hapus(void);
        void HapusBaris(uint8_t Baris);
        void HapusKiri(uint8_t Baris);
        void HapusKanan(uint8_t Baris);
        void HapusTengah(uint8_t Baris);
        void HapusPinggir(uint8_t Baris);
        void TulisKiri(uint8_t Baris, const char *text);
        void TulisKanan(uint8_t Baris, const char *text);
        void TulisTengah(uint8_t Baris, const char *text);
        void TulisPinggir(uint8_t Baris, const char *text);
        void Angka5(int16_t x);
        void Angka5u(uint16_t x);
        void Angka4(int16_t x);
        void Angka4u(uint16_t x);
        void Angka3(int16_t x);
        void Angka3u(uint16_t x);
        void sByte(int8_t x);
        void uByte(uint8_t x);
        void Biner(uint8_t x);
        void Float(float x, uint8_t presisi);
        void BackLight(uint8_t);
    private:
        void    InitCmd(uint8_t xData);
        void    Perintah(uint8_t xData);
}lcd;       // Deklarasi global obyek Motor Roda

void myLCD::InitCmd(uint8_t xData)
{   LCD_PORT =  0B00000100 | (xData & 0xF0);
    _delay_us(1);        ClrLCD_EN;
}

void myLCD::Perintah(uint8_t xData)
{   //LCD_RS=0;   LCD_RW=0;   LCD_EN=1;
    ClrLCD_RS;
    if(myFlag.LcdBacklight)
        LCD_PORT =  0B00001100 | (xData & 0xF0);
    else
        LCD_PORT =  0B00000100 | (xData & 0xF0);
    _delay_us(1);        ClrLCD_EN;
    _delay_us(5);
    if(myFlag.LcdBacklight)
        LCD_PORT =  0B00001100 | (xData << 4);
    else
        LCD_PORT =  0B00000100 | (xData << 4);
    _delay_us(1);        ClrLCD_EN;
    _delay_us(700);
    SetLCD_EN;
}

void myLCD::Data(uint8_t xData)
{   //LCD_RS=1;   LCD_RW=0;   LCD_EN=1;
    SetLCD_RS;
    if(myFlag.LcdBacklight)
        LCD_PORT =  0B00001101 | (xData & 0xF0);
    else
        LCD_PORT =  0B00000101 | (xData & 0xF0);
    _delay_us(1);        ClrLCD_EN;
    _delay_us(5);
    if(myFlag.LcdBacklight)
        LCD_PORT =  0B00001101 | (xData << 4);
    else
        LCD_PORT =  0B00000101 | (xData << 4);
    _delay_us(1);        ClrLCD_EN;
    _delay_us(40);
    SetLCD_EN;
}

void myLCD::Init(void)
{   _delay_ms(50);   myLCD::InitCmd(0x30);
    _delay_ms(5);    myLCD::InitCmd(0x30);
    _delay_ms(1);    myLCD::InitCmd(0x30);
    _delay_ms(1);    myLCD::InitCmd(0x20);
    myLCD::Perintah(0x28);         //4-bit/2-line
    myLCD::Perintah(0x10);         // Set cursor
    myLCD::Perintah(0x06);         // Entry mode =increment, no shift
    myLCD::Perintah(0x0c);         // Display ON; Cursor off
    myLCD::Perintah(0x01);         // Hapus layar
    _delay_ms(2);
}

void myLCD::GotoXY(uint8_t x, uint8_t y)
{   if (y==0) myLCD::Perintah(0x80 + x);          // baris 1 --> y=0
    else      myLCD::Perintah(0x80 + x + 0x40);   // baris 2 --> y=1
}

void myLCD::Text(const char *text)        // menuliskan string ke LCD
{   while (*text!=0)
    { myLCD::Data(*text);text++;}
}

void myLCD::Tulis(uint8_t Baris, const char *text)        // menuliskan string ke LCD
{   myLCD::GotoXY(0,Baris);
    myLCD::Text(text);
}

void myLCD::Hapus(void)              // menghapus seluruh layar
{   myLCD::Perintah(0x01);
    _delay_ms(2);
}

void myLCD::HapusBaris(uint8_t Baris)             // menghapus baris tabpa delay
{   uint8_t i;
    myLCD::GotoXY(0,Baris);
    for(i=0;i<16;i++) myLCD::Data(' ');
}

void myLCD::HapusKiri(uint8_t Baris)     // menghapus layar dari arah kiri dgn delay pada(0=baris 1, 1=Baris 2)
{   uint8_t i;
    myLCD::GotoXY(0,Baris);
    for(i=0;i<16;i++)   { myLCD::Data(' '); _delay_ms(DelayLCD);}
}

void myLCD::HapusKanan(uint8_t Baris)    // menghapus layar dari arah kanan dgn delay pada(0=baris 1, 1=Baris 2)
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=15;i>-1;i--)
    { myLCD::Perintah (j+i);   myLCD::Data(' '); _delay_ms(DelayLCD);}
}

void myLCD::HapusTengah(uint8_t Baris)    // Menghapus LCD dari arah tengah dgn delay
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=7;i>-1;i--)
    {   myLCD::Perintah (j+i);     myLCD::Data(' ');  _delay_ms(DelayLCD);
        myLCD::Perintah (j-i+15);  myLCD::Data(' ');  _delay_ms(DelayLCD);
    }
}

void myLCD::HapusPinggir(uint8_t Baris)    // Menghapus LCD dari arah pinggir dgn delay
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=0;i<8;i++)
    {   myLCD::Perintah (j+i);     myLCD::Data(' ');  _delay_ms(DelayLCD);
        myLCD::Perintah (j-i+15);  myLCD::Data(' ');  _delay_ms(DelayLCD);
    }
}

void myLCD::TulisKiri(uint8_t Baris, const char *text)    // menuliskan string dari arah kiri dgn delay pada (0=baris 1, 1=Baris 2)
{   uint8_t i;
    myLCD::GotoXY(0,Baris);
    for(i=0;i<16;i++)
    { myLCD::Data(*(text+i)); _delay_ms(DelayLCD);}
}

void myLCD::TulisKanan(uint8_t Baris, const char *text)    // menuliskan string dari arah kanan dgn delay
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=15;i>-1;i--)
    { myLCD::Perintah (j+i);   myLCD::Data(*(text+i)); _delay_ms(DelayLCD);}
}

void myLCD::TulisTengah(uint8_t Baris, const char *text)    // menuliskan string dari arah kanan dgn delay
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=7;i>-1;i--)
    {   myLCD::Perintah (j+i);     myLCD::Data(*(text+i));    _delay_ms(DelayLCD);
        myLCD::Perintah (j-i+15);  myLCD::Data(*(text-i+15)); _delay_ms(DelayLCD);
    }
}

void myLCD::TulisPinggir(uint8_t Baris, const char *text)    // menuliskan string dari arah kanan dgn delay
{   int8_t i,j;
    j = 0x80 + 0x40*Baris;
    for(i=0;i<8;i++)
    {   myLCD::Perintah (j+i);     myLCD::Data(*(text+i));    _delay_ms(DelayLCD);
        myLCD::Perintah (j-i+15);  myLCD::Data(*(text-i+15)); _delay_ms(DelayLCD);
    }
}

void myLCD::Angka5(int16_t x)
{   uint8_t sign=1;
    if(x<0){ x=-x;  myLCD::Data('-');sign=0;}
    myLCD::Angka5u(x);
    if(sign)lcd.Data(' ');
}

void myLCD::Angka5u(uint16_t x)
{   myLCD::Data(x/10000+0x30);         // menulis puluhan ribu
    myLCD::Angka4u(x%10000);
}

void myLCD::Angka4(int16_t x)
{   uint8_t sign=1;
    if(x<0){ x=-x;  myLCD::Data('-');sign=0;}
    myLCD::Angka4u(x%10000);
    if(sign)lcd.Data(' ');
}

void myLCD::Angka4u(uint16_t x)
{   myLCD::Data(x/1000+0x30);          // menulis ribuan
    myLCD::Angka3u(x%1000);
}

void myLCD::Angka3(int16_t x)
{   uint8_t sign=1;
    if(x<0){ x=-x;  myLCD::Data('-');sign=0;}
    myLCD::Angka3u(x%1000);
    if(sign)lcd.Data(' ');
}

void myLCD::Angka3u(uint16_t x)
{   myLCD::Data(x/100+0x30);           // menulis ratusan
    myLCD::Data((x%100)/10+0x30);      // menulis puluhan
    myLCD::Data(x%10+0x30);            // menulis satuan
}

void myLCD::sByte(int8_t x)
{   uint8_t sign=1;
    if(x<0){ x=-x;  myLCD::Data('-');sign=0;}
    myLCD::uByte(x);
    if(sign)lcd.Data(' ');
}

void myLCD::uByte(uint8_t x)
{   myLCD::Data(x/100+0x30);           // menulis ratusan
    myLCD::Data((x%100)/10+0x30);      // menulis puluhan
    myLCD::Data(x%10+0x30);            // menulis satuan
}

void myLCD::Float(float x, uint8_t Presisi)
{   uint8_t sign=1;
    int16_t y;
    y = (int16_t) x;
    if (x<0)
    {   y = -y;
        x = -x;
        lcd.Data('-');
        sign = 0;
    }
    if (y>=10000)       Angka5u(y);
    else if (y>=1000)   Angka4u(y);
    else if (y>=100)    Angka3u(y);
    else                uByte(y);
    // menampilkan angka dibelakang koma
    if (Presisi>0 && Presisi<=5)
    {   lcd.Data('.');
        x -= y;
        switch(Presisi)
        {   case 5: Angka5u(x*100000);  break;
            case 4: Angka4u(x*10000);   break;
            case 3: Angka3u(x*1000);    break;
            case 2: Presisi = (uint8_t)(x*100);
                    lcd.Data(Presisi/10+48);
                    lcd.Data(Presisi%10+48);
                    break;
            case 1: Presisi = (uint8_t)(x*10);
                    lcd.Data(Presisi+48);
                    break;
        }
    }
    if(sign)lcd.Data(' ');
}

void myLCD::Biner(uint8_t x)
{   unsigned char i;
    for(i=0;i<8;i++)
    {   if( (x&(1<<(7-i)))==0 )
        {myLCD::Data('0');}
        else
        {myLCD::Data('1');}
    }
}

void myLCD::BackLight(uint8_t Status)
{
    if(Status)
    {   myFlag.LcdBacklight = 1;
        SetLCD_BL;
    }
    else
    {   myFlag.LcdBacklight = 0;
        ClrLCD_BL;
    }
}
#endif // MYLCD_H_INCLUDED
