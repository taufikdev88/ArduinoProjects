/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 3/13/2014
Author  : Eko Henfri Binugroho - modified from twimaster.c made by pfleury@gmx.ch  
Company : ER2C
Code    : I2C Access functions  
*********************************************************/

#ifndef _myI2C
#define _myI2C   1

// Isi register TWCR --> TWI Control Register
#define     TWINT                   7
#define     TWEA                    6
#define     TWSTA                   5
#define     TWSTO                   4
#define     TWWC                    3
#define     TWEN                    2
#define     TWIE                    0

#define     TW_START                0x08
#define     TW_REP_START            0x10
#define     TW_MT_SLA_ACK           0x18
#define     TW_MT_SLA_NACK          0x20
#define     TW_MT_DATA_ACK          0x28
#define     TW_MT_DATA_NACK         0x30
#define     TW_MT_ARB_LOST          0x38
#define     TW_MR_ARB_LOST          0x38
#define     TW_MR_SLA_ACK           0x40
#define     TW_MR_SLA_NACK          0x48
#define     TW_MR_DATA_ACK          0x50
#define     TW_MR_DATA_NACK         0x58
#define     TW_ST_SLA_ACK           0xA8
#define     TW_ST_ARB_LOST_SLA_ACK  0xB0
#define     TW_ST_DATA_ACK          0xB8
#define     TW_ST_DATA_NACK         0xC0
#define     TW_ST_LAST_DATA         0xC8
#define     TW_SR_SLA_ACK           0x60
#define     TW_SR_ARB_LOST_SLA_ACK  0x68
#define     TW_SR_GCALL_ACK         0x70
#define     TW_SR_ARB_LOST_GCALL_ACK   0x78
#define     TW_SR_DATA_ACK          0x80
#define     TW_SR_DATA_NACK         0x88
#define     TW_SR_GCALL_DATA_ACK    0x90
#define     TW_SR_GCALL_DATA_NACK   0x98
#define     TW_SR_STOP              0xA0
#define     TW_NO_INFO              0xF8
#define     TW_BUS_ERROR            0x00
#define     TW_STATUS_MASK          0B11111000 
#define     TW_STATUS   (TWSR & TW_STATUS_MASK)

/** defines the data direction (reading from I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_READ    1

/** defines the data direction (writing to I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_WRITE   0

#pragma used+
//initialize the I2C master interace. Need to be called only once 
extern void I2C_Init(void);

//Terminates the data transfer and releases the I2C bus 
extern void I2C_Stop(void);

//Issues a start condition and sends address and transfer direction 
//  @param    addr address and transfer direction of I2C device
//  @retval   0   device accessible 
//  @retval   1   failed to access device 
extern uint8_t I2C_Start(uint8_t address);

//Issues a repeated start condition and sends address and transfer direction 
//  @param   addr address and transfer direction of I2C device
//  @retval  0 device accessible
//  @retval  1 failed to access device
extern uint8_t I2C_Rep_Start(uint8_t address);

//Issues a start condition and sends address and transfer direction 
//If device is busy, use ack polling to wait until device ready 
//  @param    addr address and transfer direction of I2C device
//  @return   none
extern void I2C_Start_Wait(uint8_t address);

//Send one byte to I2C device
//  @param    data  byte to be transfered
//  @retval   0 write successful
//  @retval   1 write failed
extern uint8_t I2C_Write(uint8_t data);

//read one byte from the I2C device, request more data from device 
//  @return   byte read from I2C device
extern uint8_t I2C_ReadAck(void);
 
//read one byte from the I2C device, read is followed by a stop condition 
// @return   byte read from I2C device
extern uint8_t I2C_ReadNak(void);
#pragma used-

//read one byte from the I2C device
//Implemented as a macro, which calls either I2CreadAck or I2CreadNak
// @param    ack 1 send ack, request more data from device<br>
//               0 send nak, read is followed by a stop condition 
// @return   byte read from I2C device
//extern unsigned char I2C_Read(unsigned char ack);
//#define I2C_Read(ack)  (ack) ? I2C_ReadAck() : I2C_ReadNak(); 

#include <mega128.h>

/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU (uint32_t) 16000000          // 16 Mhz
#endif

/* I2C clock in Hz */
#define SCL_CLOCK  (uint32_t) 100000      // 200 Khz

/*************************************************************************
  Initialization of the I2C bus interface. Need to be called only once
*************************************************************************/
void I2C_Init(void)
{
/* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
    TWSR = 0;                         /* no prescaler */
    TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */
}/* I2Cinit */


/*************************************************************************      
Issues a start condition and sends address and transfer direction.
return 0 = device accessible, 1= failed to access device
*************************************************************************/
uint8_t I2C_Start(uint8_t address)
{   uint8_t   twst;
// send START condition
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
// wait until transmission completed
    while(!(TWCR & (1<<TWINT)));
// check value of TWI Status Register. Mask prescaler bits.
    twst = TW_STATUS & 0xF8;
    if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;
        // send device address
         TWDR = address;
         TWCR = (1<<TWINT) | (1<<TWEN);
         // wail until transmission completed and ACK/NACK has been received
         while(!(TWCR & (1<<TWINT)));
         // check value of TWI Status Register. Mask prescaler bits.
         twst = TW_STATUS & 0xF8;
         if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;
         return 0;
}/* I2Cstart */

/*************************************************************************
Issues a start condition and sends address and transfer direction.
If device is busy, use ack polling to wait until device is ready  
Input:   address and transfer direction of I2C device
*************************************************************************/
void I2C_Start_Wait(uint8_t address)
{
     uint8_t   twst;
     while ( 1 )
     {  // send START condition
        TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
        // wait until transmission completed
        while(!(TWCR & (1<<TWINT)));
         // check value of TWI Status Register. Mask prescaler bits.
         twst = TW_STATUS & 0xF8;
         if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
         // send device address
         TWDR = address;
         TWCR = (1<<TWINT) | (1<<TWEN);
         // wail until transmission completed
         while(!(TWCR & (1<<TWINT)));
         // check value of TWI Status Register. Mask prescaler bits.
         twst = TW_STATUS & 0xF8;
         if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) 
         {           
             /* device busy, send stop condition to terminate write operation */
                 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
                 
                 // wait until stop condition is executed and bus released
                 while(TWCR & (1<<TWSTO));
                 continue;
         }
         //if( twst != TW_MT_SLA_ACK) return 1;
         break;
      }
}/* I2Cstart_wait */

/*************************************************************************
  Issues a repeated start condition and sends address and transfer direction 
  Input:   address and transfer direction of I2C device
  Return:  0 device accessible
           1 failed to access device
*************************************************************************/
uint8_t I2C_Rep_Start(uint8_t address)
{
    return I2C_Start( address );
}/* I2Crep_start */
#pragma used-

/*************************************************************************
  Terminates the data transfer and releases the I2C bus
*************************************************************************/
void I2C_Stop(void)
{
     /* send stop condition */
         TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
         // wait until stop condition is executed and bus released
         while(TWCR & (1<<TWSTO));
}/* I2Cstop */

/*************************************************************************
Send one byte to I2C device
   Input:    byte to be transfered
   Return:   0 write successful 
             1 write failed
*************************************************************************/
uint8_t I2C_Write( uint8_t data )
{       
         uint8_t   twst;
         // send data to the previously addressed device
         TWDR = data;
         TWCR = (1<<TWINT) | (1<<TWEN);
 
         // wait until transmission completed
         while(!(TWCR & (1<<TWINT)));
         // check value of TWI Status Register. Mask prescaler bits
         twst = TW_STATUS & 0xF8;
         if( twst != TW_MT_DATA_ACK) return 1;
         return 0;
 
}/* I2Cwrite */

/*************************************************************************
  Read one byte from the I2C device, request more data from device 
  Return:  byte read from I2C device
*************************************************************************/
uint8_t I2C_ReadAck(void)
{
         TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
         while(!(TWCR & (1<<TWINT)));    
 
     return TWDR;
 
}/* I2CreadAck */

/*************************************************************************
  Read one byte from the I2C device, read is followed by a stop condition 
  Return:  byte read from I2C device
*************************************************************************/
uint8_t I2C_ReadNak(void)
{
         TWCR = (1<<TWINT) | (1<<TWEN);
         while(!(TWCR & (1<<TWINT)));
         
     return TWDR;
 
}/* I2CreadNak */

#endif