void setup(){
 Serial.begin(115200); // Aktif Serial monitor
 Serial3.begin(115200); // Aktif Serial GY25
 Serial.println("kalibrasi");
 delay(3000);
 // Kalibrasi Tilt
 Serial3.write(0xA5);
 Serial3.write(0x54);
 delay(1000);
 Serial.println("kalibrasi Tilt done");
 // Kalibrasi Heading
 Serial3.write(0xA5);
 Serial3.write(0x55);
 delay(100);
 Serial.println("kalibrasi Heading done");
 // Output ASCII
 Serial3.write(0xA5);
 Serial3.write(0x53);
 delay(100);
}

struct gy25{
 char buffer[50];
 int counter;
 float heading = 0;
 float dHeading = 0;
} cmps;
float Roll,Pitch,Yaw;
unsigned char Re_buf[8],counter=0;

void imusense(){ 
//  Serial3.write(0xA5);
//  Serial3.write(0x51);//send it for each read
//  while(Serial3.available()) {   
//    Re_buf[counter]=(unsigned char)Serial3.read();
//    if(counter==0&&Re_buf[0]!=0xAA) return;        
//    counter++;       
//    if(counter==8)  // package is complete              
//    {    
//      counter=0;                  
//      if(Re_buf[0]==0xAA && Re_buf[7]==0x55)  // data package is correct      
//      {          
//         Yaw=(int16_t)(Re_buf[1]<<8|Re_buf[2])/100.00;   
//         Pitch=(int16_t)(Re_buf[3]<<8|Re_buf[4])/100.00;
//         Roll=(int16_t)(Re_buf[5]<<8|Re_buf[6])/100.00;
//      }      
//    }  
//    if(abs(Yaw) > 180)
//    {
//      Yaw = (Yaw > 0 ? -180 + (Yaw - 180) : 180 - (Yaw + 180));
//    }
//  }
//  Serial.println(Yaw);
  
  char tmp; // Variabel temporaryPortone.write(0XA5);
  while(Serial3.available()){
    tmp = Serial3.read();
    cmps.buffer[cmps.counter++] = tmp;
    if (tmp == '\n'){ // Langkah 1
      cmps.buffer[cmps.counter] = 0; // Karakter terminator
      cmps.dHeading = cmps.heading;
      float newHeading = atof(strtok(cmps.buffer+5, ",")); // Langkah 2-4 cmps.counter = 0;
      
      if(newHeading * cmps.dHeading < 0 && abs(cmps.dHeading) > 90){
        cmps.heading = (newHeading < 0 ? 180 + newHeading : 180 - newHeading); // dapat perbedaan
        cmps.heading = (cmps.dHeading < 0 ? -180 - newHeading : 180 + newHeading); // tambahkan 180 dengan perbedaannya
      } else {
        cmps.heading = newHeading;
      }
      
      cmps.counter = 0;
    }
  }
}

void loop(){
 // Baca data realtime
 imusense();
 Serial.println(cmps.heading);
 delay(50);
}
