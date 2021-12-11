#ifndef _Definisi_
#define _Definisi_

#define uint8_t     unsigned char
#define int8_t      signed char
#define uint16_t    unsigned short
#define int16_t     signed short

#define ON          1
#define OFF         0
// Definisi Port LCD
#define LCD_PORT        PORTA
#define SetLCD_RS       (PORTA.0 = 1)
#define ClrLCD_RS       (PORTA.0 = 0)
#define LCD_RS(o)       (o ? SetLCD_RS:ClrLCD_RS)

#define SetLCD_RW       (PORTA.1 = 1)
#define ClrLCD_RW       (PORTA.0 = 0)
#define LCD_RW(o)       (o ? SetLCD_RW:ClrLCD_RW)

#define SetLCD_EN       (PORTA.2 = 1)
#define ClrLCD_EN       (PORTA.2 = 0)
#define LCD_EN(o)       (o ? SetLCD_EN:ClrLCD_EN)

#define SetLCD_BL       (PORTA.3 = 1)
#define ClrLCD_BL       (PORTA.3 = 0)
#define LCD_BL(o)       (o ? SetLCD_BL:ClrLCD_BL)

// Definisi Port Servo
#define SetP_S ervo1     (PORTD.7 = 1)
#define ClrP_Servo1     (PORTD.7 = 0)
#define P_Servo1(o)     (o ? SetP_Servo1:ClrP_Servo1)

#define SetP_Servo2     (PORTD.6 = 1)
#define ClrP_Servo2     (PORTD.6 = 0)
#define P_Servo2(o)     (o ? SetP_Servo2:ClrP_Servo2)

#define SetP_Servo3     (PORTD.5 = 1)
#define ClrP_Servo3     (PORTD.5 = 0)
#define P_Servo3(o)     (o ? SetP_Servo3:ClrP_Servo3)

#define SetP_Servo4     (PORTD.4 = 1)
#define ClrP_Servo4     (PORTD.4 = 0)
#define P_Servo4(o)     (o ? SetP_Servo4:ClrP_Servo4)

#define SetP_Servo5     (PORTB.3 = 1)
#define ClrP_Servo5     (PORTB.3 = 0)
#define P_Servo5(o)     (o ? SetP_Servo5:ClrP_Servo5)

#define SetP_Servo6     (PORTB.5 = 1)
#define ClrP_Servo6     (PORTB.5 = 0)
#define P_Servo6(o)      (o ? SetP_Servo6:ClrP_Servo6)

#define SetP_Servo7     (PORTB.6 = 1)
#define ClrP_Servo7     (PORTB.6 = 0)
#define P_Servo7(o)     (o ? SetP_Servo7:ClrP_Servo7)

#define SetP_Servo8     (PORTB.7 = 1)
#define ClrP_Servo8     (PORTB.7 = 0)
#define P_Servo8(o)     (o ? SetP_Servo8:ClrP_Servo8)

#define dServo6 OCR1A
#define dServo7 OCR1B
#define dServo8 OCR1C

// Definisi Port Push Button
#define Tombol1         ((PINC&8)==0 ? 1:0)
#define Tombol2         ((PINC&4)==0 ? 1:0)
#define Tombol3         ((PINC&2)==0 ? 1:0)
#define Tombol4         ((PINC&1)==0 ? 1:0)

// Definisi LED  --> SetLEDx = menyalakan LEDx, ClrLEDx = mematikan LEDx
#define SetLED1         (PORTC.4 = 1)
#define ClrLED1         (PORTC.4 = 0)
#define SetLED2         (PORTC.5 = 1)
#define ClrLED2         (PORTC.5 = 0)
#define SetLED3         (PORTC.6 = 1)
#define ClrLED3         (PORTC.6 = 0)
#define SetLED4         (PORTC.7 = 1)
#define ClrLED4         (PORTC.7 = 0)

// menyalakan atau mematikan LED (o=1=On, o=0=Off)
#define LED1(o)         (o ? SetLED1:ClrLED1)
#define LED2(o)         (o ? SetLED2:ClrLED2)
#define LED3(o)         (o ? SetLED3:ClrLED3)
#define LED4(o)         (o ? SetLED4:ClrLED4)

// Definisi Port Motor kiri
#define PwmMKi(o)       OCR3BH = ((unsigned char)(o>>8)); OCR3BL = (unsigned char)o;  
#define SetDirMKi       (PORTE.5 = 1)
#define ClrDirMKi       (PORTE.5 = 0)
#define DirMKi(o)       (o ? SetDirMKi:ClrDirMKi)
// Definisi Port Motor kanan
#define PwmMKa(o)       OCR3AH = ((unsigned char)(o>>8)); OCR3AL = (unsigned char)o;  
#define SetDirMKa       (PORTE.2 = 1)
#define ClrDirMKa       (PORTE.2 = 0)
#define DirMKa(o)       (o ? SetDirMKa:ClrDirMKa)
// definisi arah putaran motor roda
#define MKi_CCW          SetDirMKi
#define MKi_CW           ClrDirMKi
#define MKa_CW           SetDirMKa
#define MKa_CCW          ClrDirMKa

// Definisi Port Enkoder Motor
#define Set_P_Enkoder1A (PORTE. 6 = 1)                                //PORTE.6 INT6
#define Clr_P_Enkoder1A (PORTE. 6 = 0)
#define _P_Enkoder1A(o) (o ? Set_P_Enkoder1A:Clr_P_Enkoder1A)
#define P_Enkoder1A     ((PINE.6)==1 ?  1:0)

#define Set_P_Enkoder1B (PORTB.0 = 1)                                 //PINB.0
#define Clr_P_Enkoder1B (PORTB.0 = 0)
#define _P_Enkoder1B(o) (o ? Set_P_Enkoder1B:Clr_P_Enkoder1B)
#define P_Enkoder1B     ((PINB.0)==1 ?  1:0)

#define Set_P_Enkoder2A (PORTE.7 = 1)                                 //PORTE.7 INT7
#define Clr_P_Enkoder2A (PORTE.7 = 0)
#define _P_Enkoder2A(o) (o ? Set_P_Enkoder2A:Clr_P_Enkoder2A)
#define P_Enkoder2A     ((PINE.7)==1 ?  1:0)

#define Set_P_Enkoder2B (PORTB.2 = 1)                                 //PINB.2
#define Clr_P_Enkoder2B (PORTB.2 = 0)
#define _P_Enkoder2B(o) (o ? Set_P_Enkoder2B:Clr_P_Enkoder2B)
#define P_Enkoder2B     ((PINB.2)==1 ?  1:0)

// Definisi Port Mode (dipswitch)
#define MODE            ((~PING) & 0B00001111)
#define MODE1           ((PING & 8) ==0 ? 1:0)
#define MODE2           ((PING & 4) ==0 ? 1:0)
#define MODE3           ((PING & 2) ==0 ? 1:0)
#define MODE4           ((PING & 1) ==0 ? 1:0)
// DipSwitchx adalah alias dari MODEx
#define DipSwitch       MODE
#define DipSwitch1      MODE1
#define DipSwitch2      MODE2
#define DipSwitch3      MODE3
#define DipSwitch4      MODE4

// sudut awal dari masing-masing motor servo (0-180 derajat)
#define SudutAwalServo1     90
#define SudutAwalServo2     90
#define SudutAwalServo3     90
#define SudutAwalServo4     90
#define SudutAwalServo5     90
#define SudutAwalServo6     90
#define SudutAwalServo7     90
#define SudutAwalServo8     90

// Definisi Nada Buzzer
#define C4 261
#define CS4 277
#define D4 293
#define DS4 311
#define E4 329
#define F4 349
#define FS4 370
#define G4 392
#define GS4 415
#define A4 440
#define AS4 466
#define B4 494

#define C5 523
#define CS5 554
#define D5 587
#define DS5 622
#define E5 659
#define F5 698
#define FS5 740
#define G5 783
#define GS5 830
#define A5 880
#define AS5 932
#define B5 987

#define C6 1046
#define CS6 1109
#define D6 1174
#define DS6 1244
#define E6 1318
#define F6 1370
#define FS6 1480
#define G6 1568
#define GS6 1661
#define A6 1760
#define AS6 1864
#define B6 1975

#define C7 2093
#define CS7 2217
#define D7 2349
#define DS7 2489
#define E7 2639
#define F7 2793
#define FS7 2960
#define G7 3136
#define GS7 3322
#define A7 3520
#define AS7 3729
#define B7 3951

#endif

