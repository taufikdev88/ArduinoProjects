#include "Initialization.h"
#include <avr/eeprom.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>

const char *my_directory="PANZER";
const char *my_file="/PANZER/PANZER.TXT";
const char *my_dirfile="/PANZER/";
const char *my_openfile="/PANZER";
char mybuf[25];
char mybuf1[25];
char mybuf2[25];
char selectfile[15];

void setup()
{
// Initialization
lcd.begin(16, 2); 
adc_init();
output_setup();
input_setup();
lcd_bar_init();
eeprom_init();
read_activation();
read_sensor();
read_recent_read();
read_recent_write();
read_pid();
read_modenormal();
read_readpoint();
read_timerpoint();
read_sdmemory();
read_kpcount();
read_counter();
read_timer();
read_delaycount();
read_stopcount();
read_vcountl();
read_vcountr();
read_maxpwm();
read_readcount();
read_linecount();
read_lineselect();
read_vcount();
read_vcount2();
read_speed1();
read_speed2();
speed_setting=-50;
activation();
if(sw1()==0)setting_activation();
idle();
pwm_on();
timer_init();
sei();
}

void loop()
{
output();  
}

void format_dir(){
  int i,j,k=0;
  File data;
  
  delay(loading);
  lcd.clear();
  while(1){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ok~ Exit");  
  lcd.setCursor(0,1);
  lcd.print("Cancel~ M+"); 
  if(sw1()==0){k=1;break;}
  if(sw5()==0)break; 
  delay(500);
  }
  if(k==1)goto cancel;
  clear_mybuf();
  sd_init();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Formatting...");
j=strlen(my_directory);
for(i=0;i<j;i++){mybuf[i]=my_directory[i];}
if(!SD.exists(mybuf)){
SD.mkdir(mybuf);
j=strlen(my_file);
for(i=0;i<j;i++){mybuf[i]=my_file[i];}
if(!SD.exists(mybuf))SD.open(mybuf, FILE_WRITE);
data=SD.open(mybuf, FILE_WRITE);
data.println("Config <ArduPanzerMegaLF>");
data.println("By Latief Perdana");
data.println();
data.println("Syntax");
data.println("Mode = Counter/Normal");
data.println("Line = Auto/Black/White");
data.println("Run  = Normal/Mirror");
data.println("*");
for(i=0;i<16;i++){
data.print("Sensor"); 
data.print(String(i)); 
if(i<=9)data.print("  : ");
else data.print(" : ");
data.print('"'); 
data.print(50); 
data.println('"'); 
}
data.println('#');

data.println();
data.println("*");
data.print("P Kp: "); 
data.print('"');
data.print(10);
data.println('"');
data.print("I Ki: "); 
data.print('"');
data.print(0);
data.println('"');
data.print("D Kd: "); 
data.print('"');
data.print(50);
data.println('"');
data.print("Imax: "); 
data.print('"');
data.print(100);
data.println('"');
data.println('#');

data.println();
data.println("*");
data.print("Speed      : "); 
data.print('"');
data.print(100);
data.println('"');
data.print("Mode       : "); 
data.print('"');
data.print("Normal");
data.println('"');
data.print("Line       : "); 
data.print('"');
data.print("Black");
data.println('"');
data.print("Timer Speed: "); 
data.print('"');
data.print(5);
data.println('"');
data.print("Max Pwm    : "); 
data.print('"');
data.print(255);
data.println('"');
data.print("Trim L     : "); 
data.print('"');
data.print(0);
data.println('"');
data.print("Trim R     : "); 
data.print('"');
data.print(0);
data.println('"');
data.print("Run        : "); 
data.print('"');
data.print("Normal");
data.println('"');
data.println('#');

data.println();
data.println("Syntax");
data.println("Read Counter= L 1/L 2/L 3/L 4/L 5/R 1/R 2/R 3/R 4/R 5/LR 1/LR 2/LR 3/LR 4/LR 5/Timer/Blank");
data.println("Counter     = Lost/Left/Right/Stop/Free/Out");
data.println("Line Counter= Auto/Black/White");
data.println("PID Counter = Normal/Center/Left/Right");
data.println();

data.println("*");
for(i=1;i<=count;i++){
data.print(i);
if(i<10)data.print("   "); 
if(i>=10&&i<100)data.print("  "); 
if(i>=100)data.print(' '); 
data.print("Kp: "); 
data.print('"');
data.print(10);
data.print('"');
data.print(' ');
data.print("Timer: "); 
data.print('"');
data.print(0);
data.print('"');
data.print(' ');
data.print("Speed1: "); 
data.print('"');
data.print(100);
data.print('"');
data.print(' ');
data.print("Speed2: "); 
data.print('"');
data.print(100);
data.print('"');
data.print(' ');
data.print("Count: "); 
data.print('"');
data.print("Lost");
data.print('"');
data.print(' ');
data.print("Read: "); 
data.print('"');
data.print("L 1");
data.print('"');
data.print(' ');
data.print("Line: "); 
data.print('"');
data.print("Black");
data.print('"');
data.print(' ');
data.print("PID: "); 
data.print('"');
data.print("Normal");
data.print('"');
data.print(' ');
data.print("Delay: "); 
data.print('"');
data.print(10);
data.print('"');
data.print(' ');
data.print("Stop: "); 
data.print('"');
data.print(0);
data.print('"');
data.print(' ');
data.print("Free L: "); 
data.print('"');
data.print(128);
data.print('"');
data.print(' ');
data.print("Free R: "); 
data.print('"');
data.print(128);
data.print('"');
data.print(' ');
data.print("Vc L: "); 
data.print('"');
data.print(188);
data.print('"');
data.print(' ');
data.print("Vc R: "); 
data.print('"');
data.print(188);
data.print('"');
data.print(' ');
data.print("MaxPwm: "); 
data.print('"');
data.print(255);
data.println('"');
}
data.println('#');

data.println(); 
data.println("Chek Point "); 
data.println("*");
for(i=1;i<=cpcount;i++){  
data.print(i);
if(i<10)data.print("  "); 
else data.print(' ');
data.print("Plan Read: ");  
data.print('"');
data.print(0);
data.print('"');
data.print(' ');
data.print("Plan Timer: ");  
data.print('"');
data.print(0);
data.println('"');
}
data.println('#');

data.println();
data.println("End Setting"); 

  data.close();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Format Done");
  lcd.setCursor(0,1);
  lcd.print("Reset Your CPU"); 
  while(1); 
cancel:
lcd.clear();
}
else{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dir Not Exists"); 
  lcd.setCursor(0,1);
  lcd.print("Reset Your CPU"); 
  while(1);  
}
}

void sd_init(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Read SDCard"); 
  delay(loading); 
  pinMode(10, OUTPUT);
if (!SD.begin(10)) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SDCard Error"); 
  lcd.setCursor(0,1);
  lcd.print("Reset Your CPU"); 
  while(1); 
  }
  else{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SDCard Ok"); 
  delay(loading); 
  }
}

void clear_mybuf(){
int i;
for(i=0;i<25;i++){mybuf[i]=0;}
}
void clear_mybuf1(){
int i;
for(i=0;i<25;i++){mybuf1[i]=0;}
}
void clear_mybuf2(){
int i;
for(i=0;i<25;i++){mybuf2[i]=0;}
}

void setting_writefile(){
  int i,j,k,l=0;
  File data;
  lcd.clear();
  delay(loading);
  clear_mybuf();
  clear_mybuf1();
  k=sdselect-1;
  while(1){
  fast=(speed_setting*2)+200; 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("To SDcard");
  lcd.setCursor(0,1);
  lcd.print(l+1);
  lcd.print(".");
  lcd.print(mybuf1);
  if(sw3()==0)l++;
  if(sw4()==0)l--;
  if(l>k)l=0;
  if(l<0)l=k;
  if(sw5()==0)break;
  for(i=0;i<sdcount;i++){mybuf1[i]=sdmemory[l][i];} 
  recent_write=l+1;
  delay(fast); 
  }
  clear_mybuf();
  clear_mybuf1();
  sd_init();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Coppy To SD");
j=strlen(my_dirfile);
for(i=0;i<j;i++){mybuf[i]=my_dirfile[i];}
for(i=0;i<sdcount;i++){mybuf1[i]=sdmemory[l][i];}
k=strlen(mybuf1);
for(i=0;i<k;i++){mybuf[j]=mybuf1[i];j++;}
if(SD.exists(mybuf))SD.remove(mybuf);
else {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("File Not Found"); 
  lcd.setCursor(0,1);
  lcd.print("Reset Your CPU"); 
  while(1);   
}
if(!SD.exists(mybuf))SD.open(mybuf, FILE_WRITE);
data=SD.open(mybuf, FILE_WRITE);
data.println("Config <ArduPanzerMegaLF>");
data.println("By Latief Perdana");
data.println();
data.println("Syntax");
data.println("Mode = Counter/Normal");
data.println("Line = Auto/Black/White");
data.println("Run  = Normal/Mirror");
data.println("*");
for(i=0;i<16;i++){
data.print("Sensor"); 
data.print(String(i)); 
if(i<=9)data.print("  : ");
else data.print(" : ");
data.print('"'); 
data.print(sensor[i]); 
data.println('"'); 
}
data.println('#');

data.println();
data.println("*");
data.print("P Kp: "); 
data.print('"');
data.print(kp);
data.println('"');
data.print("I Ki: "); 
data.print('"');
data.print(ki);
data.println('"');
data.print("D Kd: "); 
data.print('"');
data.print(kd);
data.println('"');
data.print("Imax: "); 
data.print('"');
data.print(imax);
data.println('"');
data.println('#');

data.println();
data.println("*");
data.print("Speed      : "); 
data.print('"');
data.print(pwm);
data.println('"');
data.print("Mode       : "); 
data.print('"');
if(mode==0)data.print("Normal");
else data.print("Counter");
data.println('"');
data.print("Line       : "); 
data.print('"');
if(linemode==0)data.print("Auto");
if(linemode==1)data.print("Black");
if(linemode==2)data.print("White");
data.println('"');
data.print("Timer Speed: "); 
data.print('"');
data.print(timer_speed);
data.println('"');
data.print("Max Pwm    : "); 
data.print('"');
data.print(allmaxpwm);
data.println('"');
data.print("Trim L     : "); 
data.print('"');
data.print(triml);
data.println('"');
data.print("Trim R     : "); 
data.print('"');
data.print(trimr);
data.println('"');
data.print("Run        : "); 
data.print('"');
if(run_mirror==0)data.print("Normal");
else data.print("Mirror");
data.println('"');
data.println('#');

data.println();
data.println("Syntax");
data.println("Read Counter= L 1/L 2/L 3/L 4/L 5/R 1/R 2/R 3/R 4/R 5/LR 1/LR 2/LR 3/LR 4/LR 5/Timer/Blank");
data.println("Counter     = Lost/Left/Right/Stop/Free/Out");
data.println("Line Counter= Auto/Black/White");
data.println("PID Counter = Normal/Center/Left/Right");
data.println();

data.println("*");
for(i=1;i<=count;i++){
data.print(i);
if(i<10)data.print("   "); 
if(i>=10&&i<100)data.print("  "); 
if(i>=100)data.print(' '); 
data.print("Kp: "); 
data.print('"');
data.print(kpcount[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Timer: "); 
data.print('"');
data.print(timer[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Speed1: "); 
data.print('"');
data.print(speed1[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Speed2: "); 
data.print('"');
data.print(speed2[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Count: "); 
data.print('"');
if(counter[run_select][i-1]==1)data.print("Lost");
if(counter[run_select][i-1]==2)data.print("Left");
if(counter[run_select][i-1]==3)data.print("Right");
if(counter[run_select][i-1]==4)data.print("Stop");
if(counter[run_select][i-1]==5)data.print("Free");
if(counter[run_select][i-1]==6)data.print("Out");
data.print('"');
data.print(' ');
data.print("Read: "); 
data.print('"');
if(readcount[run_select][i-1]==1)data.print("L 1");
if(readcount[run_select][i-1]==2)data.print("L 2");
if(readcount[run_select][i-1]==3)data.print("L 3");
if(readcount[run_select][i-1]==4)data.print("L 4");
if(readcount[run_select][i-1]==5)data.print("L 5");
if(readcount[run_select][i-1]==6)data.print("R 1");
if(readcount[run_select][i-1]==7)data.print("R 2");
if(readcount[run_select][i-1]==8)data.print("R 3");
if(readcount[run_select][i-1]==9)data.print("R 4");
if(readcount[run_select][i-1]==10)data.print("R 5");
if(readcount[run_select][i-1]==11)data.print("LR 1");
if(readcount[run_select][i-1]==12)data.print("LR 2");
if(readcount[run_select][i-1]==13)data.print("LR 3");
if(readcount[run_select][i-1]==14)data.print("LR 4");
if(readcount[run_select][i-1]==15)data.print("LR 5");
if(readcount[run_select][i-1]==16)data.print("Timer");
if(readcount[run_select][i-1]==17)data.print("Blank");
data.print('"');
data.print(' ');
data.print("Line: "); 
data.print('"');
if(linecount[run_select][i-1]==1)data.print("Auto");
if(linecount[run_select][i-1]==2)data.print("Black");
if(linecount[run_select][i-1]==3)data.print("White");
data.print('"');
data.print(' ');
data.print("PID: "); 
data.print('"');
if(lineselect[run_select][i-1]==1)data.print("Normal");
if(lineselect[run_select][i-1]==2)data.print("Center");
if(lineselect[run_select][i-1]==3)data.print("Left");
if(lineselect[run_select][i-1]==4)data.print("Right");
data.print('"');
data.print(' ');
data.print("Delay: "); 
data.print('"');
data.print(delaycount[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Stop: "); 
data.print('"');
data.print(stopcount[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Free L: "); 
data.print('"');
data.print(vcount[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Free R: "); 
data.print('"');
data.print(vcount2[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Vc L: "); 
data.print('"');
data.print(vcountl[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Vc R: "); 
data.print('"');
data.print(vcountr[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("MaxPwm: "); 
data.print('"');
data.print(maxpwm[run_select][i-1]);
data.println('"');
}
data.println('#');

data.println(); 
data.println("Chek Point "); 
data.println("*");
for(i=1;i<=cpcount;i++){  
data.print(i);
if(i<10)data.print("  "); 
else data.print(' ');
data.print("Plan Read: ");  
data.print('"');
data.print(readpoint[run_select][i-1]);
data.print('"');
data.print(' ');
data.print("Plan Timer: ");  
data.print('"');
data.print(timerpoint[run_select][i-1]);
data.println('"');
}
data.println('#');

data.println();
data.println("End Setting"); 

data.close();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Coppy Done");
  lcd.setCursor(0,1);
  lcd.print("Reset Your CPU"); 
  save_recent_write();
  while(1); 
  
}

void scanfile(String typefile) {
  int i,j,k=0,l=0,sd=0,sel=0,sv=0;
  char buffer[13];
  char buffer1[4];
   String bufferstring,bufferstring1;
  File dir;
  clear_mybuf();
  sd_init();
  dir = SD.open(my_openfile);
  fast=(speed_setting*2)+200;   
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Read File");
 delay(loading);
   while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       break;
     }
     bufferstring=entry.name();
     if(typefile=="ALL"){
     bufferstring1=bufferstring;
     for(i=0;i<14;i++){
         selectfile[i]=bufferstring1[i];
        } 
     }
     else{
     for(i=0;i<13;i++){
     buffer[i]=bufferstring[i]; 
      }
      for(i=0;i<4;i++){
     buffer1[i]=typefile[i];
      }
      i=strlen(buffer)-1;
      j=3;
      while(true) {
      if(buffer[i]==buffer1[j])j--;  
      i--;
      if(j<=0){
       bufferstring1=bufferstring;
       k++;
       break;
      }
      
    if(i<=0)break; 
    
      } 
     }
     
     if(k>0&&l!='2'){ 
      
       while(true){
         if(sw5()==0){sv=1;goto save;}
         l=0; 
       if(sw3()==0)sel++;
       if(sw4()==0)sel--;
       if(sel>(sdselect-1))sel=0;
       if(sel<0)sel=sdselect-1;
       if(sw1()==0)l='1';
       if(sw2()==0){
          for(i=0;i<14;i++){
          sdmemory[sel][i]=bufferstring1[i]; 
          }             
         }
       for(i=0;i<sdcount;i++){mybuf[i]=sdmemory[sel][i];}
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print(sd+1); 
       lcd.print(".");  
       lcd.print(bufferstring1);  
       lcd.setCursor(0,1); 
       lcd.print(sel+1); 
       lcd.print(".");   
       lcd.print(mybuf);        
       delay(fast);
       if(l=='1'){sd++;break;}
       if(l=='2'){
       for(i=0;i<14;i++){
         selectfile[i]=bufferstring1[i]; 
        }  
       break;}
       } 
       
     }
      entry.close();
   }
save:
if(sv==1){
  lcd.clear();
  save_sdmemory();
  lcd.setCursor(0,0);
  lcd.print("Saving Done"); 
  lcd.setCursor(0,1);
  lcd.print("Reset Your CPU"); 
  while(1); 
}
else {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Not Select"); 
  lcd.setCursor(0,1);
  lcd.print("Reset Your CPU"); 
  while(1);   
}
 lcd.clear();
}

void setting_readfile(){
  int i=0,j=0,k,l=0,end_read=0;
  int file_true=0,add=0,my_scn=0,my_scn1=0;
  const unsigned int end_cnt_h=15;
  const unsigned int end_cp_h=2;
  char ascii;
  File data;
  lcd.clear();
  delay(loading);
  clear_mybuf();
  clear_mybuf1();
  k=sdselect-1;
  while(1){
  fast=(speed_setting*2)+200; 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("To Eeprom");
  lcd.setCursor(0,1);
  lcd.print(l+1);
  lcd.print(".");
  lcd.print(mybuf1);
  if(sw3()==0)l++;
  if(sw4()==0)l--;
  if(l>k)l=0;
  if(l<0)l=k;
  if(sw5()==0)break;
  for(i=0;i<sdcount;i++){mybuf1[i]=sdmemory[l][i];} 
  recent_read=l+1;
  delay(fast); 
  }
  clear_mybuf();
  clear_mybuf1();
  clear_mybuf2();
  sd_init();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reading Txt...");
  delay(loading);
j=strlen(my_dirfile);
for(i=0;i<j;i++){mybuf[i]=my_dirfile[i];}
for(i=0;i<sdcount;i++){mybuf2[i]=sdmemory[l][i];}
k=strlen(mybuf2);
for(i=0;i<k;i++){mybuf[j]=mybuf2[i];j++;}
i=0;
k=0;
j=0;
l=0;
data=SD.open(mybuf);
while(data.available()){
  
while(file_true==0){
ascii=data.read(); 
if(ascii=='<'){
while(1){
  ascii=data.read(); 
  if(ascii=='>'||i>20)break;
mybuf1[i]=ascii;
i++;
}
}

if(ascii=='>'){
if(stringinput(mybuf1,"ArduPanzerMegaLF")==1)file_true=1;
else file_true=2;
}
if(i>20||j>20)file_true=2;
j++;
}  
if(file_true==2)goto exit;
if(file_true==1&&i!=0){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Coppy To Ram");
  i=0;
  j=0;
  clear_mybuf1();
}

ascii=data.read(); 

if(ascii=='*'&&add==0){
my_scn=0;
while(add==0){
clear_mybuf1();
i=0;
while(add==0){
  ascii=data.read();
  if(ascii=='#'){add++;break;}
  if(ascii=='"')break;
}
while(add==0){
  ascii=data.read(); 
  if(ascii=='#'){add++;break;}
  if(ascii=='"'){my_scn++;break;}
  mybuf1[i]=ascii;
  i++;  
}
if(i!=0&&ascii!='#'){sensor[my_scn-1]=atoi(mybuf1);}
}
}

if(ascii=='*'&&add==1){
my_scn=0;
while(add==1){
clear_mybuf1();
i=0;
while(add==1){
  ascii=data.read();
  if(ascii=='#'){add++;break;}
  if(ascii=='"')break;
}
while(add==1){
  ascii=data.read(); 
  if(ascii=='#'){add++;break;}
  if(ascii=='"'){my_scn++;break;}
  mybuf1[i]=ascii;
  i++;  
}
if(my_scn==1&&ascii!='#'){kp=atoi(mybuf1);}
if(my_scn==2&&ascii!='#'){ki=atoi(mybuf1);}
if(my_scn==3&&ascii!='#'){kd=atoi(mybuf1);}
if(my_scn==4&&ascii!='#'){imax=atoi(mybuf1);}
}
}

if(ascii=='*'&&add==2){
my_scn=0;
while(add==2){
clear_mybuf1();
i=0;
while(add==2){
  ascii=data.read();
  if(ascii=='#'){add++;break;}
  if(ascii=='"')break;
}
while(add==2){
  ascii=data.read(); 
  if(ascii=='#'){add++;break;}
  if(ascii=='"'){my_scn++;break;}
  mybuf1[i]=ascii;
  i++;  
}
if(my_scn==1&&ascii!='#'){pwm=atoi(mybuf1);}
if(my_scn==2&&ascii!='#'){
if(stringinput(mybuf1,"Normal")==1)mode=0;  
if(stringinput(mybuf1,"Counter")==1)mode=1; 
}
if(my_scn==3&&ascii!='#'){
if(stringinput(mybuf1,"Auto")==1)linemode=0;
if(stringinput(mybuf1,"Black")==1)linemode=1;
if(stringinput(mybuf1,"White")==1)linemode=2;
}
if(my_scn==4&&ascii!='#'){timer_speed=atoi(mybuf1);}
if(my_scn==5&&ascii!='#'){allmaxpwm=atoi(mybuf1);}
if(my_scn==6&&ascii!='#'){triml=atoi(mybuf1);}
if(my_scn==7&&ascii!='#'){trimr=atoi(mybuf1);}
if(my_scn==8&&ascii!='#'){speed_setting1=atoi(mybuf1);}
if(my_scn==9&&ascii!='#'){
if(stringinput(mybuf1,"Normal")==1)run_mirror=0;
if(stringinput(mybuf1,"Mirror")==1)run_mirror=1;
}
}
}

if(ascii=='*'&&add==3){
my_scn=0;
my_scn1=0;
while(add==3){
clear_mybuf1();
i=0;
while(add==3){
  ascii=data.read();
  if(ascii=='#'){add++;break;}
  if(ascii=='"')break;
}
while(add==3){
  ascii=data.read(); 
  if(ascii=='#'){add++;break;}
  if(ascii=='"'){my_scn++;break;}
  mybuf1[i]=ascii;
  i++;  
}
if(my_scn==1&&ascii!='#'){kpcount[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==2&&ascii!='#'){timer[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==3&&ascii!='#'){speed1[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==4&&ascii!='#'){speed2[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==5&&ascii!='#'){
if(stringinput(mybuf1,"Lost")==1)counter[run_select][my_scn1]=1;
if(stringinput(mybuf1,"Left")==1)counter[run_select][my_scn1]=2;
if(stringinput(mybuf1,"Right")==1)counter[run_select][my_scn1]=3;
if(stringinput(mybuf1,"Stop")==1)counter[run_select][my_scn1]=4;
if(stringinput(mybuf1,"Free")==1)counter[run_select][my_scn1]=5;
if(stringinput(mybuf1,"Out")==1)counter[run_select][my_scn1]=6;
}
if(my_scn==6&&ascii!='#'){
if(stringinput(mybuf1,"L 1")==1)readcount[run_select][my_scn1]=1;
if(stringinput(mybuf1,"L 2")==1)readcount[run_select][my_scn1]=2;
if(stringinput(mybuf1,"L 3")==1)readcount[run_select][my_scn1]=3;
if(stringinput(mybuf1,"L 4")==1)readcount[run_select][my_scn1]=4;
if(stringinput(mybuf1,"L 5")==1)readcount[run_select][my_scn1]=5;
if(stringinput(mybuf1,"R 1")==1)readcount[run_select][my_scn1]=6;
if(stringinput(mybuf1,"R 2")==1)readcount[run_select][my_scn1]=7;
if(stringinput(mybuf1,"R 3")==1)readcount[run_select][my_scn1]=8;
if(stringinput(mybuf1,"R 4")==1)readcount[run_select][my_scn1]=9;
if(stringinput(mybuf1,"R 5")==1)readcount[run_select][my_scn1]=10;
if(stringinput(mybuf1,"LR 1")==1)readcount[run_select][my_scn1]=11;
if(stringinput(mybuf1,"LR 2")==1)readcount[run_select][my_scn1]=12;
if(stringinput(mybuf1,"LR 3")==1)readcount[run_select][my_scn1]=13;
if(stringinput(mybuf1,"LR 4")==1)readcount[run_select][my_scn1]=14;
if(stringinput(mybuf1,"LR 5")==1)readcount[run_select][my_scn1]=15;
if(stringinput(mybuf1,"Timer")==1)readcount[run_select][my_scn1]=16;
if(stringinput(mybuf1,"Blank")==1)readcount[run_select][my_scn1]=17;
}
if(my_scn==7&&ascii!='#'){
if(stringinput(mybuf1,"Auto")==1)linecount[run_select][my_scn1]=1;
if(stringinput(mybuf1,"Black")==1)linecount[run_select][my_scn1]=2;
if(stringinput(mybuf1,"White")==1)linecount[run_select][my_scn1]=3;
}
if(my_scn==8&&ascii!='#'){
if(stringinput(mybuf1,"Normal")==1)lineselect[run_select][my_scn1]=1;
if(stringinput(mybuf1,"Center")==1)lineselect[run_select][my_scn1]=2;
if(stringinput(mybuf1,"Left")==1)lineselect[run_select][my_scn1]=3;
if(stringinput(mybuf1,"Right")==1)lineselect[run_select][my_scn1]=4;
}
if(my_scn==9&&ascii!='#'){delaycount[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==10&&ascii!='#'){stopcount[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==11&&ascii!='#'){vcount[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==12&&ascii!='#'){vcount2[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==13&&ascii!='#'){vcountl[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==14&&ascii!='#'){vcountr[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==15&&ascii!='#'){maxpwm[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn>=end_cnt_h){my_scn1++;my_scn=0;}
}
}

if(ascii=='*'&&add==4){
my_scn=0;
my_scn1=0;
while(add==4){
clear_mybuf1();
i=0;
while(add==4){
  ascii=data.read();
  if(ascii=='#'){end_read=1;add++;break;}
  if(ascii=='"')break;
}
while(add==4){
  ascii=data.read(); 
  if(ascii=='#'){end_read=1;add++;break;}
  if(ascii=='"'){my_scn++;break;}
  mybuf1[i]=ascii;
  i++;  
}
if(my_scn==1&&ascii!='#'){readpoint[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn==2&&ascii!='#'){timerpoint[run_select][my_scn1]=atoi(mybuf1);}
if(my_scn>=end_cp_h){my_scn1++;my_scn=0;}
}
}

    }
    data.close();
exit:
if(file_true==1&&end_read==1){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Save To Eeprom");
auto_set_vc();
save_sensor();
save_recent_read();
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
  lcd.print("Saving Done");
  lcd.setCursor(0,1);
  lcd.print("Reset Your CPU"); 
  while(1); 
}
else{
    lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("File Not Found");
  lcd.setCursor(0,1);
  lcd.print("Reset Your CPU"); 
  while(1); 
}
  
}

char stringinput(char *in, char *param){
  int i=0,j=0,k=0,l=0;
  char ok;
j=strlen(in);
l=strlen(param);

while(true){
if(in[i]==param[i]){k++;}
i++;
if(i>=j){

if(k==i&&l==j)ok=1;
else ok=0;
break;
}
}
return ok;
}

