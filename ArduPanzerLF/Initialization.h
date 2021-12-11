#include "Arduino.h"
#include <LiquidCrystal.h>
#include <avr/eeprom.h>

// Configuration Hardware
// Fuse Bit L=0xFF H=0xD0 E=0xFC
//#define APMLF_X9 // ArduPanzerMega Line Follower X9 Board
#define APMLF1 // ArduPanzerMega Line Follower V1 Board
//#define APMLF2 // ArduPanzerMega Line Follower V2 Board

//#define SENSOR12
#define SENSOR14

#define hack 0
// ACTIVATION
unsigned char active[30],active_ok,active_value=200,user_active_value=50;
const char mypassword[]="ardukampretfkadklfnkdjbfkj#";
const char mypassword1[]="userpanzerrobotics";
const char mypassword2[]="daruratuserpanzerrobotics";

#if defined (SENSOR12)
const int sensor_speed=30;
#endif
#if defined (SENSOR14)
const int sensor_speed=30;
#endif


#define LCD_BAR1
//#define LCD_BAR2

#if defined (APMLF_X9)
const char rs=5,e=6,d4=10,d5=11,d6=12,d7=13;
#endif
#if defined (APMLF1) || defined (APMLF2)
const char rs=4,e=5,d4=6,d5=7,d6=0,d7=1;
#endif

#if defined (APMLF_X9)
#define LPWM1 OCR1BL
#define RPWM1 OCR1AL
#endif 
#if defined (APMLF1) || defined (APMLF2)
#define LPWM1 OCR1BL
#define LPWM2 OCR1AL
#define RPWM1 OCR2B
#define RPWM2 OCR2A
#endif

LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

void output_setup();
void input_setup();
char sw1();
char sw2();
char sw3();
char sw4();
char sw5();
void lsw(char on);
void rsw(char on);
void led_lcd(char on,char mirror);
void led_timer(char on);
unsigned char read_adc(unsigned char adc_input);
void adc_init();
void timer_init();
void out_pid();
void clear_pid();
void pwm_on();
void pwm_off();
void set_motors(int LPWM,int RPWM,int MAXPWM,int BALANCEL,int BALANCER);
void lcd_bar_init();
void lcd_bar(char input,char x,char y);
unsigned int sensor10bar();
unsigned int sensor12bar();
unsigned int sensor14bar();
unsigned int sensor10(char mirror);
unsigned int sensor12(char mirror);
unsigned int sensor14(char mirror);
int error_out(unsigned int input_error);
int line_center(unsigned int input_error);
int line_left(unsigned int input_error);
int line_right(unsigned int input_error);
int sensorfix(unsigned char m_line);
unsigned char eeprom_read(unsigned int address);
void eeprom_write(unsigned int address,unsigned char value);
void eeprom_init();
void save_activation();
void save_sensor();
void save_recent_read();
void save_recent_write();
void save_pid();
void save_modenormal();
void save_readpoint();
void save_timerpoint();
void save_kpcount();
void save_counter();
void save_timer();
void save_delaycount();
void save_stopcount();
void save_vcountl();
void save_vcountr();
void save_maxpwm();
void save_readcount();
void save_linecount();
void save_lineselect();
void save_vcount();
void save_vcount2();
void save_speed1();
void save_speed2();
void save_sdmemory();
void read_activation();
void read_sensor();
void read_recent_read();
void read_recent_write();
void read_pid();
void read_modenormal();
void read_readpoint();
void read_timerpoint();
void read_kpcount();
void read_counter();
void read_timer();
void read_delaycount();
void read_stopcount();
void read_vcountl();
void read_vcountr();
void read_maxpwm();
void read_readcount();
void read_linecount();
void read_lineselect();
void read_vcount();
void read_vcount2();
void read_speed1();
void read_speed2();
void read_sdmemory();
void setting_default();
void sensor_calibrate();
void auto_set_vc();
void setting_pid();
void setting_normal();
void setting_readpoint();
void setting_timerpoint();
void setting_kpcount();
void setting_counter();
void setting_timer();
void setting_delaycount();
void setting_stopcount();
void setting_vcountlr();
void setting_maxpwm();
void setting_readcount();
void setting_linecount();
void setting_lineselect();
void setting_vcount();
void setting_speed1();
void setting_speed2();
void setting_coppyprogram();
void setting_insert();
void setting_delete();
void setting_mirror();
void serial_off();
void setting_activation();
void activation();
void setting_advance();
void idle();
void lcd_clear(char time);
void mode_stop();
void mode_free();
void mode_left();
void mode_right();
void mode_counter();
void output();
void setting_ceckpoint();
void mydelay(unsigned int mytime);
void setting_cp();
void setting_writefile();
void format_dir();
void sd_init();
void clear_mybuf();
void scanfile(String typefile);
void setting_readfile();

// OUTPUT SETUP
char lpwm1,lpwm2,rpwm1,rpwm2,lsensor,rsensor,ledlcd,ledtimer,relay; // UNO/APMLF
char ldir1,ldir2,rdir1,rdir2; //X9

// INPUT SETUP
char s1,s2,s3,s4,s5,analog;

// PID 
int kp,ki,kd,imax,out_p,out_i,out_d;
int integral,error,last_error,pv,pv_in,LOUT,ROUT,rot,lot;
const unsigned char ti=100;
int sp=0,pv_out;

//SENSOR
unsigned int out_sensor;
unsigned char sensor[16],sensor1,sensor2,sensor3,sensor4,sensor5,sensor11,sensor22,sensor33,sensor44,sensor55;
unsigned char invers;
int linemode,monitor=0;
unsigned char read1x=0;

// LCD CONTROL
char lcdstatus,lcdinvert;

// RUN
int mode,pwm,timer_speed,allmaxpwm,triml,trimr,run_counter=0,run_select=0,run_timer=0,run_delay=0,en_timer=0,en_delay=0,c_lock=0,run_mirror;
int speed_setting,speed_setting1,fast,lcdtime=0,clock_timer=0,clock_delay=0,lost_code=0;
int run_cp=0,lock_count=0,memo_l=0,memo_r=0;
const int loading=300,p_delay=1;
int recent_read,recent_write;
// MODE COUNTER
#if defined (APMLF_X9) || defined (APMLF1) || defined (APMLF2)
const unsigned char select=1,count=200,cpcount=30,sdselect=16,sdcount=14;
const char *sd_def="PANZER.TXT";
const char *sd_empaty="Empty!";
#endif
unsigned char kpcount[select][count],counter[select][count],timer[select][count],delaycount[select][count];
unsigned char stopcount[select][count],vcountl[select][count],vcountr[select][count],maxpwm[select][count]; 
unsigned char readcount[select][count],linecount[select][count],lineselect[select][count],vcount[select][count],vcount2[select][count]; 
unsigned char readpoint[select][cpcount],timerpoint[select][cpcount],speed1[select][count],speed2[select][count];
unsigned char sdmemory[sdselect][sdcount];

// EEPROM ADRRESS
int kpcount_add,counter_add,timer_add,delaycount_add;
int stopcount_add,vcountl_add,vcountr_add,maxpwm_add; 
int readcount_add,linecount_add,lineselect_add,vcount_add,vcount2_add; 
int speed1_add,speed2_add;
int start_address=450;

#if defined (LCD_BAR1)
// LCD BAR
byte chr0[8]={      
32,32,32,32,32,32,32,31};
byte chr1[8]={      
32,32,32,32,32,32,31,31};
byte chr2[8]={      
32,32,32,32,32,31,31,31};
byte chr3[8]={      
32,32,32,32,31,31,31,31};
byte chr4[8]={      
32,32,32,31,31,31,31,31};
byte chr5[8]={      
32,32,31,31,31,31,31,31};
byte chr6[8]={      
32,31,31,31,31,31,31,31};
byte chr7[8]={      
31,31,31,31,31,31,31,31};
#endif

#if defined (LCD_BAR2)
// LCD BAR
byte chr0[8]={      
32,32,32,32,32,32,32,31};
byte chr1[8]={      
32,32,32,32,32,32,31,32};
byte chr2[8]={      
32,32,32,32,32,31,32,32};
byte chr3[8]={      
32,32,32,32,31,32,32,32};
byte chr4[8]={      
32,32,32,31,32,32,32,32};
byte chr5[8]={      
32,32,31,32,32,32,32,32};
byte chr6[8]={      
32,31,32,32,32,32,32,32};
byte chr7[8]={      
31,32,32,32,32,32,32,32};
#endif

ISR(TIMER0_COMPA_vect) {
  lcdtime++;
  if(mode==1&&en_timer==1&&run_timer<timer[run_select][run_counter]){
    clock_timer++;
    if(clock_timer>=timer_speed) {
      run_timer++;clock_timer=0;  
    }
    led_timer(1);
  }
  if(mode==1&&en_timer==1&&run_timer>=timer[run_select][run_counter]) led_timer(0);
  if(mode==1&&en_delay==1&&run_delay<delaycount[run_select][run_counter-1]) {
    clock_delay++;
    if(clock_delay>=timer_speed)  {
      run_delay++;
      clock_delay=0; 
    }
    led_timer(1);
  }
  if(mode==1&&en_delay==1&&run_delay>=delaycount[run_select][run_counter-1]) led_timer(0);
}

void output_setup() {
  adc_init();
  #if defined (APMLF_X9)
  ldir1=2;
  ldir2=4;
  rdir1=3;
  rdir2=9;
  lpwm1=30;
  rpwm1=8;
  ledlcd=7;
  ledtimer=31;
  lsensor=28;
  rsensor=29;
  relay=1;
  pinMode(lpwm1, OUTPUT);
  pinMode(rpwm1, OUTPUT);
  pinMode(ldir1, OUTPUT); 
  pinMode(ldir2, OUTPUT); 
  pinMode(rdir1, OUTPUT); 
  pinMode(rdir2, OUTPUT); 
  pinMode(ledlcd, OUTPUT); 
  pinMode(ledtimer, OUTPUT); 
  pinMode(lsensor, OUTPUT);
  pinMode(rsensor, OUTPUT); 
  pinMode(relay, OUTPUT); 
  digitalWrite(lpwm1, LOW); 
  digitalWrite(rpwm1, LOW); 
  digitalWrite(ldir1, LOW); 
  digitalWrite(ldir2, LOW); 
  digitalWrite(rdir1, LOW); 
  digitalWrite(rdir2, LOW); 
  digitalWrite(ledlcd, LOW); 
  digitalWrite(ledtimer, HIGH); 
  digitalWrite(lsensor, LOW);
  digitalWrite(rsensor, LOW);
  digitalWrite(relay, LOW);
  #endif

  #if defined (APMLF1)
  lpwm1=30;
  lpwm2=8;
  rpwm1=9;
  rpwm2=31;
  ledlcd=2;
  ledtimer=27;
  lsensor=28;
  rsensor=29;
  relay=3;
  pinMode(lpwm1, OUTPUT); 
  pinMode(lpwm2, OUTPUT); 
  pinMode(rpwm1, OUTPUT); 
  pinMode(rpwm2, OUTPUT); 
  pinMode(ledlcd, OUTPUT); 
  pinMode(ledtimer, OUTPUT); 
  pinMode(lsensor, OUTPUT);
  pinMode(rsensor, OUTPUT);
  pinMode(relay, OUTPUT); 
  digitalWrite(lpwm1, LOW); 
  digitalWrite(lpwm2, LOW); 
  digitalWrite(rpwm1, LOW); 
  digitalWrite(rpwm2, LOW); 
  digitalWrite(ledlcd, LOW); 
  digitalWrite(ledtimer, LOW); 
  digitalWrite(lsensor, LOW);
  digitalWrite(rsensor, LOW); 
  digitalWrite(relay, LOW);
  #endif

  #if defined (APMLF2)
  lpwm1=30;
  lpwm2=8;
  rpwm1=9;
  rpwm2=31;
  ledlcd=2;
  ledtimer=27;
  lsensor=23;
  rsensor=22;
  relay=3;
  pinMode(lpwm1, OUTPUT); 
  pinMode(lpwm2, OUTPUT); 
  pinMode(rpwm1, OUTPUT); 
  pinMode(rpwm2, OUTPUT); 
  pinMode(ledlcd, OUTPUT); 
  pinMode(ledtimer, OUTPUT); 
  pinMode(lsensor, OUTPUT);
  pinMode(rsensor, OUTPUT); 
  pinMode(relay, OUTPUT); 
  digitalWrite(lpwm1, LOW); 
  digitalWrite(lpwm2, LOW); 
  digitalWrite(rpwm1, LOW); 
  digitalWrite(rpwm2, LOW); 
  digitalWrite(ledlcd, LOW); 
  digitalWrite(ledtimer, LOW); 
  digitalWrite(lsensor, LOW);
  digitalWrite(rsensor, LOW); 
  digitalWrite(relay, LOW);
  #endif
}

void input_setup() {
  #if defined (APMLF_X9) || defined (APMLF1)
  s1=22;
  s2=23;
  s3=24;
  s4=25;
  s5=26;
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);
  digitalWrite(s1, HIGH);
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  digitalWrite(s4, HIGH);
  digitalWrite(s5, HIGH);
  #endif

  #if defined (APMLF2)
  s1=28;
  s2=29;
  s3=24;
  s4=25;
  s5=26;
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);
  digitalWrite(s1, HIGH);
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  digitalWrite(s4, HIGH);
  digitalWrite(s5, HIGH);
  #endif
}

char sw1() {
  char data=1;
  if(digitalRead(s1)==0) data=0;
  else data=1;
  return data;
}

char sw2() {
  char data=1;
  if(digitalRead(s2)==0) data=0;
  else data=1;
  return data;
}

char sw3() {
  char data=1;
  if(digitalRead(s3)==0) data=0;
  else data=1;
  return data;
}


char sw4() {
  char data=1;
  if(digitalRead(s4)==0) data=0;
  else data=1;
  return data;
}

char sw5() {
  char data=1;
  if(digitalRead(s5)==0) data=0;
  else data=1;
  return data;
}

void lsw(char on) {
  if(on==1) digitalWrite(lsensor, HIGH);  
  else digitalWrite(lsensor, LOW);  
}

void rsw(char on) {
  if(on==1) digitalWrite(rsensor, HIGH);  
  else digitalWrite(rsensor, LOW);  
}

void led_lcd(char on,char mirror){
  if(mirror==0) {
    if(on==0) digitalWrite(ledlcd, LOW);
    else digitalWrite(ledlcd, HIGH);
  } else {
    if(on==0)digitalWrite(ledlcd, HIGH);
    else digitalWrite(ledlcd, LOW);  
  }
}

void led_timer(char on) {
  #if defined (APMLF_X9)
  if(on==0) digitalWrite(ledtimer, HIGH); 
  else digitalWrite(ledtimer, LOW); 
  #endif

  #if defined (APMLF1) || defined (APMLF2)
  if(on==0) digitalWrite(ledtimer, LOW); 
  else digitalWrite(ledtimer, HIGH); 
  #endif
}

#define ADC_VREF_TYPE 0x60
unsigned char read_adc(unsigned char adc_input) {
  ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
  delayMicroseconds(10);
  ADCSRA|=0x40;
  while ((ADCSRA & 0x10)==0);
  ADCSRA|=0x10;
  return ADCH;
}

void adc_init() {
  DIDR0=0x00;
  ADMUX=ADC_VREF_TYPE & 0xff;
  ADCSRA=0x84; 
}

void timer_init() {
  TCCR0B=0x05;
  OCR0A=0x00;
  TIMSK0=0x02; 
}

void out_pid() {
  out_p=(int)error*kp;
  out_i=(int)(integral*ki)/ti;
  if(error!=0) integral=integral+error;
  else integral=0;
  if(out_i>imax) out_i=imax;
  if(out_i<-imax) out_i=-imax;
  out_d=(int)(error-last_error)*kd;
  last_error=error;  
}

void clear_pid() {
  error=0;
  last_error=0;
  integral=0;
  pv=0;
  pv_out=0;
  out_p=0;
  out_i=0;
  out_d=0;
  pv_in=0;
  LOUT=0;
  ROUT=0;
}

void pwm_on() {
  #if defined (APMLF1) || defined (APMLF2)
  TCCR1A=0xA1;
  TCCR1B=0x03;
  TCCR2A=0xA1;
  TCCR2B=0x04;
  #endif
  #if defined (APMLF_X9)
  TCCR1A=0xA1;
  TCCR1B=0x0B;
  #endif    
}

void pwm_off() {
  #if defined (APMLF1) || defined (APMLF2)
  TCCR1A=0x00;
  TCCR1B=0x00;
  TCCR2A=0x00;
  TCCR2B=0x00;
  #endif
  #if defined (APMLF_X9)
  TCCR1A=0x00;
  TCCR1B=0x00;
  #endif
}

void set_motors(int LPWM,int RPWM,int MAXPWM,int BALANCEL,int BALANCER){
  if(BALANCEL>255) BALANCEL=255;
  if(BALANCEL<0) BALANCEL=0;
  if(BALANCER>255) BALANCER=255;
  if(BALANCER<0) BALANCER=0;
  if(LPWM>0) LPWM=LPWM+BALANCEL;
  if(LPWM<0) LPWM=LPWM+(-BALANCEL);
  if(RPWM>0) RPWM=RPWM+BALANCER;
  if(RPWM<0) RPWM=RPWM+(-BALANCER);
  if(MAXPWM>255) MAXPWM=255;
  if(MAXPWM<0) MAXPWM=0;
  if(LPWM>MAXPWM) LPWM=MAXPWM;
  if(LPWM<-MAXPWM) LPWM=-MAXPWM;
  if(RPWM>MAXPWM) RPWM=MAXPWM;
  if(RPWM<-MAXPWM) RPWM=-MAXPWM;
     
  #if defined (APMLF_X9)
  if(LPWM==0){ 
    digitalWrite(ldir1, LOW); 
    digitalWrite(ldir2, LOW);
    LPWM1=0; 
  }
  if(LPWM>0){  
    digitalWrite(ldir1, HIGH); 
    digitalWrite(ldir2, LOW);
    LPWM1=LPWM;
  }
  if(LPWM<0){
    digitalWrite(ldir1, LOW); 
    digitalWrite(ldir2, HIGH);
    LPWM1=-LPWM;  
  }
  if(RPWM==0){ 
    digitalWrite(rdir1, LOW); 
    digitalWrite(rdir2, LOW);
    RPWM1=0; 
  }
  if(RPWM>0){  
    digitalWrite(rdir1, HIGH); 
    digitalWrite(rdir2, LOW);
    RPWM1=RPWM;
  }
  if(RPWM<0){
    digitalWrite(rdir1, LOW); 
    digitalWrite(rdir2, HIGH);
    RPWM1=-RPWM;  
  }
  #endif 

  #if defined (APMLF1) || defined (APMLF2)
  if(LPWM==0){ 
    LPWM1=0;
    LPWM2=0;
  }
  if(LPWM>0){ 
    LPWM1=0;
    LPWM2=LPWM;
  }
  if(LPWM<0){ 
    LPWM1=-LPWM;
    LPWM2=0;
  }
  if(RPWM==0){ 
    RPWM1=0;
    RPWM2=0;
  }
  if(RPWM>0){ 
    RPWM1=RPWM;
    RPWM2=0;
  }
  if(RPWM<0){ 
    RPWM1=0;
    RPWM2=-RPWM;
  }
  #endif 
}

void lcd_bar_init() {
  lcd.createChar(1, chr0);  
  lcd.createChar(2, chr1);   
  lcd.createChar(3, chr2); 
  lcd.createChar(4, chr3); 
  lcd.createChar(5, chr4); 
  lcd.createChar(6, chr5); 
  lcd.createChar(7, chr6);
  lcd.createChar(8, chr7); 
}

void lcd_bar(char input,char x,char y){
  char i;
  if(input==0){
    lcd.setCursor(x,y);
    lcd.print(' ');
  }
  if(input==1){
    lcd.setCursor(x,y);
    lcd.write(1);
  }
  if(input==2){
    lcd.setCursor(x,y);
    lcd.write(2);
  }
  if(input==3){
    lcd.setCursor(x,y);
    lcd.write(3);
  }
  if(input==4){
    lcd.setCursor(x,y);
    lcd.write(4);
  }
  if(input==5){
    lcd.setCursor(x,y);
    lcd.write(5);
  }
  if(input==6){
    lcd.setCursor(x,y);
    lcd.write(6);
  }
  if(input==7){
    lcd.setCursor(x,y);
    lcd.write(7);
  }
  if(input==8){
    lcd.setCursor(x,y);
    lcd.write(8);
  }
}

#if defined (SENSOR12)
unsigned int sensor12bar()  {
  unsigned int out_sens=0,sens=0; 
  char scan;
  unsigned char in_sens=0,bar=0;

  for(scan=0;scan<8;scan++){
    in_sens=read_adc(scan);
    lsw(1);
    rsw(0);
    if(scan==0){
      if(in_sens>sensor[scan]){out_sens=out_sens+256;bar=in_sens/31;lcd_bar(bar,10,0);}
      else {lcd.setCursor(10,0);lcd.print(' ');}
    }
    if(scan==1){
      if(in_sens>sensor[scan]){out_sens=out_sens+2;bar=in_sens/31;lcd_bar(bar,3,0);}
      else {lcd.setCursor(3,0);lcd.print(' ');}
    }
    if(scan==2){
      if(in_sens>sensor[scan]){out_sens=out_sens+4;bar=in_sens/31;lcd_bar(bar,4,0);}
      else {lcd.setCursor(4,0);lcd.print(' ');} 
    }
    if(scan==3){
      if(in_sens>sensor[scan]){out_sens=out_sens+8;bar=in_sens/31;lcd_bar(bar,5,0);}
      else {lcd.setCursor(5,0);lcd.print(' ');}
    }
    if(scan==4){
      if(in_sens>sensor[scan]){out_sens=out_sens+16;bar=in_sens/31;lcd_bar(bar,6,0);}
      else {lcd.setCursor(6,0);lcd.print(' ');} 
    }
    if(scan==5){
      if(in_sens>sensor[scan]){out_sens=out_sens+32;bar=in_sens/31;lcd_bar(bar,7,0);}
      else {lcd.setCursor(7,0);lcd.print(' ');} 
    }
    if(scan==6){
      if(in_sens>sensor[scan]){out_sens=out_sens+64;bar=in_sens/31;lcd_bar(bar,8,0);}
      else {lcd.setCursor(8,0);lcd.print(' ');} 
    }
    if(scan==7){
      if(in_sens>sensor[scan]){out_sens=out_sens+128;bar=in_sens/31;lcd_bar(bar,9,0);}
      else {lcd.setCursor(9,0);lcd.print(' ');} 
    }
  }

  for(scan=8;scan<12;scan++){
    in_sens=read_adc(scan-8);
    lsw(0);
    rsw(1);
    if(scan==8){
      if(in_sens>sensor[scan]){out_sens=out_sens+1;bar=in_sens/31;lcd_bar(bar,2,0);}
      else {lcd.setCursor(2,0);lcd.print(' ');} 
    }
    if(scan==9){
      if(in_sens>sensor[scan]){out_sens=out_sens+512;bar=in_sens/31;lcd_bar(bar,11,0);}
      else {lcd.setCursor(11,0);lcd.print(' ');} 
    }
    if(scan==10){
      if(in_sens>sensor[scan]){out_sens=out_sens+1024;bar=in_sens/31;lcd_bar(bar,12,0);}
      else {lcd.setCursor(12,0);lcd.print(' ');}
    }
    if(scan==11){
      if(in_sens>sensor[scan]){out_sens=out_sens+2048;bar=in_sens/31;lcd_bar(bar,13,0);}
      else {lcd.setCursor(13,0);lcd.print(' ');}
    }
  }
  
  out_sensor=out_sens;
  sens=out_sensor;
  return sens;
}

unsigned int sensor12(char mirror)  {
  unsigned int out_sens=0,sens=0; 
  char scan;
  unsigned char in_sens=0,bar=0,sens1=0,sens2=0,sens3=0,sens4=0,sens11=0,sens22=0,sens33=0,sens44=0;

  for(scan=0;scan<8;scan++){
    in_sens=read_adc(scan);
    lsw(1);
    rsw(0);
    if(scan==0){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+256;sens11=sens11+1;}//1R
    }
    if(scan==1){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+2;sens3=sens3+1;}//3L
    }
    if(scan==2){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+4;sens2=sens2+1;}//2L
    }
    if(scan==3){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+8;sens1=sens1+1;}//1L
    }
    if(scan==4){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+16;}
    }
    if(scan==5){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+32;}
    }
    if(scan==6){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+64;}
    }
    if(scan==7){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+128;}
    }
  }
  
  for(scan=8;scan<12;scan++){
    in_sens=read_adc(scan-8);
    lsw(0);
    rsw(1);
    if(scan==8){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+1;sens4=sens4+1;}//4L 
    }
    if(scan==9){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+512;sens22=sens22+1;}//2R
    }
    if(scan==10){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+1024;sens33=sens33+1;}//3R
    }
    if(scan==11){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+2048;sens44=sens44+1;}//4R
    }
  }

  out_sensor=out_sens;
  sens=out_sensor;
  sensor1=sens1;
  sensor2=sens2;
  sensor3=sens3;
  sensor4=sens4;
  sensor11=sens11;
  sensor22=sens22;
  sensor33=sens33;
  sensor44=sens44;
  lcdinvert=0;
  if(mirror==2){
    out_sensor=4095-out_sens;
    sens=out_sensor;
    sensor1=1-sens1;
    sensor2=1-sens2;
    sensor3=1-sens3;
    sensor4=1-sens4; 
    sensor11=1-sens11;
    sensor22=1-sens22;
    sensor33=1-sens33;
    sensor44=1-sens44;
    lcdinvert=1;  
  }
  if(mirror==0&&invers==1){
    out_sensor=4095-out_sens;
    sens=out_sensor;
    sensor1=1-sens1;
    sensor2=1-sens2;
    sensor3=1-sens3;
    sensor4=1-sens4; 
    sensor11=1-sens11;
    sensor22=1-sens22;
    sensor33=1-sens33;
    sensor44=1-sens44;
    lcdinvert=1;
  } else 
    invers=0; 

  switch(sens){
    case 0b111111111110:
    case 0b111111111100:
    case 0b111111111101:
    case 0b111111111001:
    case 0b111111111011:
    case 0b111111110011:
    case 0b111111110111:
    case 0b111111100111:
    case 0b111111101111:
    case 0b111111001111:
    case 0b111111011111:
    case 0b111110011111:
    case 0b111110111111:
    case 0b111100111111:
    case 0b111101111111:
    case 0b111001111111:
    case 0b111011111111:
    case 0b110011111111:
    case 0b110111111111:
    case 0b100111111111:
    case 0b101111111111:
    case 0b001111111111:
    case 0b011111111111:
    case 0b111111111111:
    case 0b111111111000:
    case 0b111111110001:
    case 0b111111100011:
    case 0b111111000111:
    case 0b111110001111:
    case 0b111100011111:
    case 0b111000111111:
    case 0b110001111111:
    case 0b100011111111:
    case 0b000111111111:
    case 0b111111110000:
    case 0b111111100001:
    case 0b111111000011:
    case 0b111110000111:
    case 0b111100001111:
    case 0b111000011111:
    case 0b110000111111:
    case 0b100001111111:
    case 0b000011111111:
    if (mirror==0){
      invers++;
    }
    break; 
  }

  return sens;
}
#endif

#if defined (SENSOR14)
unsigned int sensor14bar(){
  unsigned int out_sens=0,sens=0; 
  char scan;
  unsigned char in_sens=0,bar=0;

  for(scan=0;scan<8;scan++){
    in_sens=read_adc(scan);
    lsw(1);
    rsw(0);
    if(scan==0){
      if(in_sens>sensor[scan]){out_sens=out_sens+512;bar=in_sens/31;lcd_bar(bar,10,0);}
      else {lcd.setCursor(10,0);lcd.print(' ');}
    }
    if(scan==1){
      if(in_sens>sensor[scan]){out_sens=out_sens+4;bar=in_sens/31;lcd_bar(bar,3,0);}
      else {lcd.setCursor(3,0);lcd.print(' ');}
    }
    if(scan==2){
      if(in_sens>sensor[scan]){out_sens=out_sens+8;bar=in_sens/31;lcd_bar(bar,4,0);}
      else {lcd.setCursor(4,0);lcd.print(' ');} 
    }
    if(scan==3){
      if(in_sens>sensor[scan]){out_sens=out_sens+16;bar=in_sens/31;lcd_bar(bar,5,0);}
      else {lcd.setCursor(5,0);lcd.print(' ');}
    }
    if(scan==4){
      if(in_sens>sensor[scan]){out_sens=out_sens+32;bar=in_sens/31;lcd_bar(bar,6,0);}
      else {lcd.setCursor(6,0);lcd.print(' ');} 
    }
    if(scan==5){
      if(in_sens>sensor[scan]){out_sens=out_sens+64;bar=in_sens/31;lcd_bar(bar,7,0);}
      else {lcd.setCursor(7,0);lcd.print(' ');} 
    }
    if(scan==6){
      if(in_sens>sensor[scan]){out_sens=out_sens+128;bar=in_sens/31;lcd_bar(bar,8,0);}
      else {lcd.setCursor(8,0);lcd.print(' ');} 
    }
    if(scan==7){
      if(in_sens>sensor[scan]){out_sens=out_sens+256;bar=in_sens/31;lcd_bar(bar,9,0);}
      else {lcd.setCursor(9,0);lcd.print(' ');} 
    }
  }

  for(scan=8;scan<14;scan++){
    if(scan>11)in_sens=read_adc(scan-7);
    else in_sens=read_adc(scan-8);
    lsw(0);
    rsw(1);
    if(scan==8){
      if(in_sens>sensor[scan]){out_sens=out_sens+2;bar=in_sens/31;lcd_bar(bar,2,0);}
      else {lcd.setCursor(2,0);lcd.print(' ');} 
    }
    if(scan==9){
      if(in_sens>sensor[scan]){out_sens=out_sens+1024;bar=in_sens/31;lcd_bar(bar,11,0);}
      else {lcd.setCursor(11,0);lcd.print(' ');} 
    }
    if(scan==10){
      if(in_sens>sensor[scan]){out_sens=out_sens+2048;bar=in_sens/31;lcd_bar(bar,12,0);}
      else {lcd.setCursor(12,0);lcd.print(' ');}
    }
    if(scan==11){
      if(in_sens>sensor[scan]){out_sens=out_sens+4096;bar=in_sens/31;lcd_bar(bar,13,0);}
      else {lcd.setCursor(13,0);lcd.print(' ');}
    }
    if(scan==12){
      if(in_sens>sensor[scan]){out_sens=out_sens+1;bar=in_sens/31;lcd_bar(bar,1,0);}
      else {lcd.setCursor(1,0);lcd.print(' ');}
    }
    if(scan==13){
      if(in_sens>sensor[scan]){out_sens=out_sens+8192;bar=in_sens/31;lcd_bar(bar,14,0);}
      else {lcd.setCursor(14,0);lcd.print(' ');}
    }
  }
  
  out_sensor=out_sens;
  sens=out_sensor;
  return sens;
}
//*
unsigned int sensor14(char mirror){
  unsigned int out_sens=0,sens=0; 
  char scan;
  unsigned char in_sens=0,bar=0,sens1=0,sens2=0,sens3=0,sens4=0,sens5=0,sens11=0,sens22=0,sens33=0,sens44=0,sens55=0;

  for(scan=0;scan<8;scan++){
    in_sens=read_adc(scan);
    lsw(1);
    rsw(0);
    if(scan==0){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+512;sens11=sens11+1;}//1R
    }
    if(scan==1){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+4;sens3=sens3+1;}//3L
    }
    if(scan==2){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+8;sens2=sens2+1;}//2L
    }
    if(scan==3){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+16;sens1=sens1+1;}//1L
    }
    if(scan==4){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+32;}
    }
    if(scan==5){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+64;}
    }
    if(scan==6){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+128;}
    }
    if(scan==7){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+256;}
    }
  }

  for(scan=8;scan<14;scan++){
    if(scan>11)in_sens=read_adc(scan-7);
    else in_sens=read_adc(scan-8);
    lsw(0);
    rsw(1);
    if(scan==8){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+2;sens4=sens4+1;}//4L
    }
    if(scan==9){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+1024;sens22=sens22+1;}//2R
    }
    if(scan==10){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+2048;sens33=sens33+1;}//3R
    }
    if(scan==11){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+4096;sens44=sens44+1;}//4R
    }
    if(scan==12){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+1;sens5=sens5+1;}//5L
    }
    if(scan==13){
      delayMicroseconds(sensor_speed);
      if(in_sens>sensor[scan]){out_sens=out_sens+8192;sens55=sens55+1;}//5R
    }
  }
  
  out_sensor=out_sens;
  sens=out_sensor;
  sensor1=sens1;
  sensor2=sens2;
  sensor3=sens3;
  sensor4=sens4;
  sensor5=sens5;
  sensor11=sens11;
  sensor22=sens22;
  sensor33=sens33;
  sensor44=sens44;
  sensor55=sens55;
  lcdinvert=0;
  if(mirror==2){
    out_sensor=16383-out_sens;
    sens=out_sensor;
    sensor1=1-sens1;
    sensor2=1-sens2;
    sensor3=1-sens3;
    sensor4=1-sens4;
    sensor5=1-sens5; 
    sensor11=1-sens11;
    sensor22=1-sens22;
    sensor33=1-sens33;
    sensor44=1-sens44;
    sensor55=1-sens55;
    lcdinvert=1;  
  }
  if(mirror==0&&invers==1){
    out_sensor=16383-out_sens;
    sens=out_sensor;
    sensor1=1-sens1;
    sensor2=1-sens2;
    sensor3=1-sens3;
    sensor4=1-sens4;
    sensor5=1-sens5; 
    sensor11=1-sens11;
    sensor22=1-sens22;
    sensor33=1-sens33;
    sensor44=1-sens44;
    sensor55=1-sens55;
    lcdinvert=1;
  } else 
    invers=0;

  switch(sens){
    case 0b11111111111111:
    case 0b11111111111110:
    case 0b11111111111100:
    case 0b11111111111101:
    case 0b11111111111001:
    case 0b11111111111011:
    case 0b11111111110011:
    case 0b11111111110111:
    case 0b11111111100111:
    case 0b11111111101111:
    case 0b11111111001111:
    case 0b11111111011111:
    case 0b11111110011111:
    case 0b11111110111111:
    case 0b11111100111111:
    case 0b11111101111111:
    case 0b11111001111111:
    case 0b11111011111111:
    case 0b11110011111111:
    case 0b11110111111111:
    case 0b11100111111111:
    case 0b11101111111111:
    case 0b11001111111111:
    case 0b11011111111111:
    case 0b10011111111111:
    case 0b10111111111111:
    case 0b00111111111111:
    case 0b01111111111111:
    case 0b11111111111000:
    case 0b11111111110001:
    case 0b11111111100011:
    case 0b11111111000111:
    case 0b11111110001111:
    case 0b11111100011111:
    case 0b11111000111111:
    case 0b11110001111111:
    case 0b11100011111111:
    case 0b11000111111111:
    case 0b10001111111111:
    case 0b00011111111111:
    case 0b11111111110000:
    case 0b11111111100001:
    case 0b11111111000011:
    case 0b11111110000111:
    case 0b11111100001111:
    case 0b11111000011111:
    case 0b11110000111111:
    case 0b11100001111111:
    case 0b11000011111111:
    case 0b10000111111111:
    case 0b00001111111111:
    if (mirror==0){
      invers++;
    }
    break; 
  }

  return sens;
}
#endif

int error_out(unsigned int input_error){
  int out_pv;  

  #if defined (SENSOR12)
  switch(input_error){
    case 0b000000000001: pv=-20; break;
    case 0b000000000011: pv=-18; break;
    case 0b000000000010: pv=-16; break;
    case 0b000000000110: pv=-14; break;
    case 0b000000000100: pv=-12; break;
    case 0b000000001100: pv=-10; break;
    case 0b000000001000: pv=-8; break;
    case 0b000000011000: pv=-6; break;
    case 0b000000010000: pv=-4; break;
    case 0b000000110000: pv=-2; break;
    case 0b000000100000: pv=-1; break;
    case 0b000001100000: pv=0; break;
    case 0b000001000000: pv=1; break;
    case 0b000011000000: pv=2; break;
    case 0b000010000000: pv=4; break;
    case 0b000110000000: pv=6; break;
    case 0b000100000000: pv=8; break;
    case 0b001100000000: pv=10; break;
    case 0b001000000000: pv=12; break;
    case 0b011000000000: pv=14; break;
    case 0b010000000000: pv=16; break;
    case 0b110000000000: pv=18; break;
    case 0b100000000000: pv=20; break;
    case 0b000000000111: pv=-16; break;
    case 0b000000001111: pv=-14; break;
    case 0b000000001110: pv=-12; break;
    case 0b000000011110: pv=-10; break;
    case 0b000000011100: pv=-8; break;
    case 0b000000111100: pv=-6; break;
    case 0b000000111000: pv=-4; break;
    case 0b000001111000: pv=-2; break;
    case 0b000001110000: pv=-1; break;
    case 0b000011110000: pv=0; break;
    case 0b000011100000: pv=1; break;
    case 0b000111100000: pv=2; break;
    case 0b000111000000: pv=4; break;
    case 0b001111000000: pv=6; break;
    case 0b001110000000: pv=8; break;
    case 0b011110000000: pv=10; break;
    case 0b011100000000: pv=12; break;
    case 0b111100000000: pv=14; break;
    case 0b111000000000: pv=16; break;
    case 0b100000000001:
    case 0b110000000011:
    case 0b111000000111:
    case 0b111100001111:
    case 0b111110011111:
    case 0b111111111111:
    case 0b011111111110:
    case 0b001111111100:
    case 0b000111111000:
    case 0b000001000001:
    case 0b010001100010:
    case 0b011001100110:
    case 0b110001100011:
    case 0b011000100110:
    case 0b100011110000:
    case 0b011111100110:
    case 0b001110011100:
    case 0b001100011100:
    case 0b100000111000:
    case 0b000001100001:
    case 0b100001100000:
    case 0b100111000000: pv=0; break;
  }
  #endif

  #if defined (SENSOR14)
  switch(input_error){
    case 0b00000000000001: pv=-24; break;
    case 0b00000000000011: pv=-22; break;
    case 0b00000000000010: pv=-20; break;
    case 0b00000000000110: pv=-18; break;
    case 0b00000000000100: pv=-16; break;
    case 0b00000000001100: pv=-14; break;
    case 0b00000000001000: pv=-12; break;
    case 0b00000000011000: pv=-10; break;
    case 0b00000000010000: pv=-8; break;
    case 0b00000000110000: pv=-6; break;
    case 0b00000000100000: pv=-4; break;
    case 0b00000001100000: pv=-2; break;
    case 0b00000001000000: pv=-1; break;
    case 0b00000011000000: pv=0; break;
    case 0b00000010000000: pv=1; break;
    case 0b00000110000000: pv=2; break;
    case 0b00000100000000: pv=4; break;
    case 0b00001100000000: pv=6; break;
    case 0b00001000000000: pv=8; break;
    case 0b00011000000000: pv=10; break;
    case 0b00010000000000: pv=12; break;
    case 0b00110000000000: pv=14; break;
    case 0b00100000000000: pv=16; break;
    case 0b01100000000000: pv=18; break;
    case 0b01000000000000: pv=20; break;
    case 0b11000000000000: pv=22; break;
    case 0b10000000000000: pv=24; break;
    case 0b00000000000111: pv=-20; break;
    case 0b00000000001111: pv=-18; break;
    case 0b00000000001110: pv=-16; break;
    case 0b00000000011110: pv=-14; break;
    case 0b00000000011100: pv=-12; break;
    case 0b00000000111100: pv=-10; break;
    case 0b00000000111000: pv=-8; break;
    case 0b00000001111000: pv=-6; break;
    case 0b00000001110000: pv=-4; break;
    case 0b00000011110000: pv=-2; break;
    case 0b00000011100000: pv=-1; break;
    case 0b00000111100000: pv=0; break;
    case 0b00000111000000: pv=1; break;
    case 0b00001111000000: pv=2; break;
    case 0b00001110000000: pv=4; break;
    case 0b00011110000000: pv=6; break;
    case 0b00011100000000: pv=8; break;
    case 0b00111100000000: pv=10; break;
    case 0b00111000000000: pv=12; break;
    case 0b01111000000000: pv=14; break;
    case 0b01110000000000: pv=16; break;
    case 0b11110000000000: pv=18; break;
    case 0b11100000000000: pv=20; break;
    case 0b10000000000001:
    case 0b11000000000011:
    case 0b11100000000111:
    case 0b11110000001111:
    case 0b11111000011111:
    case 0b11111100111111:
    case 0b11111111111111:
    case 0b01111111111110:
    case 0b00111111111100:
    case 0b00011111111000:
    case 0b00001111110000:
    case 0b10001111110001:
    case 0b10000011000001:
    case 0b10000010000001:
    case 0b00100011000100:
    case 0b00110011001100:
    case 0b01100011000110:
    case 0b00110001001100:
    case 0b10000111100001:
    case 0b10111111111101:
    case 0b10011111111001:
    case 0b10011100111001:
    case 0b00000001110001:
    case 0b00000011000001:
    case 0b10000011000000:
    case 0b10001110000000: pv=0; break;
  }
  #endif
  
  out_pv=pv;
  return out_pv;
}

int line_center(unsigned int input_error){
  int out_pv;  

  #if defined (SENSOR12)
  switch(input_error){
    case 0b111111000000: pv=-16; break;
    case 0b111111100000: pv=-12; break;
    case 0b111111110000: pv=-8; break;
    case 0b111111111000: pv=-4; break;
    case 0b111111111100: pv=-2; break;
    case 0b111111111110: pv=-1; break;
    case 0b111111111111: pv=0; break;
    case 0b011111111111: pv=1; break;
    case 0b001111111111: pv=2; break;
    case 0b000111111111: pv=4; break;
    case 0b000011111111: pv=8; break;
    case 0b000001111111: pv=12; break;
    case 0b000000111111: pv=16; break;
  }
  #endif

  #if defined (SENSOR14)
  switch(input_error){
    case 0b11111110000000: pv=-20; break;
    case 0b11111111000000: pv=-16; break;
    case 0b11111111100000: pv=-12; break;
    case 0b11111111110000: pv=-8; break;
    case 0b11111111111000: pv=-4; break;
    case 0b11111111111100: pv=-2; break;
    case 0b11111111111110: pv=-1; break;
    case 0b11111111111111: pv=0; break;
    case 0b01111111111111: pv=1; break;
    case 0b00111111111111: pv=2; break;
    case 0b00011111111111: pv=4; break;
    case 0b00001111111111: pv=8; break;
    case 0b00000111111111: pv=12; break;
    case 0b00000011111111: pv=16; break;
    case 0b00000001111111: pv=20; break;
  }
  #endif
  
  out_pv=pv;
  return out_pv;
}

int line_left(unsigned int input_error){
  int out_pv;  
  #if defined (SENSOR12)
  switch(input_error){
    case 0b111111111111: pv=-16; break;
    case 0b011111111111: pv=-12; break;
    case 0b001111111111: pv=-8; break;
    case 0b000111111111: pv=-4; break;
    case 0b000011111111: pv=-2; break;
    case 0b000001111111: pv=-1; break;
    case 0b000000111111: pv=0; break;
    case 0b000000011111: pv=1; break;
    case 0b000000001111: pv=2; break;
    case 0b000000000111: pv=4; break;
    case 0b000000000011: pv=8; break;
    case 0b000000000001: pv=12; break;
    case 0b000000000000: pv=16; break;
  }
  #endif

  #if defined (SENSOR14)
  switch(input_error){
    case 0b11111111111111: pv=-20; break;
    case 0b01111111111111: pv=-16; break;
    case 0b00111111111111: pv=-12; break;
    case 0b00011111111111: pv=-8; break;
    case 0b00001111111111: pv=-4; break;
    case 0b00000111111111: pv=-2; break;
    case 0b00000011111111: pv=-1; break;
    case 0b00000001111111: pv=0; break;
    case 0b00000000111111: pv=1; break;
    case 0b00000000011111: pv=2; break;
    case 0b00000000001111: pv=4; break;
    case 0b00000000000111: pv=8; break;
    case 0b00000000000011: pv=12; break;
    case 0b00000000000001: pv=16; break;
    case 0b00000000000000: pv=20; break;
  }
  #endif
  
  out_pv=pv;
  return out_pv;
}

int line_right(unsigned int input_error){
  int out_pv;  
  #if defined (SENSOR12)
  switch(input_error){
    case 0b000000000000: pv=-16; break;
    case 0b100000000000: pv=-12; break;
    case 0b110000000000: pv=-8; break;
    case 0b111000000000: pv=-4; break;
    case 0b111100000000: pv=-2; break;
    case 0b111110000000: pv=-1; break;
    case 0b111111000000: pv=0; break;
    case 0b111111100000: pv=1; break;
    case 0b111111110000: pv=2; break;
    case 0b111111111000: pv=4; break;
    case 0b111111111100: pv=8; break;
    case 0b111111111110: pv=12; break;
    case 0b111111111111: pv=16; break;
  }
  #endif

  #if defined (SENSOR14)
  switch(input_error){
    case 0b00000000000000: pv=-20; break;
    case 0b10000000000000: pv=-16; break;
    case 0b11000000000000: pv=-12; break;
    case 0b11100000000000: pv=-8; break;
    case 0b11110000000000: pv=-4; break;
    case 0b11111000000000: pv=-2; break;
    case 0b11111100000000: pv=-1; break;
    case 0b11111110000000: pv=0; break;
    case 0b11111111000000: pv=1; break;
    case 0b11111111100000: pv=2; break;
    case 0b11111111110000: pv=8; break;
    case 0b11111111111000: pv=4; break;
    case 0b11111111111100: pv=12; break;
    case 0b11111111111110: pv=16; break;
    case 0b11111111111111: pv=20; break;
  }
  #endif
  
  out_pv=pv;
  return out_pv;
}

int sensorfix(unsigned char m_line){
  #if defined (SENSOR12)
  return sensor12(m_line);
  #endif
  #if defined (SENSOR14)
  return sensor14(m_line);
  #endif
}

unsigned char eeprom_read(unsigned int address){
  return eeprom_read_byte((unsigned char *) address); 
}

void eeprom_write(unsigned int address,unsigned char value){
  eeprom_write_byte((unsigned char *) address, value); 
}

void eeprom_init(){
  if(select==1){
    kpcount_add=start_address;
    counter_add=start_address+(count*1);
    timer_add=start_address+(count*2);
    delaycount_add=start_address+(count*3);
    stopcount_add=start_address+(count*4);
    vcountl_add=start_address+(count*5);
    vcountr_add=start_address+(count*6);
    maxpwm_add=start_address+(count*7);
    readcount_add=start_address+(count*8);
    linecount_add=start_address+(count*9);
    lineselect_add=start_address+(count*10);
    vcount_add=start_address+(count*11); 
    speed1_add=start_address+(count*12); 
    speed2_add=start_address+(count*13); 
    vcount2_add=start_address+(count*14); 
  }
}

// PULSE ADDRESS 0-29;
// 1-29
void save_activation(){
  char address=1;
  char i;
  for(i=1;i<29;i++){
    eeprom_write(i,active[i]);  
  }
}
// 30-60
void save_sensor(){
  char address=30;
  char i;
  for(i=0;i<16;i++){
    eeprom_write(address+i,sensor[i]);  
  }
}
// 61
void save_recent_read(){
  char address=61;
  eeprom_write(address,recent_read);   
}
// 62
void save_recent_write(){
  char address=62;
  eeprom_write(address,recent_write);   
}
// 65-68
void save_pid(){
  char address=65;
  eeprom_write(address,kp);address++;
  eeprom_write(address,ki);address++;
  eeprom_write(address,kd);address++;
  eeprom_write(address,imax);address++;
}
// 69-79
void save_modenormal(){
  char address=69;
  eeprom_write(address,pwm);address++;
  eeprom_write(address,mode);address++;
  eeprom_write(address,linemode);address++;
  eeprom_write(address,timer_speed);address++;
  eeprom_write(address,allmaxpwm);address++;
  eeprom_write(address,triml);address++;
  eeprom_write(address,trimr);address++;
  eeprom_write(address,run_mirror);address++;
}
// 80-139
void save_readpoint(){
  int i,j,l,address,start=80;
  for(i=0;i<select;i++){
    for(j=0;j<cpcount;j++){
      address=start+l;
      eeprom_write(address,readpoint[i][j]); 
      l++;
    }
  }
}
// 140-199
void save_timerpoint(){
  int i,j,l,address,start=140;
  for(i=0;i<select;i++){
    for(j=0;j<cpcount;j++){
      address=start+l;
      eeprom_write(address,timerpoint[i][j]); 
      l++;
    }
  }
}
// 200-424
void save_sdmemory(){
  int i,j,l,address,start=200;
  for(i=0;i<sdselect;i++){
    for(j=0;j<sdcount;j++){
      address=start+l;
      eeprom_write(address,sdmemory[i][j]); 
      l++;
    }
  }
}
// start_address-finish
void save_kpcount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=kpcount_add+l;
      eeprom_write(address,kpcount[i][j]); 
      l++;
    }
  }
}
void save_counter(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=counter_add+l;
      eeprom_write(address,counter[i][j]); 
      l++;
    }
  }
}
void save_timer(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=timer_add+l;
      eeprom_write(address,timer[i][j]); 
      l++;
    }
  }
}
void save_delaycount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=delaycount_add+l;
      eeprom_write(address,delaycount[i][j]); 
      l++;
    }
  }
}
void save_stopcount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=stopcount_add+l;
      eeprom_write(address,stopcount[i][j]); 
      l++;
    }
  }
}
void save_vcountl(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=vcountl_add+l;
      eeprom_write(address,vcountl[i][j]); 
      l++;
    }
  }
}
void save_vcountr(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=vcountr_add+l;
      eeprom_write(address,vcountr[i][j]); 
      l++;
    }
  }
}
void save_maxpwm(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=maxpwm_add+l;
      eeprom_write(address,maxpwm[i][j]); 
      l++;
    }
  }
}
void save_readcount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=readcount_add+l;
      eeprom_write(address,readcount[i][j]); 
      l++;
    }
  }
}
void save_linecount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=linecount_add+l;
      eeprom_write(address,linecount[i][j]); 
      l++;
    }
  }
}
void save_lineselect(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=lineselect_add+l;
      eeprom_write(address,lineselect[i][j]); 
      l++;
    }
  }
}
void save_vcount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=vcount_add+l;
      eeprom_write(address,vcount[i][j]); 
      l++;
    }
  }
}
void save_vcount2(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=vcount2_add+l;
      eeprom_write(address,vcount2[i][j]); 
      l++;
    }
  }
}
void save_speed1(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=speed1_add+l;
      eeprom_write(address,speed1[i][j]); 
      l++;
    }
  }
}
void save_speed2(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=speed2_add+l;
      eeprom_write(address,speed2[i][j]); 
      l++;
    }
  }
}
// 1-29
void read_activation(){
  char address=1;
  char i;
  for(i=1;i<29;i++){
    active[i]=eeprom_read(i);  
  }
}
// 30-60
void read_sensor(){
  char address=30;
  char i;
  for(i=0;i<16;i++){
    sensor[i]=eeprom_read(address+i);  
  }
}
// 61
void read_recent_read(){
  char address=61;
  recent_read=eeprom_read(address);  
}
// 62
void read_recent_write(){
  char address=62;
  recent_write=eeprom_read(address);  
}
// 65-68
void read_pid(){
  char address=65;
  kp=eeprom_read(address);address++;
  ki=eeprom_read(address);address++;
  kd=eeprom_read(address);address++;
  imax=eeprom_read(address);address++;
}
// 69-79
void read_modenormal(){
  char address=69;
  pwm=eeprom_read(address);address++;
  mode=eeprom_read(address);address++;
  linemode=eeprom_read(address);address++;
  timer_speed=eeprom_read(address);address++;
  allmaxpwm=eeprom_read(address);address++;
  triml=eeprom_read(address);address++;
  trimr=eeprom_read(address);address++;
  run_mirror=eeprom_read(address);address++;
}
// 80-139
void read_readpoint(){
  int i,j,l,address,start=80;
  for(i=0;i<select;i++){
    for(j=0;j<cpcount;j++){
      address=start+l;
      readpoint[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
// 140-199
void read_timerpoint(){
  int i,j,l,address,start=140;
  for(i=0;i<select;i++){
    for(j=0;j<cpcount;j++){
      address=start+l;
      timerpoint[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
// 200-424
void read_sdmemory(){
  int i,j,l,address,start=200;
  for(i=0;i<sdselect;i++){
    for(j=0;j<sdcount;j++){
      address=start+l;
      sdmemory[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
// start_address-finish
void read_kpcount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=kpcount_add+l;
      kpcount[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_counter(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=counter_add+l;
      counter[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_timer(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=timer_add+l;
      timer[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_delaycount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=delaycount_add+l;
      delaycount[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_stopcount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=stopcount_add+l;
      stopcount[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_vcountl(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=vcountl_add+l;
      vcountl[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_vcountr(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=vcountr_add+l;
      vcountr[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_maxpwm(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=maxpwm_add+l;
      maxpwm[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_readcount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=readcount_add+l;
      readcount[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_linecount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=linecount_add+l;
      linecount[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_lineselect(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=lineselect_add+l;
      lineselect[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_vcount(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=vcount_add+l;
      vcount[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_vcount2(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=vcount2_add+l;
      vcount2[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_speed1(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=speed1_add+l;
      speed1[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void read_speed2(){
  int i,j,l,address;
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      address=speed2_add+l;
      speed2[i][j]=eeprom_read(address); 
      l++;
    }
  }
}
void setting_default(){
  int i=0,j;
  delay(loading);
  lcd.clear();
  while(1){
    fast=(speed_setting*2)+200;  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ok~ Exit");  
    lcd.setCursor(0,1);
    lcd.print("Cancel~ M+"); 
    if(sw1()==0){i=1;break;}
    if(sw5()==0)break; 
    delay(fast);
  }
  if(i==1)goto rasido;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Defaulting...");
  lcd.setCursor(0,1);
  lcd.print("ArduPanzerMega");
  for(i=0;i<16;i++){sensor[i]=50;}
  recent_read=0;
  recent_write=0;
  pwm=100;
  kp=10;
  ki=0;
  kd=50;
  imax=100;
  mode=0;
  linemode=1;
  timer_speed=5;
  allmaxpwm=255;
  triml=0;
  trimr=0;
  speed_setting1=50; 
  run_select=0;
  run_mirror=0;
  for(i=0;i<select;i++){
    for(j=0;j<cpcount;j++){
      readpoint[i][j]=0;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<cpcount;j++){
      timerpoint[i][j]=0;
    }
  }
  for(i=0;i<sdselect;i++){
    for(j=0;j<sdcount;j++){
      if(i==0)sdmemory[i][j]=sd_def[j];
      else sdmemory[i][j]=sd_empaty[j];
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      kpcount[i][j]=10;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      counter[i][j]=1;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      timer[i][j]=0;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      delaycount[i][j]=10;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      stopcount[i][j]=0;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      vcountl[i][j]=188;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      vcountr[i][j]=188;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      maxpwm[i][j]=255;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      readcount[i][j]=14;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      linecount[i][j]=2;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      lineselect[i][j]=1;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      vcount[i][j]=128;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      vcount2[i][j]=128;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      speed1[i][j]=100;
    }
  }
  for(i=0;i<select;i++){
    for(j=0;j<count;j++){
      speed2[i][j]=100;
    }
  }
  save_sensor();
  save_recent_read();
  save_recent_write();
  save_pid();
  save_modenormal();
  save_readpoint();
  save_timerpoint();
  save_sdmemory();
  save_kpcount();
  save_counter();
  save_timer();
  save_delaycount();
  save_stopcount();
  save_vcountl();
  save_vcountr();
  save_maxpwm();
  save_readcount();
  save_linecount();
  save_lineselect();
  save_vcount();
  save_vcount2();
  save_speed1();
  save_speed2(); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Default Done");
  delay(loading);
rasido:
  lcd.clear();  
}

void sensor_calibrate(){
  int i;
  unsigned char high[16],low[16],adc_input;
  for(i=0;i<16;i++){
    sensor[i]=0;
    low[i]=255;
    high[i]=0;
  }
  
  lcd.clear();
  
  while(1){
    #if defined(SENSOR12)
    lsw(1);
    rsw(0);
    for(i=0;i<8;i++){
      adc_input=read_adc(i);
      if(adc_input<low[i]){
        low[i]=adc_input;           
      }
      if(adc_input>high[i]){
        high[i]=adc_input;        
      }
      delayMicroseconds(20);
    }
    lsw(0);
    rsw(1);
    for(i=8;i<12;i++){
      adc_input=read_adc(i);
      if(adc_input<low[i]){
        low[i]=adc_input;           
      }
      if(adc_input>high[i]){
        high[i]=adc_input;        
      }
      delayMicroseconds(20);
    }
    sensor12bar();
    #endif
    #if defined(SENSOR14)
    lsw(1);
    rsw(0);
    for(i=0;i<8;i++){
      adc_input=read_adc(i);
      if(adc_input<low[i]){
        low[i]=adc_input;           
      }
      if(adc_input>high[i]){
        high[i]=adc_input;        
      }
      delayMicroseconds(20);
    }
    lsw(0);
    rsw(1);
    for(i=8;i<14;i++){
      if(i>11)adc_input=read_adc(i-7);
      else adc_input=read_adc(i-8);
      if(adc_input<low[i]){
        low[i]=adc_input;           
      }
      if(adc_input>high[i]){
        high[i]=adc_input;        
      }
      delayMicroseconds(20);
    }
    sensor14bar();
    #endif
    lcd.setCursor(0,1);
    lcd.print("Calibrating"); 
    if(sw5()==0)break;
  }
  lsw(0);
  rsw(0);
  for(i=0;i<16;i++){
    sensor[i]=(high[i]-low[i])/2+low[i];     
  }
  save_sensor();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving Done");
  delay(loading);
  lcd.clear();
}

void setting_pid(){
  char i=0; 
  delay(loading);
  lcd.clear();
  while(1){
    fast=(speed_setting*2)+200;
    if(sw1()==0)i++;
    if(sw2()==0)i--;
    if(i>3)i=0;
    if(i<0)i=3;
    lcd.clear();
    if(i==0){
      lcd.setCursor(0,0);
      lcd.print("P Kp: ");
      lcd.print(kp);
      if(sw3()==0)kp++;  
      if(sw4()==0)kp--;
      if(kp>255)kp=0;
      if(kp<0)kp=255; 
    }
    if(i==1){
      lcd.setCursor(0,0);
      lcd.print("I Ki: ");
      lcd.print(ki);
      if(sw3()==0)ki++;  
      if(sw4()==0)ki--;
      if(ki>255)ki=0;
      if(ki<0)ki=255; 
    }
    if(i==2){
      lcd.setCursor(0,0);
      lcd.print("D Kd: ");
      lcd.print(kd);
      if(sw3()==0)kd++;  
      if(sw4()==0)kd--;
      if(kd>255)kd=0;
      if(kd<0)kd=255; 
    }
    if(i==3){
      lcd.setCursor(0,0);
      lcd.print("Imax: ");
      lcd.print(imax);
      if(sw3()==0)imax++;  
      if(sw4()==0)imax--;
      if(imax>255)imax=0;
      if(imax<0)imax=255; 
    }
    if(sw5()==0)break;
    delay(fast);
  } 
  save_pid();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving Done");
  delay(loading);
  lcd.clear();
}

void setting_normal(){
  char i=0,j=0,ii=0; 
  char mybuff[25];
  char mybuff1[25];
  delay(loading);
  run_select=0;
  if(recent_read>0)for(ii=0;ii<sdcount;ii++){mybuff[ii]=sdmemory[recent_read-1][ii];} 
  else for(ii=0;ii<sdcount;ii++){mybuff[ii]=sd_empaty[ii];}
  if(recent_write>0)for(ii=0;ii<sdcount;ii++){mybuff1[ii]=sdmemory[recent_write-1][ii];} 
  else for(ii=0;ii<sdcount;ii++){mybuff1[ii]=sd_empaty[ii];}
  lcd.clear();
  while(1){
    fast=(speed_setting*2)+200;   
    if(sw1()==0)i++;
    if(sw2()==0)i--;
    if(i>11)i=0;
    if(i<0)i=11;
    lcd.clear();
    if(i==0){
      lcd.setCursor(0,0);
      lcd.print("Calibrate");
      if(sw3()==0||sw4()==0) sensor_calibrate();  
    }
    if(i==1){
      lcd.setCursor(0,0);
      lcd.print("Speed: ");
      lcd.print(pwm);
      if(sw3()==0)pwm++;  
      if(sw4()==0)pwm--;
      if(pwm>255)pwm=0;
      if(pwm<0)pwm=255; 
    }
    if(i==2){
      lcd.setCursor(0,0);
      lcd.print("PID");
      if(sw3()==0||sw4()==0) setting_pid();  
    }
    if(i==3){
      lcd.setCursor(0,0);
      lcd.print("Mode: ");
      if(mode==0)lcd.print("Normal");
      else lcd.print("Counter");
      if(sw3()==0)mode=1;  
      if(sw4()==0)mode=0;
    }
    if(i==4){
      lcd.setCursor(0,0);
      lcd.print("Line: ");
      if(linemode==0)lcd.print("Auto");
      if(linemode==1)lcd.print("Black");
      if(linemode==2)lcd.print("White");
      if(sw3()==0)linemode++;  
      if(sw4()==0)linemode++;
      if(linemode>2)linemode=0;
      if(linemode<0)linemode=2; 
    }
    if(i==5){
      lcd.setCursor(0,0);
      lcd.print("Timer Speed: ");
      lcd.print(timer_speed);
      if(sw3()==0)timer_speed++;  
      if(sw4()==0)timer_speed--;
      if(timer_speed>255)timer_speed=0;
      if(timer_speed<0)timer_speed=255; 
    } 
    if(i==6){
      lcd.setCursor(0,0);
      lcd.print("MaxPwm: ");
      lcd.print(allmaxpwm);
      if(sw3()==0)allmaxpwm++;  
      if(sw4()==0)allmaxpwm--;
      if(allmaxpwm>255)allmaxpwm=0;
      if(allmaxpwm<0)allmaxpwm=255; 
    }
    if(i==7){
      lcd.setCursor(0,0);
      lcd.print("Trim L: ");
      lcd.print(triml);
      if(sw3()==0)triml++;  
      if(sw4()==0)triml--;
      if(triml>255)triml=0;
      if(triml<0)triml=255; 
    }
    if(i==8){
      lcd.setCursor(0,0);
      lcd.print("Trim R: ");
      lcd.print(trimr);
      if(sw3()==0)trimr++;  
      if(sw4()==0)trimr--;
      if(trimr>255)trimr=0;
      if(trimr<0)trimr=255; 
    }
    if(i==9){
      lcd.setCursor(0,0);
      lcd.print("Run: ");
      if(run_mirror==0)lcd.print("Normal");
      else lcd.print("Mirror");
      if(sw3()==0)run_mirror=1;  
      if(sw4()==0)run_mirror=0; 
    }
    if(i==10){
      lcd.setCursor(0,0);
      lcd.print("History Read");
      lcd.setCursor(0,1);
      lcd.print(mybuff);
    }
    if(i==11){
      lcd.setCursor(0,0);
      lcd.print("History Write");
      lcd.setCursor(0,1);
      lcd.print(mybuff1);
    }
    if(sw1()==0&&sw2()==0){j=1;lcd.clear();delay(200);}
    if(sw3()==0&&sw4()==0){j=0;lcd.clear();delay(200);}
    if(j==1){pwm_on();set_motors(pwm,pwm,allmaxpwm,triml,trimr);}
    if(j==0){pwm_off();set_motors(0,0,0,0,0);}
    if(sw5()==0)break;
    delay(fast); 
  } 
  set_motors(0,0,0,0,0);
  save_modenormal();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving Done");
  delay(loading);
  lcd.clear();
}

void setting_readpoint(){
  int i=0,j=0,k,l; 
  k=select-1;
  l=cpcount-1;
  delay(loading);
  lcd.clear();
  while(1){
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)readpoint[i][j]++;
    if(sw4()==0)readpoint[i][j]--;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CP: ");
    lcd.print(j+1);
    lcd.setCursor(0,1);
    lcd.print("Read: ");
    lcd.print(readpoint[i][j]);
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_readpoint();
batal:
  lcd.clear();
}

void setting_timerpoint(){
  int i=0,j=0,k,l; 
  k=select-1;
  l=cpcount-1;
  delay(loading);
  lcd.clear();
  while(1){
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)timerpoint[i][j]++;
    if(sw4()==0)timerpoint[i][j]--;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CP: ");
    lcd.print(j+1);
    lcd.setCursor(0,1);
    lcd.print("Timer: ");
    lcd.print(timerpoint[i][j]);
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_timerpoint();
batal:
  lcd.clear();
}

void setting_kpcount(){
  int i=0,j=0,k,l,m; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      m=kpcount[i][j];
      for(j=0;j<count;j++){
        kpcount[i][j]=m;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)kpcount[i][j]++;
    if(sw4()==0)kpcount[i][j]--;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("P Kp ");
    lcd.print(j);
    lcd.print(": ");
    lcd.print(kpcount[i][j]);
    if(sw5()==0)break;
    delay(fast);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");   
  save_kpcount();
batal:
  lcd.clear();
}

void auto_set_vc(){
  int i=0,j=0,vcl=0,vcr=0,v_code=0;
  for(j=0;j<count;j++){
    if(counter[i][j]==2){
      if(vcountl[i][j]>128){vcl=(int)128-((int)vcountl[i][j]-128);vcountl[i][j]=vcl;vcl=0;v_code=1;} 
      if(vcountr[i][j]<128){vcr=(int)((int)128-vcountr[i][j])+128;vcountr[i][j]=vcr;vcr=0;v_code=1;} 
      if(v_code==1){
        vcl=(int)(128-vcountr[i][j])+128;
        vcr=(int)128-(vcountl[i][j]-128); 
        vcountl[i][j]=vcl;
        vcountr[i][j]=vcr;
        v_code=0;
      }
    }
    if(counter[i][j]==3){
      if(vcountr[i][j]>128){vcr=(int)128-((int)vcountr[i][j]-128);vcountr[i][j]=vcr;vcl=0;v_code=1;}   
      if(vcountl[i][j]<128){vcl=(int)((int)128-vcountl[i][j])+128;vcountl[i][j]=vcl;vcr=0;v_code=1;}
      if(v_code==1){
        vcl=(int)(128-vcountr[i][j])+128;
        vcr=(int)128-(vcountl[i][j]-128); 
        vcountl[i][j]=vcl;
        vcountr[i][j]=vcr;
        v_code=0;
      }
    }
  }  
}

void setting_counter(){
  int i=0,j=0,k,l,m,vcl=0,vcr=0,v_code=0,n=0,c; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      n++;
      if(n>1)n=0;
      lcd.clear();
      lcd.setCursor(0,0);
      if(n==0)lcd.print("Edit Count");
      else lcd.print("Edit Reader");
      delay(loading);  
    }
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(n==0){
      if(sw3()==0){
        counter[i][j]++;
        if(counter[i][j]==2||counter[i][j]==3||counter[i][j]==4){
          if(counter[i][j]==2)readcount[i][j]=4;
          if(counter[i][j]==3)readcount[i][j]=9;
          if(counter[i][j]==4)readcount[i][j]=17;
        } else {
          readcount[i][j]=14;
        }
      }
      if(sw4()==0){
        counter[i][j]--;
        if(counter[i][j]==2||counter[i][j]==3||counter[i][j]==4){
          if(counter[i][j]==2)readcount[i][j]=4;
          if(counter[i][j]==3)readcount[i][j]=9;
          if(counter[i][j]==4)readcount[i][j]=17;
        } else {
          readcount[i][j]=14;
        }
      } 
    }
    if(n==1){
      if(sw3()==0)readcount[i][j]++;
      if(sw4()==0)readcount[i][j]--;
    }
    if(counter[i][j]>6)counter[i][j]=1;
    if(counter[i][j]<1)counter[i][j]=6; 
    
    if(readcount[i][j]>17)readcount[i][j]=1;
    if(readcount[i][j]<1)readcount[i][j]=17;  
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Count: ");
    lcd.print(j+1);
    lcd.setCursor(0,1);
    if(n==0)lcd.print("~");
    else lcd.print(" "); 
    if(counter[i][j]==1)lcd.print("Lost");
    if(counter[i][j]==2)lcd.print("Left");
    if(counter[i][j]==3)lcd.print("Right");
    if(counter[i][j]==4)lcd.print("Stop");
    if(counter[i][j]==5)lcd.print("Free");
    if(counter[i][j]==6)lcd.print("Out");
    lcd.setCursor(8,1);
    if(n==1)lcd.print("~");
    else lcd.print(" ");
    if(readcount[i][j]==1)lcd.print("L 1");
    if(readcount[i][j]==2)lcd.print("L 2");
    if(readcount[i][j]==3)lcd.print("L 3");
    if(readcount[i][j]==4)lcd.print("L 4");
    if(readcount[i][j]==5)lcd.print("L 5");
    if(readcount[i][j]==6)lcd.print("R 1");
    if(readcount[i][j]==7)lcd.print("R 2");
    if(readcount[i][j]==8)lcd.print("R 3");
    if(readcount[i][j]==9)lcd.print("R 4");
    if(readcount[i][j]==10)lcd.print("R 5");
    if(readcount[i][j]==11)lcd.print("LR 1");
    if(readcount[i][j]==12)lcd.print("LR 2");
    if(readcount[i][j]==13)lcd.print("LR 3");
    if(readcount[i][j]==14)lcd.print("LR 4");
    if(readcount[i][j]==15)lcd.print("LR 5");
    if(readcount[i][j]==16)lcd.print("Timer");
    if(readcount[i][j]==17)lcd.print("Blank");
    if(sw5()==0)break;
    delay(fast);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving"); 
  auto_set_vc();
  save_counter();
  save_vcountl();
  save_vcountr();
  save_readcount();
batal:
  lcd.clear();
}

void setting_timer(){
  int i=0,j=0,k,l,m; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      m=timer[i][j];
      for(j=0;j<count;j++){
        timer[i][j]=m;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0) timer[i][j]++;
    if(sw4()==0) timer[i][j]--;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Timer ");
    lcd.print(j);
    lcd.print(": ");
    lcd.print(timer[i][j]);
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_timer();
batal:
  lcd.clear();
}
void setting_delaycount(){
  int i=0,j=0,k,l,value,m; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      m=delaycount[i][j];
      for(j=0;j<count;j++){
        delaycount[i][j]=m;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)delaycount[i][j]++;
    if(sw4()==0)delaycount[i][j]--;
    if(counter[i][j]==5)
      value=delaycount[i][j];
    else {
      if(delaycount[i][j]<=127) value=delaycount[i][j];
      else value=delaycount[i][j]-128;
    }
    lcd.clear();
    lcd.setCursor(0,0);
    if(counter[i][j]==1)lcd.print("Lost");
    if(counter[i][j]==2)lcd.print("Left");
    if(counter[i][j]==3)lcd.print("Right");
    if(counter[i][j]==4)lcd.print("Stop");
    if(counter[i][j]==5)lcd.print("Free");
    if(counter[i][j]==6)lcd.print("Out");
    lcd.setCursor(0,1);
    if(counter[i][j]==5||counter[i][j]==6){lcd.print("TimerF ");}
    else{
      if(delaycount[i][j]<=127)lcd.print("DelayA ");
      else lcd.print("DelayB ");
    }
    lcd.print(j+1);
    lcd.print(": ");
    lcd.print(value);
    if(sw5()==0)break;
    delay(fast);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");   
  save_delaycount();
batal:
  lcd.clear();
}

void setting_stopcount(){
  int i=0,j=0,k,l,m; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      m=stopcount[i][j];
      for(j=0;j<count;j++){
        stopcount[i][j]=m;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)stopcount[i][j]++;
    if(sw4()==0)stopcount[i][j]--;
    lcd.clear();
    lcd.setCursor(0,0);
    if(counter[i][j]==1)lcd.print("Lost");
    if(counter[i][j]==2)lcd.print("Left");
    if(counter[i][j]==3)lcd.print("Right");
    if(counter[i][j]==4)lcd.print("Stop");
    if(counter[i][j]==5)lcd.print("Free");
    if(counter[i][j]==6)lcd.print("Out");
    lcd.setCursor(0,1);
    lcd.print("Stop ");
    lcd.print(j+1);
    lcd.print(": ");
    lcd.print(stopcount[i][j]);
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_stopcount();
batal:
  lcd.clear();
}

void setting_vcountlr(){
  int i=0,j=0,k=0,l,value,value1,m,m1,n=0; 
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){ 
      m=vcountl[i][j];
      m1=vcountr[i][j];
      if(counter[i][j]==2||counter[i][j]==3){
        if(counter[i][j]==3){
          for(k=0;k<count;k++){
            if(counter[i][k]==3)vcountl[i][k]=m;
            if(counter[i][k]==2)vcountr[i][k]=m;
          }
          for(k=0;k<count;k++){
            if(counter[i][k]==3)vcountr[i][k]=m1;
            if(counter[i][k]==2)vcountl[i][k]=m1;
          }
        }
        if(counter[i][j]==2){
          for(k=0;k<count;k++){
            if(counter[i][k]==2)vcountl[i][k]=m;
            if(counter[i][k]==3)vcountr[i][k]=m;
          }
          for(k=0;k<count;k++){
            if(counter[i][k]==2)vcountr[i][k]=m1;
            if(counter[i][k]==3)vcountl[i][k]=m1;
          }
        }
      } else {
        for(k=0;k<count;k++){
          vcountr[i][k]=m1;
          vcountl[i][k]=m;
        }  
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200; 
    if(sw1()==0)n++;
    if(sw2()==0)n--;
    if(n>1){n=0;j++;}
    if(n<0){n=1;j--;}
    if(j>l)j=0;
    if(j<0)j=l;
    if(n==0){
      if(sw3()==0)vcountl[i][j]++;
      if(sw4()==0)vcountl[i][j]--;
    }
    if(n==1){
      if(sw3()==0)vcountr[i][j]++;
      if(sw4()==0)vcountr[i][j]--;
    }
    if(counter[i][j]==2){
      if(vcountl[i][j]>128)vcountl[i][j]=128; 
      if(vcountr[i][j]<128)vcountr[i][j]=128;
    }
    if(counter[i][j]==3){
      if(vcountl[i][j]<128)vcountl[i][j]=128; 
      if(vcountr[i][j]>128)vcountr[i][j]=128;
    }
    value=vcountl[i][j]-128;
    value1=vcountr[i][j]-128;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("C");
    lcd.print(j+1);
    lcd.print(". ");
    if(counter[i][j]==1)lcd.print("Lost");
    if(counter[i][j]==2)lcd.print("Left");
    if(counter[i][j]==3)lcd.print("Right");
    if(counter[i][j]==4)lcd.print("Stop");
    if(counter[i][j]==5)lcd.print("Free");
    if(counter[i][j]==6)lcd.print("Out");
    lcd.setCursor(0,1);
    if(n==0)lcd.print("~");
    else lcd.print(" ");
    lcd.print("L:");
    lcd.print(value);
    lcd.setCursor(8,1);
    if(n==1)lcd.print("~");
    else lcd.print(" ");
    lcd.print("R:");
    lcd.print(value1);
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_vcountl();
  save_vcountr();
batal:
  lcd.clear();
}

void setting_maxpwm(){
  int i=0,j=0,k,l,m; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      m=maxpwm[i][j];
      for(j=0;j<count;j++){
        maxpwm[i][j]=m;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)maxpwm[i][j]++;
    if(sw4()==0)maxpwm[i][j]--;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MaxPwm ");
    lcd.print(j);
    lcd.print(": ");
    lcd.print(maxpwm[i][j]);
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_maxpwm();
batal:
  lcd.clear();
}

void setting_linecount(){
  int i=0,j=0,k,l,m; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      m=linecount[i][j];
      for(j=0;j<count;j++){
        linecount[i][j]=m;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200; 
    if(sw1()==0)j++;
    if(sw2()==0)j--; 
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)linecount[i][j]++;
    if(sw4()==0)linecount[i][j]--;
    if(linecount[i][j]>3)linecount[i][j]=1;
    if(linecount[i][j]<1)linecount[i][j]=3;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Line ");
    lcd.print(j);
    lcd.print(": ");
    if(linecount[i][j]==1)lcd.print("Auto");
    if(linecount[i][j]==2)lcd.print("Black");
    if(linecount[i][j]==3)lcd.print("White");
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_linecount();
batal:
  lcd.clear();
}

void setting_lineselect(){
  int i=0,j=0,k,l,m; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      m=lineselect[i][j];
      for(j=0;j<count;j++){
        lineselect[i][j]=m;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200; 
    if(sw1()==0)j++;
    if(sw2()==0)j--; 
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)lineselect[i][j]++;
    if(sw4()==0)lineselect[i][j]--;
    if(lineselect[i][j]>4)lineselect[i][j]=1;
    if(lineselect[i][j]<1)lineselect[i][j]=4;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PID ");
    lcd.print(j);
    lcd.print(": ");
    if(lineselect[i][j]==1)lcd.print("Normal");
    if(lineselect[i][j]==2)lcd.print("Center");
    if(lineselect[i][j]==3)lcd.print("Left");
    if(lineselect[i][j]==4)lcd.print("Right");
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_lineselect();
batal:
  lcd.clear();
}

void setting_vcount(){
  int i=0,j=0,k=0,l,value,value1,m,m1,n=0; 
  int testl=0,testr=0;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    fast=(speed_setting*2)+200; 
    if(sw1()==0)n++;
    if(sw2()==0)n--;
    if(n>1){n=0;j++;}
    if(n<0){n=1;j--;}
    if(j>l)j=0;
    if(j<0)j=l;
    if(n==0){
      if(sw3()==0)vcount[i][j]++;
      if(sw4()==0)vcount[i][j]--;
    }
    if(n==1){
      if(sw3()==0)vcount2[i][j]++;
      if(sw4()==0)vcount2[i][j]--;
    }
    value=vcount[i][j]-128;
    value1=vcount2[i][j]-128;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("F");
    lcd.print(j+1);
    lcd.print(". ");
    if(counter[i][j]==1)lcd.print("Lost");
    if(counter[i][j]==2)lcd.print("Left");
    if(counter[i][j]==3)lcd.print("Right");
    if(counter[i][j]==4)lcd.print("Stop");
    if(counter[i][j]==5)lcd.print("Free");
    if(counter[i][j]==6)lcd.print("Out");
    lcd.setCursor(0,1);
    if(n==0)lcd.print("~");
    else lcd.print(" "); 
    lcd.print("L:");
    lcd.print(value);
    lcd.setCursor(8,1);
    if(n==1)lcd.print("~");
    else lcd.print(" "); 
    lcd.print("R:");
    lcd.print(value1);
    
    if(sw1()==0&&sw2()==0){k=1;lcd.clear();delay(200);}
    if(sw3()==0&&sw4()==0){k=0;lcd.clear();delay(200);}
    testl=(int)(vcount[i][j]-128)*2;
    testr=(int)(vcount2[i][j]-128)*2;
    if(k==1){pwm_on();set_motors(testl,testr,allmaxpwm,triml,trimr);}
    if(k==0){pwm_off();set_motors(0,0,0,0,0);}
    if(sw5()==0)break;
    delay(fast);
  } 
  set_motors(0,0,0,0,0);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_vcount();
  save_vcount2();
batal:
  lcd.clear();
}

void setting_speed1(){
  int i=0,j=0,k,l,m; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      m=speed1[i][j];
      for(j=0;j<count;j++){
        speed1[i][j]=m;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)speed1[i][j]++;
    if(sw4()==0)speed1[i][j]--;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Speed1 ");
    lcd.print(j);
    lcd.print(": ");
    lcd.print(speed1[i][j]);
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_speed1();
batal:
  lcd.clear();
}

void setting_speed2(){
  int i=0,j=0,k,l,m; 
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    if(sw1()==0&&sw2()==0){
      m=speed2[i][j];
      for(j=0;j<count;j++){
        speed2[i][j]=m;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set All Done");
      delay(loading);
    }
    fast=(speed_setting*2)+200;   
    if(sw1()==0)j++;
    if(sw2()==0)j--;
    if(j>l)j=0;
    if(j<0)j=l;
    if(sw3()==0)speed2[i][j]++;
    if(sw4()==0)speed2[i][j]--;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Speed2 ");
    lcd.print(j);
    lcd.print(": ");
    lcd.print(speed2[i][j]);
    if(sw5()==0)break;
    delay(fast);
  } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving");  
  save_speed2();
batal:
  lcd.clear();
}

void setting_coppyprogram(){
  int i=1,j,k,l=0,m=0;
  delay(loading);
  k=select-1;
  lcd.clear();
  while(1){  
    fast=(speed_setting*2)+200;
    if(sw3()==0)m=1;
    if(sw4()==0)m=0; 
    if(sw1()==0)i++;
    if(sw2()==0)i--;
    if(i>k){i=0;l++;}
    if(i<0){i=k;l--;}
    if(l>k)l=0;
    if(l<0)l=k;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Invert Counter");
    lcd.setCursor(0,1);
    if(m==0)lcd.print("Normal");
    else lcd.print("Mirror");
    if(sw5()==0)break;
    if(sw1()==0&&sw2()==0)goto batal;
    delay(fast);  
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Inverting...");
  for(j=0;j<cpcount;j++){
    readpoint[i][j]=readpoint[l][j];
  }
  for(j=0;j<cpcount;j++){
    timerpoint[i][j]=timerpoint[l][j];
  }
  for(j=0;j<count;j++){
    kpcount[i][j]=kpcount[l][j];
  }
  for(j=0;j<count;j++){
    if(m==0)counter[i][j]=counter[l][j];
    else {
      if(counter[l][j]==2||counter[l][j]==3){
        if(counter[l][j]==2){counter[i][j]=3;goto next;}
        if(counter[l][j]==3){counter[i][j]=2;goto next;}
      } else {
        counter[i][j]=counter[l][j];led_timer(0);
      }
    }
next:
    led_timer(1);
  }
  led_timer(0);
  for(j=0;j<count;j++){
    timer[i][j]=timer[l][j];
  }
  for(j=0;j<count;j++){
    delaycount[i][j]=delaycount[l][j];
  }
  for(j=0;j<count;j++){
    stopcount[i][j]=stopcount[l][j];
  }
  for(j=0;j<count;j++){
    vcountl[i][j]=vcountl[l][j];
  }
  for(j=0;j<count;j++){
    vcountr[i][j]=vcountr[l][j];
  }
  for(j=0;j<count;j++){
    maxpwm[i][j]=maxpwm[l][j];
  }
  for(j=0;j<count;j++){
    if(m==0)readcount[i][j]=readcount[l][j];
    else {
      if(readcount[l][j]<=10){
        if(readcount[l][j]==1){readcount[i][j]=6;goto next1;}
        if(readcount[l][j]==2){readcount[i][j]=7;goto next1;}
        if(readcount[l][j]==3){readcount[i][j]=8;goto next1;}
        if(readcount[l][j]==4){readcount[i][j]=9;goto next1;}
        if(readcount[l][j]==5){readcount[i][j]=10;goto next1;}
        if(readcount[l][j]==6){readcount[i][j]=1;goto next1;}
        if(readcount[l][j]==7){readcount[i][j]=2;goto next1;}
        if(readcount[l][j]==8){readcount[i][j]=3;goto next1;}
        if(readcount[l][j]==9){readcount[i][j]=4;goto next1;}
        if(readcount[l][j]==10){readcount[i][j]=5;goto next1;}
      } else {
        readcount[i][j]=readcount[l][j];led_timer(0);
      }
    }
next1:
    led_timer(1);
  }
  led_timer(0);
  for(j=0;j<count;j++){
    linecount[i][j]=linecount[l][j];
  }
  for(j=0;j<count;j++){
    if(m==0)lineselect[i][j]=lineselect[l][j];
    else {
      if(lineselect[l][j]==3||lineselect[l][j]==4){
        if(lineselect[l][j]==3){lineselect[i][j]=4;goto next2;}
        if(lineselect[l][j]==4){lineselect[i][j]=3;goto next2;}
      } else {
        lineselect[i][j]=lineselect[l][j];led_timer(0);
      }
    } 
next2:
    led_timer(1);
  }
  for(j=0;j<count;j++){
    vcount[i][j]=vcount[l][j];
  }
  for(j=0;j<count;j++){
    vcount2[i][j]=vcount2[l][j];
  }
  for(j=0;j<count;j++){
    speed1[i][j]=speed1[l][j];
  }
  for(j=0;j<count;j++){
    speed2[i][j]=speed2[l][j];
  }
  auto_set_vc();
  save_readpoint();
  save_timerpoint();
  save_kpcount();
  save_counter();
  save_timer();
  save_delaycount();
  save_stopcount();
  save_vcountl();
  save_vcountr();
  save_maxpwm();
  save_readcount();
  save_linecount();
  save_lineselect();
  save_vcount();
  save_vcount2();
  save_speed1();
  save_speed2();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Inverting Done");
batal:
  delay(loading);
  lcd.clear();
}

void setting_insert(){
  int i=0,j=0,k,l,m=0,n,x,y;
  unsigned char ram[count];
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){  
    fast=(speed_setting*2)+200;
    if(sw3()==0)j++;
    if(sw4()==0)j--; 
    if(sw1()==0)m++;
    if(sw2()==0)m--;
    if(m>l)m=0;
    if(m<0)m=l;
    if(j>l)j=0;
    if(j<0)j=l;
    if(m<j)m=j;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Insert: ");
    lcd.print(j+1);
    lcd.print("~");
    lcd.print(m+1);
    if(sw5()==0)break;
    delay(fast);  
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Insert...");
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=kpcount[i][n];
  }
  for(n=x;n<=y;n++){
    kpcount[i][n]=10;
  }
  y++;
  for(n=y;n<count;n++){
    kpcount[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=counter[i][n];
  }
  for(n=x;n<=y;n++){
    counter[i][n]=1;
  }
  y++;
  for(n=y;n<count;n++){
    counter[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=timer[i][n];
  }
  for(n=x;n<=y;n++){
    timer[i][n]=0;
  }
  y++;
  for(n=y;n<count;n++){
    timer[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=delaycount[i][n];
  }
  for(n=x;n<=y;n++){
    delaycount[i][n]=10;
  }
  y++;
  for(n=y;n<count;n++){
    delaycount[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=stopcount[i][n];
  }
  for(n=x;n<=y;n++){
    stopcount[i][n]=0;
  }
  y++;
  for(n=y;n<count;n++){
    stopcount[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=vcountl[i][n];
  }
  for(n=x;n<=y;n++){
    vcountl[i][n]=188;
  }
  y++;
  for(n=y;n<count;n++){
    vcountl[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=vcountr[i][n];
  }
  for(n=x;n<=y;n++){
    vcountr[i][n]=188;
  }
  y++;
  for(n=y;n<count;n++){
    vcountr[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=maxpwm[i][n];
  }
  for(n=x;n<=y;n++){
    maxpwm[i][n]=255;
  }
  y++;
  for(n=y;n<count;n++){
    maxpwm[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=readcount[i][n];
  }
  for(n=x;n<=y;n++){
    readcount[i][n]=14;
  }
  y++;
  for(n=y;n<count;n++){
    readcount[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=linecount[i][n];
  }
  for(n=x;n<=y;n++){
    linecount[i][n]=2;
  }
  y++;
  for(n=y;n<count;n++){
    linecount[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=lineselect[i][n];
  }
  for(n=x;n<=y;n++){
    lineselect[i][n]=1;
  }
  y++;
  for(n=y;n<count;n++){
    lineselect[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=vcount[i][n];
  }
  for(n=x;n<=y;n++){
    vcount[i][n]=128;
  }
  y++;
  for(n=y;n<count;n++){
    vcount[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=vcount2[i][n];
  }
  for(n=x;n<=y;n++){
    vcount2[i][n]=128;
  }
  y++;
  for(n=y;n<count;n++){
    vcount2[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=speed1[i][n];
  }
  for(n=x;n<=y;n++){
    speed1[i][n]=100;
  }
  y++;
  for(n=y;n<count;n++){
    speed1[i][n]=ram[x];x++;
  }
  x=j;
  y=m;
  for(n=0;n<count;n++){
    ram[n]=speed2[i][n];
  }
  for(n=x;n<=y;n++){
    speed2[i][n]=100;
  }
  y++;
  for(n=y;n<count;n++){
    speed2[i][n]=ram[x];x++;
  }
  auto_set_vc();
  save_kpcount();
  save_counter();
  save_timer();
  save_delaycount();
  save_stopcount();
  save_vcountl();
  save_vcountr();
  save_maxpwm();
  save_readcount();
  save_linecount();
  save_lineselect();
  save_vcount();
  save_vcount2();
  save_speed1();
  save_speed2();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Insert Done");
batal:
  delay(loading);
  lcd.clear();
}

void setting_delete(){
  int i=0,j=0,k,l,m=0,n,x,y;
  unsigned char ram[count];
  k=select-1;
  l=count-1;
  delay(loading);
  lcd.clear();
  while(1){  
    fast=(speed_setting*2)+200;
    if(sw3()==0)j++;
    if(sw4()==0)j--; 
    if(sw1()==0)m++;
    if(sw2()==0)m--;
    if(m>l)m=0;
    if(m<0)m=l;
    if(j>l)j=0;
    if(j<0)j=l;
    if(m<j)m=j;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Delete: ");
    lcd.print(j+1);
    lcd.print("~");
    lcd.print(m+1);
    if(sw5()==0)break;
    delay(fast);  
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Delete...");
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=kpcount[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    kpcount[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=counter[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    counter[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=timer[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    timer[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=delaycount[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    delaycount[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=stopcount[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    stopcount[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=vcountl[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    vcountl[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=vcountr[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    vcountr[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=maxpwm[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    maxpwm[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=readcount[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    readcount[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=linecount[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    linecount[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=lineselect[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    lineselect[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=vcount[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    vcount[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=vcount2[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    vcount2[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=speed1[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    speed1[i][n]=ram[x];x++;
  }
  x=m;
  y=j;
  for(n=0;n<count;n++){
    ram[n]=speed2[i][n];
  }
  x++;
  for(n=y;n<count;n++){
    speed2[i][n]=ram[x];x++;
  }
  auto_set_vc();
  save_kpcount();
  save_counter();
  save_timer();
  save_delaycount();
  save_stopcount();
  save_vcountl();
  save_vcountr();
  save_maxpwm();
  save_readcount();
  save_linecount();
  save_lineselect();
  save_vcount();
  save_vcount2();
  save_speed1();
  save_speed2();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Delete Done");
batal:
  delay(loading);
  lcd.clear();
}

void setting_mirror(){
  int i,j;
  i=run_select;
  for(j=0;j<count;j++){
    if(counter[i][j]==2||counter[i][j]==3){
      if(counter[i][j]==2){counter[i][j]=3;goto next;}
      if(counter[i][j]==3){counter[i][j]=2;goto next;}
    }
next:
    led_timer(0);
  }
  for(j=0;j<count;j++){
    if(readcount[i][j]<=10){
      if(readcount[i][j]==1){readcount[i][j]=6;goto next1;}
      if(readcount[i][j]==2){readcount[i][j]=7;goto next1;}
      if(readcount[i][j]==3){readcount[i][j]=8;goto next1;}
      if(readcount[i][j]==4){readcount[i][j]=9;goto next1;}
      if(readcount[i][j]==5){readcount[i][j]=10;goto next1;}
      if(readcount[i][j]==6){readcount[i][j]=1;goto next1;}
      if(readcount[i][j]==7){readcount[i][j]=2;goto next1;}
      if(readcount[i][j]==8){readcount[i][j]=3;goto next1;}
      if(readcount[i][j]==9){readcount[i][j]=4;goto next1;}
      if(readcount[i][j]==10){readcount[i][j]=5;goto next1;}
    }
next1:
    led_timer(0);
  }
  for(j=0;j<count;j++){
    if(lineselect[i][j]==3||lineselect[i][j]==4){
      if(lineselect[i][j]==3){lineselect[i][j]=4;goto next2;}
      if(lineselect[i][j]==4){lineselect[i][j]=3;goto next2;}
    }
next2:
    led_timer(0);
  }
  auto_set_vc();
}

char verify(const char *my_password,char *parameter){
  char i=0,j=0,value=0,count=0;
  j=strlen(my_password);
  for(i=0;i<j;i++){
    if(my_password[i]!=parameter[i]){value=0;goto salah;}
    count++;
    if(count==j)value=1;
  }
salah:
  return value;
}

void serial_off(){
  UCSR0B=0x00;
  cli();
}

void setting_activation(){
  int i=0,j,k=0,user=0,premium=0; 
  char buffer[50],remot;
  for(j=0;j<50;j++){buffer[j]=0;}
  active[0] = eeprom_read(0);  
  while(1){
    if(sw2()==0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Bluetoth Remot");
      delay(2000);
      lcd.clear();  
      Serial.begin(9600); 
      pwm_on();
      while(1){ 
        remot=Serial.read();
        switch(remot){
          case 'F': set_motors(pwm,pwm,allmaxpwm,triml,trimr); led_timer(1); break;
          case 'B': set_motors(-pwm,-pwm,allmaxpwm,triml,trimr); led_timer(1); break;
          case 'L': set_motors(-pwm,pwm,allmaxpwm,triml,trimr); led_timer(1); break;
          case 'R': set_motors(pwm,-pwm,allmaxpwm,triml,trimr); led_timer(1); break;
          case 'C': set_motors(0,pwm,allmaxpwm,triml,trimr); led_timer(1); break;
          case 'D': set_motors(pwm,0,allmaxpwm,triml,trimr); led_timer(1); break;
          case 'A': set_motors(0,0,allmaxpwm,triml,trimr); digitalWrite(relay,HIGH); led_timer(1); break;
          case '0': set_motors(0,0,allmaxpwm,triml,trimr); led_timer(0);   break;
          default: led_timer(0); digitalWrite(relay,LOW); break;
        }    
      }
    }
  
    lcd_bar(active[1]/31,0,0);
    lcd_bar(active[2]/31,1,0);
    lcd_bar(active[3]/31,2,0);
    lcd_bar(active[4]/31,3,0);
    lcd_bar(active[5]/31,4,0);
    lcd_bar(active[6]/31,5,0);
    lcd_bar(active[7]/31,6,0);
    lcd_bar(active[8]/31,7,0);
    lcd_bar(active[9]/31,8,0);
    lcd_bar(active[10]/31,9,0);
    lcd_bar(active[11]/31,10,0);
    lcd_bar(active[12]/31,11,0);
    lcd_bar(active[13]/31,12,0);
    lcd_bar(active[14]/31,13,0);
    lcd_bar(active[15]/31,14,0);
    lcd_bar(active[16]/31,15,0);
    lcd_bar(active[17]/31,0,1);
    lcd_bar(active[18]/31,1,1);
    lcd_bar(active[19]/31,2,1);
    lcd_bar(active[20]/31,3,1);
    lcd_bar(active[21]/31,4,1);
    lcd_bar(active[22]/31,5,1);
    lcd_bar(active[23]/31,6,1);
    lcd_bar(active[24]/31,7,1);
    lcd_bar(active[25]/31,8,1);
    lcd_bar(active[26]/31,9,1);
    lcd_bar(active[27]/31,10,1);
    lcd_bar(active[28]/31,11,1);
    led_timer(1);
    delay(50);
    led_timer(0);
    delay(50);
    if(sw3()==0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Mode Serial");
      delay(loading);
      lcd.clear();  
      Serial.begin(9600); 
      i=1;
      Serial.println();
      Serial.println("Serial Open");
      Serial.println("ArduPanzerMega Expert Line Follower Robot");
    }

    if(i==1){
      if(Serial.available()){
        buffer[k]=Serial.read();
        k++;
      }
      if(verify(mypassword,buffer)==1&&premium==0){
        Serial.println("PREMIUM ID OK");
        for(j=0;j<50;j++){buffer[j]=0;}
        k=0;
        premium=1;
        for(j=1;j<30;j++){active[j]=active_value;}
          active_ok=1;
          save_activation();
          eeprom_write(0,5);
          Serial.println("PREMIUM ACTIVE");
          break;
        }
      if(verify(mypassword1,buffer)==1&&user==0){
        Serial.println("USER ID OK");
        for(j=0;j<50;j++){buffer[j]=0;}
        k=0;
        user=1;
        if(active[0]==5){
          for(j=1;j<30;j++){active[j]=user_active_value;}
          active_ok=1;
          save_activation();
          eeprom_write(0,4);
          Serial.println("USER ACTIVE");
          break; 
        } else {
          active_ok=0;
          Serial.println("USER 2 NOT ACTIVE");
          break;
        }
      }
      if(verify(mypassword2,buffer)==1&&user==0){
        Serial.println("USER ID 2 OK");
        for(j=0;j<50;j++){buffer[j]=0;}
        k=0;
        user=1;
        if(active[0]==4){
          for(j=1;j<30;j++){active[j]=user_active_value;}
          active_ok=1;
          save_activation();
          eeprom_write(0,0);
          Serial.println("USER 2 ACTIVE");
          break; 
        } else {
          active_ok=0;
          Serial.println("USER 2 NOT ACTIVE");
          break;
        }
      }
    }
    if(sw5()==0)break;
    if(active_ok!=1&&i==0){
      lcd.clear();
      lcd.setCursor(0,0);  
      lcd.print("NOT ACTIVE");
      delay(300);
    }
  }
//serial_off();
  lcd.begin(16, 2);
  lcd.clear();  
  delay(loading);
}

void activation(){
  char i,j;
  active_ok=0;
  for(i=30;i>0;i--){
    if(active[i]>0){active[i]--;active_ok=1;goto save;}
  }
save:
  save_activation();
}

void setting_advance(){
  char i=0; 
  delay(loading);
  lcd.clear();
  while(1){
    fast=(speed_setting*2)+200;   
    if(sw1()==0)i++;
    if(sw2()==0)i--;
    if(i>21)i=0;
    if(i<0)i=21;
    lcd.clear();
    if(i==0){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Kp Counter");
      if(sw3()==0||sw4()==0)setting_kpcount();  
    }  
    if(i==1){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Timer");
      if(sw3()==0||sw4()==0)setting_timer();  
    }  
    if(i==2){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Speed 1");
      if(sw3()==0||sw4()==0)setting_speed1();  
    }
    if(i==3){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Speed 2");
      if(sw3()==0||sw4()==0)setting_speed2();  
    }
    if(i==4){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Counter");
      if(sw3()==0||sw4()==0)setting_counter();  
    }  
    if(i==5){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Line Counter");
      if(sw3()==0||sw4()==0)setting_linecount(); 
    }  
    if(i==6){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".PID Counter");
      if(sw3()==0||sw4()==0)setting_lineselect(); 
    } 
    if(i==7){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Delay&Timer");
      if(sw3()==0||sw4()==0)setting_delaycount();
    } 
    if(i==8){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Stop Counter");
      if(sw3()==0||sw4()==0)setting_stopcount();
    } 
    if(i==9){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".V Free");
      if(sw3()==0||sw4()==0)setting_vcount();
    } 
    if(i==10){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".V Counter");
      if(sw3()==0||sw4()==0)setting_vcountlr();
    } 
    if(i==11){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".MaxPwm");
      if(sw3()==0||sw4()==0)setting_maxpwm();
    } 
    if(i==12){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Plan Read");
      if(sw3()==0||sw4()==0)setting_readpoint();
    } 
    if(i==13){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Plan Timer");
      if(sw3()==0||sw4()==0)setting_timerpoint();
    } 
    if(i==14){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Invert Count");
      if(sw3()==0||sw4()==0)setting_coppyprogram();
    } 
    if(i==15){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Insert");
      if(sw3()==0||sw4()==0)setting_insert();
    } 
    if(i==16){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Delete");
      if(sw3()==0||sw4()==0)setting_delete();
    } 
    if(i==17){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Set Default");
      if(sw3()==0||sw4()==0)setting_default();
    }
    if(i==18){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Format File");
      if(sw3()==0||sw4()==0)format_dir();
    } 
    if(i==19){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Select File");
      if(sw3()==0||sw4()==0)scanfile(".TXT");
    }
    if(i==20){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".SD To Eeprom");
      if(sw3()==0||sw4()==0)setting_readfile();
    } 
    if(i==21){
      lcd.setCursor(0,0);
      lcd.print(i+1);
      lcd.print(".Eeprom To SD");
      if(sw3()==0||sw4()==0)setting_writefile();
    } 
    if(sw5()==0)break;
    delay(fast);
  }  
  delay(loading);
  lcd.clear();
}

void idle(){
  int i=0,j,k;
  lcd.clear();
  if( hack == 1 ){
    for(j=1;j<30;j++){active[j]=active_value;}
    active_ok=1;
    save_activation();
    eeprom_write(0,0); 
    lcd.setCursor(0,0);
    lcd.print("Premium Active");
    delay(1000);
  }
  k=eeprom_read(0);
  if(sw2()==0&&sw3()==0&&sw5()==0&&k<5){
    for(j=1;j<30;j++){active[j]=user_active_value;}
    active_ok=1;
    k++;
    save_activation();
    eeprom_write(0,k);  
    lcd.setCursor(0,0);
    lcd.print("User Active");
    lcd.setCursor(0,1);
    lcd.print("Count: ");
    lcd.print(k);
    while(1);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Panzer Robotics");
  lcd.setCursor(0,1);
  lcd.print("APM1284 SDCARD");
  delay(500);
  lcd.clear();
  while(1){
    #if defined(SENSOR12)
    sensor12bar();
    #endif
    #if defined(SENSOR14)
    sensor14bar();
    #endif
    //if(sw2()==0&&active_ok==1){lsw(0);rsw(0);setting_advance();}
    if(sw2()==0){lsw(0);rsw(0);setting_advance();}
    if(sw1()==0){lsw(0);rsw(0);setting_normal();}
    if(sw5()==0)break;
    if(sw3()==0){i=1;break;}
    lcd.setCursor(0,1);
    if(mode==0)lcd.print("Mode Normal");
    else {
      if(run_mirror==0)lcd.print("Counter Normal");
      else lcd.print("Counter Mirror");
    }
  }
  if(i==1)setting_cp();
  if(mode==1&&run_mirror==1)setting_mirror();
  lcd.clear();
  en_timer=1;
  led_lcd(1,lcdinvert);
}

void lcd_clear(char time){
  if(lcdtime>time){lcd.clear();lcdtime=0;}  
}

void mode_stop(){
  if(stopcount[run_select][run_counter-1]>0){
    set_motors(-255,-255,allmaxpwm,triml,trimr);
    mydelay(stopcount[run_select][run_counter-1]);
  }
  while(1){
    lsw(1);
    rsw(1);
    pwm_off();
    set_motors(0,0,allmaxpwm,triml,trimr);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stop");
    delay(1000);
    lsw(0);
    rsw(0);
    led_lcd(0,0);
    delay(1000);
    led_lcd(1,0);
  }
}

void mode_free(){
  int ml,mr;
  clear_pid();
  lcd.clear();
  led_lcd(0,lcdinvert);
  ml=(int)(vcount[run_select][run_counter-1]-128)*2;
  mr=(int)(vcount2[run_select][run_counter-1]-128)*2;
  if(stopcount[run_select][run_counter-1]>0){
    set_motors(-255,-255,allmaxpwm,triml,trimr);
    mydelay(stopcount[run_select][run_counter-1]);
  }
  clock_delay=0;
  run_delay=0;
  en_delay=1;
  en_timer=0;
  lcd.clear();
  while(1){
    lcd.setCursor(0,0);
    lcd.print(ml);
    lcd.print(" ");
    lcd.print(mr);
    lcd.print(" ");
    lcd.print(run_counter);
    lcd.print(" ");
    lcd.print(run_delay);
    set_motors(ml,mr,allmaxpwm,triml,trimr);
    if(run_delay>=delaycount[run_select][run_counter-1])break;
  }
  set_motors(0,0,allmaxpwm,triml,trimr);
  lcd.clear();
  clock_timer=0;
  run_timer=0;
  en_timer=1;
  en_delay=0;
  lock_count=0;
  pv_out=0;
}

void mode_left(){
  int ml,mr,pvv_count,pv_count,del;
  en_timer=0;
  clock_timer=0;
  run_timer=0;
  led_lcd(0,lcdinvert);
  if(stopcount[run_select][run_counter-1]>0){ // stop counter
    set_motors(-255,-255,allmaxpwm,triml,trimr);
    mydelay(stopcount[run_select][run_counter-1]);
  }

  ml=(int)(vcountl[run_select][run_counter-1]-128)*2;
  mr=(int)(vcountr[run_select][run_counter-1]-128)*2;
  memo_l=ml;
  memo_r=mr;
  set_motors(ml,mr,allmaxpwm,triml,trimr);
  if(delaycount[run_select][run_counter-1]<=127){ // delay A
    lock_count=1;
    sensor1=0;
    del=delaycount[run_select][run_counter-1];
    mydelay(del);
  } else {
    lock_count=0;
    pv_out=0;
    del=(int)(delaycount[run_select][run_counter-1]-128); // delay B No memory
    mydelay(del);
    clear_pid();
    set_motors(0,0,allmaxpwm,triml,trimr);
    read1x=0;
    en_timer=1;
  }
}

void mode_right(){
  int ml,mr,pvv_count,pv_count,del;
  en_timer=0;
  clock_timer=0;
  run_timer=0;
  led_lcd(0,lcdinvert);
  if(stopcount[run_select][run_counter-1]>0){ // stop count
    set_motors(-255,-255,allmaxpwm,triml,trimr);
    mydelay(stopcount[run_select][run_counter-1]);
  }

  ml=(int)(vcountl[run_select][run_counter-1]-128)*2;
  mr=(int)(vcountr[run_select][run_counter-1]-128)*2;
  memo_l=ml;
  memo_r=mr;
  set_motors(ml,mr,allmaxpwm,triml,trimr);
  if(delaycount[run_select][run_counter-1]<=127){ // delay A
    lock_count=2;
    sensor11=0;
    del=delaycount[run_select][run_counter-1];
    mydelay(del);
  } else { // delay B
    pv_out=0;
    lock_count=0;
    del=(int)(delaycount[run_select][run_counter-1]-128);
    mydelay(del);
    clear_pid();
    set_motors(0,0,allmaxpwm,triml,trimr);
    read1x=0;
    en_timer=1;
  }
}

// Start Mode Counter
void mode_counter(){
  if(mode==1){ 
    kp=kpcount[run_select][run_counter]; 
    if(lock_count==1){
      if(sensor1>0){
        lock_count=0;
        read1x=0;
        en_timer=1;
      } 
    }
    if(sensor11>0){
      if(pv_out>1&&pv_out<=25){
        lock_count=0;
        read1x=0;
        en_timer=1;
      } 
    }
    if(run_timer>=timer[run_select][run_counter]) pwm=speed2[run_select][run_counter];
    else pwm=speed1[run_select][run_counter];
    linemode=linecount[run_select][run_counter]-1;
    pv_in=sensorfix(linemode);
    if(lineselect[run_select][run_counter]==1)pv_out=error_out(pv_in); 
    if(lineselect[run_select][run_counter]==2)pv_out=sp-line_center(pv_in);
    if(lineselect[run_select][run_counter]==3)pv_out=sp-line_left(pv_in);
    if(lineselect[run_select][run_counter]==4)pv_out=sp-line_right(pv_in);
    
    if(readcount[run_select][run_counter]==1){
      if(run_timer>=timer[run_select][run_counter]&&sensor1==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }  
      }
    }
    if(readcount[run_select][run_counter]==2){
      if(run_timer>=timer[run_select][run_counter]&&sensor2==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }
      }
    }
    if(readcount[run_select][run_counter]==3){
      if(run_timer>=timer[run_select][run_counter]&&sensor3==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }  
      }
    }
    if(readcount[run_select][run_counter]==4){
      if(run_timer>=timer[run_select][run_counter]&&sensor4==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }  
      }
    }
    if(readcount[run_select][run_counter]==5){
      if(run_timer>=timer[run_select][run_counter]&&sensor5==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }  
      }
    }
    if(readcount[run_select][run_counter]==6){
      if(run_timer>=timer[run_select][run_counter]&&sensor11==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }  
      }
    }
    if(readcount[run_select][run_counter]==7){
      if(run_timer>=timer[run_select][run_counter]&&sensor22==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }  
      }
    }
    if(readcount[run_select][run_counter]==8){
      if(run_timer>=timer[run_select][run_counter]&&sensor33==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }  
      }
    }
    if(readcount[run_select][run_counter]==9){
      if(run_timer>=timer[run_select][run_counter]&&sensor44==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }  
      }
    }
    if(readcount[run_select][run_counter]==10){
      if(run_timer>=timer[run_select][run_counter]&&sensor55==1) {
        if(read1x==0){
          run_counter++;
          read1x=1;
          c_lock=1;
          en_timer=0;
        }  
      }
    }
    if(readcount[run_select][run_counter]==11){
      if(run_timer>=timer[run_select][run_counter]&&(sensor1==1||sensor11==1)) {
        if(read1x==0){run_counter++;read1x=1;c_lock=1;en_timer=0;}  
      }
    }
    if(readcount[run_select][run_counter]==12){
      if(run_timer>=timer[run_select][run_counter]&&(sensor2==1||sensor22==1)) {
        if(read1x==0){run_counter++;read1x=1;c_lock=1;en_timer=0;}  
      }
    }
    if(readcount[run_select][run_counter]==13){
      if(run_timer>=timer[run_select][run_counter]&&(sensor3==1||sensor33==1)) {
        if(read1x==0){run_counter++;read1x=1;c_lock=1;en_timer=0;}  
      }
    }
    if(readcount[run_select][run_counter]==14){
      if(run_timer>=timer[run_select][run_counter]&&(sensor4==1||sensor44==1)) {
        if(read1x==0){run_counter++;read1x=1;c_lock=1;en_timer=0;}  
      }
    }
    if(readcount[run_select][run_counter]==15){
      if(run_timer>=timer[run_select][run_counter]&&(sensor5==1||sensor55==1)) {
        if(read1x==0){run_counter++;read1x=1;c_lock=1;en_timer=0;}  
      }
    }
    if(readcount[run_select][run_counter]==16){
      if(run_timer>=timer[run_select][run_counter]) {
        if(read1x==0){run_counter++;read1x=1;c_lock=1;en_timer=0;}  
      }
    }
    if(readcount[run_select][run_counter]==17){
      if(run_timer>=timer[run_select][run_counter]&&out_sensor==0) {
        if(read1x==0){run_counter++;read1x=1;c_lock=1;en_timer=0;}  
      }
    }
    if(read1x==1&&c_lock==1&&counter[run_select][run_counter-1]==1){
      led_lcd(0,lcdinvert);
      lost_code=1;
      if(readcount[run_select][run_counter-1]==1){if(sensor1==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==2){if(sensor2==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==3){if(sensor3==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==4){if(sensor4==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==5){if(sensor5==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==6){if(sensor11==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==7){if(sensor22==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==8){if(sensor33==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==9){if(sensor44==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==10){if(sensor55==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==11){if(sensor1==0&&sensor11==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==12){if(sensor2==0&&sensor22==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==13){if(sensor3==0&&sensor33==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==14){if(sensor4==0&&sensor44==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==15){if(sensor5==0&&sensor55==0){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}}
      if(readcount[run_select][run_counter-1]==16){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}
      if(readcount[run_select][run_counter-1]==17){clock_timer=0;run_timer=0;read1x=0;c_lock=0;lost_code=0;en_timer=1;}
    }
    if(read1x==1&&c_lock==1&&counter[run_select][run_counter-1]==2){
      led_lcd(0,lcdinvert);
      mode_left();
      c_lock=0;
    }
    if(read1x==1&&c_lock==1&&counter[run_select][run_counter-1]==3){
      led_lcd(0,lcdinvert);
      mode_right();
      c_lock=0;
    }
    if(read1x==1&&c_lock==1&&counter[run_select][run_counter-1]==4){
      led_lcd(0,lcdinvert);
      mode_stop();
    }
    if(read1x==1&&c_lock==1&&counter[run_select][run_counter-1]==5){
      led_lcd(0,lcdinvert);
      mode_free();
      read1x=0;
      c_lock=0;
    }
    if(read1x==1&&c_lock==1&&counter[run_select][run_counter-1]==6){
      led_lcd(0,lcdinvert);
      digitalWrite(relay,HIGH);
      mode_free();
      digitalWrite(relay,LOW);
      read1x=0;
      c_lock=0;
    }
  }
}

// Main Loop Of Ardu Panzer
void output(){
  if(monitor==1) lcd_clear(5);
  if(active_ok==1) mode_counter();
  // Running mode 0
  if(mode==0){
    pv_in=sensorfix(linemode);
    pv_out=error_out(pv_in); 
  }
  
  error=(int)sp-pv_out;
  out_pid();
  LOUT=(int)pwm-out_p-out_i-out_d;
  ROUT=(int)pwm+out_p+out_i+out_d;  
  if(LOUT>255) LOUT=255;
  if(LOUT<-255) LOUT=-255;
  if(ROUT>255) ROUT=255;
  if(ROUT<-255) ROUT=-255;

  // Running mode 1
  if(mode==1){
    if(LOUT>maxpwm[run_select][run_counter]) LOUT=maxpwm[run_select][run_counter];
    if(LOUT<-maxpwm[run_select][run_counter]) LOUT=-maxpwm[run_select][run_counter];
    if(ROUT>maxpwm[run_select][run_counter]) ROUT=maxpwm[run_select][run_counter];
    if(ROUT<-maxpwm[run_select][run_counter]) ROUT=-maxpwm[run_select][run_counter]; 

    if(lock_count==1||lock_count==2){
      LOUT=memo_l;  
      ROUT=memo_r; 
      led_lcd(0,lcdinvert);
    } else {
      led_lcd(1,lcdinvert);
    }
  }
  
  set_motors(LOUT,ROUT,allmaxpwm,triml,trimr);

  if(sw3()==0){ monitor=1;led_lcd(0,lcdinvert); }
  if(sw4()==0){ monitor=0;led_lcd(1,lcdinvert); }

  lcd.setCursor(0,1);
  lcd.print(LOUT);
  lcd.print(" ");
  lcd.print(ROUT);
  lcd.print(" ");
  if(mode==0) lcd.print(pv_out);
  if(mode==1 && active_ok==1){
    lcd.print(" ");
    lcd.print(run_counter);
    lcd.print(" ");
    lcd.print(run_timer);
  }
}
// End of main loop ardupanzer

void mydelay(unsigned int mytime){
  unsigned int d_count=0; 
  while(d_count<mytime){
    d_count++;
    delay(p_delay);
  }
}

void setting_cp(){
  int i,j; 
  i=cpcount-1;
  j=count-1;
  delay(loading);
  lcd.clear();
  while(1){
    fast=(speed_setting*2)+200;   
    if(sw3()==0) run_cp++;
    if(sw4()==0) run_cp--; 
    if(sw1()==0) run_counter++;
    if(sw2()==0) run_counter--; 
    if(run_cp>i) run_cp=0;
    if(run_cp<0) run_cp=i;
    if(run_counter>j) run_counter=0;
    if(run_counter<0) run_counter=j;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Chek Point: ");
    lcd.print(run_cp);
    lcd.setCursor(0,1);
    lcd.print("Counter: ");
    lcd.print(run_counter);
    delay(fast);
    if(sw5()==0) break;
  } 
  if(run_counter==0&&run_cp>0){
    run_counter=readpoint[run_select][run_cp-1];
    timer[run_select][run_counter]=timerpoint[run_select][run_cp-1];
  }
}
