#include <SimpleModbusMaster.h>
#define baud 9600
#define timeout 1000
#define polling 200 // the scan rate
#define retry_count 100 
#define TxEnablePin 2

const int _id = 80;
unsigned long cur_get_rs485, old_get_rs485;

enum
{
  PACKET1,
  TOTAL_NO_OF_PACKETS
};
Packet packets[TOTAL_NO_OF_PACKETS];

packetPointer packet1 = &packets[PACKET1];

unsigned int regs[8];


float kwh; float tap;
float van, vbn, vcn, freq;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  packet1->id = 1;
  packet1->function = READ_INPUT_REGISTERS;
  packet1->address = 0;
  packet1->no_of_registers = 8;
  packet1->register_array = regs;

  modbus_configure(baud, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS);
  
}
void loop() {
  modbus_update(packets);
 Serial.println(regs[0]);//volt * 0.01v
 float teg = (regs[0])*0.01;
 Serial.println (teg);
 Serial.println(regs[1]);//arus * 0.01a
 Serial.println(regs[2]);//daya * 0.1w
 Serial.println(regs[4]);//energy wh
 Serial.println(regs[6]);//high alarm voltage
 Serial.println(regs[7]);//low alarm voltage
  delay(1000);
}
