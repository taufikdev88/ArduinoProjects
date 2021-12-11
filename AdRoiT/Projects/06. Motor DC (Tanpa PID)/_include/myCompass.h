#ifndef MYCOMPASS_H_INCLUDED
#define MYCOMPASS_H_INCLUDED

/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
          modified from I2Cdev library collection - HMC5883L I2C device class
          Based on Honeywell HMC5883L datasheet, 10/2010 (Form #900405 Rev B)
          6/12/2012 by Jeff Rowberg <jeff@rowberg.net>
Company : ER2C
Code    : Program perhitungan heading dengan kompas HMC5883L
*********************************************************/

#if UseCompass == 1

//definitions
#define HMC5883L_ADDR (0x1E<<1) //device address

#define HMC5883L_ADDRESS            0x1E // this device only has one address
#define HMC5883L_DEFAULT_ADDRESS    0x1E

#define HMC5883L_RA_CONFIG_A        0x00
#define HMC5883L_RA_CONFIG_B        0x01
#define HMC5883L_RA_MODE            0x02
#define HMC5883L_RA_DATAX_H         0x03
#define HMC5883L_RA_DATAX_L         0x04
#define HMC5883L_RA_DATAZ_H         0x05
#define HMC5883L_RA_DATAZ_L         0x06
#define HMC5883L_RA_DATAY_H         0x07
#define HMC5883L_RA_DATAY_L         0x08
#define HMC5883L_RA_STATUS          0x09
#define HMC5883L_RA_ID_A            0x0A
#define HMC5883L_RA_ID_B            0x0B
#define HMC5883L_RA_ID_C            0x0C

#define HMC5883L_CRA_AVERAGE_BIT    6
#define HMC5883L_CRA_AVERAGE_LENGTH 2
#define HMC5883L_CRA_RATE_BIT       4
#define HMC5883L_CRA_RATE_LENGTH    3
#define HMC5883L_CRA_BIAS_BIT       1
#define HMC5883L_CRA_BIAS_LENGTH    2

#define HMC5883L_AVERAGING_1        0x00
#define HMC5883L_AVERAGING_2        0x01
#define HMC5883L_AVERAGING_4        0x02
#define HMC5883L_AVERAGING_8        0x03

#define HMC5883L_RATE_0P75          0x00
#define HMC5883L_RATE_1P5           0x01
#define HMC5883L_RATE_3             0x02
#define HMC5883L_RATE_7P5           0x03
#define HMC5883L_RATE_15            0x04
#define HMC5883L_RATE_30            0x05
#define HMC5883L_RATE_75            0x06

#define HMC5883L_BIAS_NORMAL        0x00
#define HMC5883L_BIAS_POSITIVE      0x01
#define HMC5883L_BIAS_NEGATIVE      0x02

#define HMC5883L_CRB_GAIN_BIT       7
#define HMC5883L_CRB_GAIN_LENGTH    3

#define HMC5883L_GAIN_1370          0x00
#define HMC5883L_GAIN_1090          0x01
#define HMC5883L_GAIN_820           0x02
#define HMC5883L_GAIN_660           0x03
#define HMC5883L_GAIN_440           0x04
#define HMC5883L_GAIN_390           0x05
#define HMC5883L_GAIN_330           0x06
#define HMC5883L_GAIN_220           0x07

#define HMC5883L_MODEREG_BIT        1
#define HMC5883L_MODEREG_LENGTH     2

#define HMC5883L_MODE_CONTINUOUS    0x00
#define HMC5883L_MODE_SINGLE        0x01
#define HMC5883L_MODE_IDLE          0x02

#define HMC5883L_STATUS_LOCK_BIT    1
#define HMC5883L_STATUS_READY_BIT   0

class myCompass {
    public:
        int8_t  Init();

        // CONFIG_A register
        uint8_t GetSampleAveraging();
        void    SetSampleAveraging(uint8_t averaging);
        uint8_t GetDataRate();
        void    SetDataRate(uint8_t rate);
        uint8_t GetMeasurementBias();
        void    SetMeasurementBias(uint8_t bias);

        // CONFIG_B register
        uint8_t GetGain();
        void    SetGain(uint8_t gain);

        // MODE register
        uint8_t GetMode();
        void    SetMode(uint8_t mode);

        // DATA* registers
        void    GetHeading(int16_t *x, int16_t *y, int16_t *z);
        int16_t GetHeadingX();
        int16_t GetHeadingY();
        int16_t GetHeadingZ();

        // STATUS register
        bool    GetLockStatus();
        bool    GetReadyStatus();


    private:
        // ID_* registers
        void    StartBacaRegister(uint8_t Register);
        uint8_t GetIDA();
        uint8_t GetIDB();
        uint8_t GetIDC();
        int8_t  TestConnection(void);
        uint8_t DevAddr;
        uint8_t mode;
}kompas;


int8_t myCompass::TestConnection(void)
{   uint8_t buffer[3];
    // meletakkan ponter register pada ID register A
    i2c.Start_Wait(HMC5883L_ADDR | I2C_WRITE);
	i2c.Write(HMC5883L_RA_ID_A);
	i2c.Stop();
	// membaca data dari ID register A, B dan C
    i2c.Start_Wait(HMC5883L_ADDR | I2C_READ);
	buffer[0] = i2c.ReadAck();      // membaca isi ID register A --> isi seharusnya adalah karakter H
	buffer[1] = i2c.ReadAck();      // membaca isi ID register B --> isi seharusnya adalah karakter 4
	buffer[2] = i2c.ReadNak();      // membaca isi ID register C --> isi seharusnya adalah karakter 3
	i2c.Stop();
    if(buffer[0]=='H' && buffer[1]=='4' && buffer[2]=='3')  {   return 1;   }
    else                                                    {   return 0;   }
}

int8_t myCompass::Init()
{   if(TestConnection())
    {
        // write CONFIG_A register
        i2c.Start(HMC5883L_ADDR | I2C_WRITE);
        i2c.Write(HMC5883L_RA_CONFIG_A); //reg
        i2c.Write((HMC5883L_AVERAGING_8 << (HMC5883L_CRA_AVERAGE_BIT - HMC5883L_CRA_AVERAGE_LENGTH + 1)) |
        (HMC5883L_RATE_15     << (HMC5883L_CRA_RATE_BIT - HMC5883L_CRA_RATE_LENGTH + 1)) |
        (HMC5883L_BIAS_NORMAL << (HMC5883L_CRA_BIAS_BIT - HMC5883L_CRA_BIAS_LENGTH + 1)));
        i2c.Stop();
        // write CONFIG_B register
        SetGain(HMC5883L_GAIN_1090);
        // write MODE register
        SetMode(HMC5883L_MODE_SINGLE);
        return 1;
    }
    else
    {   return 0;
    }
}


// CONFIG_A register

/** Get number of samples averaged per measurement.
 * @return Current samples averaged per measurement (0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_AVERAGING_8
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
uint8_t myCompass::GetSampleAveraging() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_AVERAGE_BIT, HMC5883L_CRA_AVERAGE_LENGTH, &buffer);
    return buffer;
}
/** Set number of samples averaged per measurement.
 * @param averaging New samples averaged per measurement setting(0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
void myCompass::SetSampleAveraging(uint8_t averaging) {
    i2c.WriteBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_AVERAGE_BIT, HMC5883L_CRA_AVERAGE_LENGTH, averaging);
}
/** Get data output rate value.
 * The Table below shows all selectable output rates in continuous measurement
 * mode. All three channels shall be measured within a given output rate. Other
 * output rates with maximum rate of 160 Hz can be achieved by monitoring DRDY
 * interrupt pin in single measurement mode.
 *
 * Value | Typical Data Output Rate (Hz)
 * ------+------------------------------
 * 0     | 0.75
 * 1     | 1.5
 * 2     | 3
 * 3     | 7.5
 * 4     | 15 (Default)
 * 5     | 30
 * 6     | 75
 * 7     | Not used
 *
 * @return Current rate of data output to registers
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
uint8_t myCompass::GetDataRate() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT, HMC5883L_CRA_RATE_LENGTH, &buffer);
    return buffer;
}
/** Set data output rate value.
 * @param rate Rate of data output to registers
 * @see getDataRate()
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
void myCompass::SetDataRate(uint8_t rate) {
    i2c.WriteBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT, HMC5883L_CRA_RATE_LENGTH, rate);
}
/** Get measurement bias value.
 * @return Current bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
uint8_t myCompass::GetMeasurementBias() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_BIAS_BIT, HMC5883L_CRA_BIAS_LENGTH, &buffer);
    return buffer;
}
/** Set measurement bias value.
 * @param bias New bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
void myCompass::SetMeasurementBias(uint8_t bias) {
    i2c.WriteBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_BIAS_BIT, HMC5883L_CRA_BIAS_LENGTH, bias);
}

// CONFIG_B register

/** Get magnetic field gain value.
 * The table below shows nominal gain settings. Use the "Gain" column to convert
 * counts to Gauss. Choose a lower gain value (higher GN#) when total field
 * strength causes overflow in one of the data output registers (saturation).
 * The data output range for all settings is 0xF800-0x07FF (-2048 - 2047).
 *
 * Value | Field Range | Gain (LSB/Gauss)
 * ------+-------------+-----------------
 * 0     | +/- 0.88 Ga | 1370
 * 1     | +/- 1.3 Ga  | 1090 (Default)
 * 2     | +/- 1.9 Ga  | 820
 * 3     | +/- 2.5 Ga  | 660
 * 4     | +/- 4.0 Ga  | 440
 * 5     | +/- 4.7 Ga  | 390
 * 6     | +/- 5.6 Ga  | 330
 * 7     | +/- 8.1 Ga  | 230
 *
 * @return Current magnetic field gain value
 * @see HMC5883L_GAIN_1090
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
uint8_t myCompass::GetGain() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_B, HMC5883L_CRB_GAIN_BIT, HMC5883L_CRB_GAIN_LENGTH, &buffer);
    return buffer;
}
/** Set magnetic field gain value.
 * @param gain New magnetic field gain value
 * @see getGain()
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
void myCompass::SetGain(uint8_t gain) {
    // use this method to guarantee that bits 4-0 are set to zero, which is a
    // requirement specified in the datasheet; it's actually more efficient than
    // using the I2Cdev.writeBits method
    i2c.WriteByte(HMC5883L_ADDR, HMC5883L_RA_CONFIG_B, gain << (HMC5883L_CRB_GAIN_BIT - HMC5883L_CRB_GAIN_LENGTH + 1));
}

// MODE register

/** Get measurement mode.
 * In continuous-measurement mode, the device continuously performs measurements
 * and places the result in the data register. RDY goes high when new data is
 * placed in all three registers. After a power-on or a write to the mode or
 * configuration register, the first measurement set is available from all three
 * data output registers after a period of 2/fDO and subsequent measurements are
 * available at a frequency of fDO, where fDO is the frequency of data output.
 *
 * When single-measurement mode (default) is selected, device performs a single
 * measurement, sets RDY high and returned to idle mode. Mode register returns
 * to idle mode bit values. The measurement remains in the data output register
 * and RDY remains high until the data output register is read or another
 * measurement is performed.
 *
 * @return Current measurement mode
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
uint8_t myCompass::GetMode() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_MODE, HMC5883L_MODEREG_BIT, HMC5883L_MODEREG_LENGTH, &buffer);
    return buffer;
}
/** Set measurement mode.
 * @param newMode New measurement mode
 * @see getMode()
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
void myCompass::SetMode(uint8_t newMode) {
    // use this method to guarantee that bits 7-2 are set to zero, which is a
    // requirement specified in the datasheet; it's actually more efficient than
    // using the I2Cdev.writeBits method
    i2c.WriteByte(HMC5883L_ADDR, HMC5883L_RA_MODE, newMode << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    mode = newMode; // track to tell if we have to clear bit 7 after a read
}

// DATA* registers

/** Get 3-axis heading measurements.
 * In the event the ADC reading overflows or underflows for the given channel,
 * or if there is a math overflow during the bias measurement, this data
 * register will contain the value -4096. This register value will clear when
 * after the next valid measurement is made. Note that this method automatically
 * clears the appropriate bit in the MODE register if Single mode is active.
 * @param x 16-bit signed integer container for X-axis heading
 * @param y 16-bit signed integer container for Y-axis heading
 * @param z 16-bit signed integer container for Z-axis heading
 * @see HMC5883L_RA_DATAX_H
 */

  void myCompass::StartBacaRegister(uint8_t Register)
    {   i2c.Start(HMC5883L_ADDR | I2C_WRITE);
        i2c.Write(Register);
        _delay_us(5);
            //read data
        i2c.Start(HMC5883L_ADDR | I2C_READ);
    }

void myCompass::GetHeading(int16_t *x, int16_t *y, int16_t *z) {
    Data16Bit   Comp;
    StartBacaRegister(HMC5883L_RA_DATAX_H);
    Comp.d8[1] = i2c.ReadAck();  // index pembacaan data dibalik (MPU6050 = big endian, AVR = little endian)
    Comp.d8[0] = i2c.ReadAck();
    *x = Comp.d16s;
    Comp.d8[1] = i2c.ReadAck();
    Comp.d8[0] = i2c.ReadAck();
    *z = Comp.d16s;
    Comp.d8[1] = i2c.ReadAck();
    Comp.d8[0] = i2c.ReadNak();
    *y = Comp.d16s;
    i2c.Stop();

    /*uint8_t buffer[2];      // index pembacaan data dibalik (MPU6050 = big endian, AVR = little endian)
    StartBacaRegister(HMC5883L_RA_DATAX_H);
    buffer[1] = i2c.ReadAck();  // index pembacaan data dibalik (MPU6050 = big endian, AVR = little endian)
    buffer[0] = i2c.ReadAck();
    *x = *((int16_t *) &buffer[0]);
    buffer[1] = i2c.ReadAck();
    buffer[0] = i2c.ReadAck();
    *z = *((int16_t *) &buffer[0]);
    buffer[1] = i2c.ReadAck();
    buffer[0] = i2c.ReadNak();
    *y = *((int16_t *) &buffer[0]);
    i2c.Stop();*/
}
/** Get X-axis heading measurement.
 * @return 16-bit signed integer with X-axis heading
 * @see HMC5883L_RA_DATAX_H
 */
int16_t myCompass::GetHeadingX() {
    Data16Bit   Comp;
    StartBacaRegister(HMC5883L_RA_DATAX_H);
    Comp.d8[1] = i2c.ReadAck();  // index pembacaan data dibalik (MPU6050 = big endian, AVR = little endian)
    Comp.d8[0] = i2c.ReadNak();
    i2c.Stop();
    if (mode == HMC5883L_MODE_SINGLE)
        i2c.WriteByte(HMC5883L_ADDR, HMC5883L_RA_MODE, HMC5883L_MODE_SINGLE << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    return Comp.d16s;

    /*uint8_t buffer[2];
    StartBacaRegister(HMC5883L_RA_DATAX_H);
    buffer[1] = i2c.ReadAck();  // index pembacaan data dibalik (MPU6050 = big endian, AVR = little endian)
    buffer[0] = i2c.ReadNak();
    i2c.Stop();
    if (mode == HMC5883L_MODE_SINGLE)
        i2c.WriteByte(HMC5883L_ADDR, HMC5883L_RA_MODE, HMC5883L_MODE_SINGLE << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    return *((int16_t *) &buffer[0]);*/
}
/** Get Y-axis heading measurement.
 * @return 16-bit signed integer with Y-axis heading
 * @see HMC5883L_RA_DATAY_H
 */
int16_t myCompass::GetHeadingY() {
    Data16Bit   Comp;
    StartBacaRegister(HMC5883L_RA_DATAY_H);
    Comp.d8[1] = i2c.ReadAck();  // index pembacaan data dibalik (MPU6050 = big endian, AVR = little endian)
    Comp.d8[0] = i2c.ReadNak();
    i2c.Stop();
    if (mode == HMC5883L_MODE_SINGLE)
        i2c.WriteByte(HMC5883L_ADDR, HMC5883L_RA_MODE, HMC5883L_MODE_SINGLE << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    return Comp.d16s;
    /*uint8_t buffer[2];
    StartBacaRegister(HMC5883L_RA_DATAY_H);
    buffer[1] = i2c.ReadAck();  // index pembacaan data dibalik (MPU6050 = big endian, AVR = little endian)
    buffer[0] = i2c.ReadNak();
    i2c.Stop();
    if (mode == HMC5883L_MODE_SINGLE)
        i2c.WriteByte(HMC5883L_ADDR, HMC5883L_RA_MODE, HMC5883L_MODE_SINGLE << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    return *((int16_t *) &buffer[0]);*/
}
/** Get Z-axis heading measurement.
 * @return 16-bit signed integer with Z-axis heading
 * @see HMC5883L_RA_DATAZ_H
 */
int16_t myCompass::GetHeadingZ() {
    Data16Bit   Comp;
    StartBacaRegister(HMC5883L_RA_DATAZ_H);
    Comp.d8[1] = i2c.ReadAck();  // index pembacaan data dibalik (MPU6050 = big endian, AVR = little endian)
    Comp.d8[0] = i2c.ReadNak();
    i2c.Stop();
    if (mode == HMC5883L_MODE_SINGLE)
        i2c.WriteByte(HMC5883L_ADDR, HMC5883L_RA_MODE, HMC5883L_MODE_SINGLE << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    return Comp.d16s;
    /*uint8_t buffer[2];
    StartBacaRegister(HMC5883L_RA_DATAZ_H);
    buffer[1] = i2c.ReadAck();  // index pembacaan data dibalik (MPU6050 = big endian, AVR = little endian)
    buffer[0] = i2c.ReadNak();
    i2c.Stop();
    if (mode == HMC5883L_MODE_SINGLE)
        i2c.WriteByte(HMC5883L_ADDR, HMC5883L_RA_MODE, HMC5883L_MODE_SINGLE << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    return *((int16_t *) &buffer[0]);*/
}
// STATUS register

/** Get data output register lock status.
 * This bit is set when this some but not all for of the six data output
 * registers have been read. When this bit is set, the six data output registers
 * are locked and any new data will not be placed in these register until one of
 * three conditions are met: one, all six bytes have been read or the mode
 * changed, two, the mode is changed, or three, the measurement configuration is
 * changed.
 * @return Data output register lock status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_LOCK_BIT
 */
bool myCompass::GetLockStatus() {
    uint8_t buffer;
    i2c.ReadBit(HMC5883L_ADDR, HMC5883L_RA_STATUS, HMC5883L_STATUS_LOCK_BIT, &buffer);
    return buffer;
}
/** Get data ready status.
 * This bit is set when data is written to all six data registers, and cleared
 * when the device initiates a write to the data output registers and after one
 * or more of the data output registers are written to. When RDY bit is clear it
 * shall remain cleared for 250 us. DRDY pin can be used as an alternative to
 * the status register for monitoring the device for measurement data.
 * @return Data ready status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_READY_BIT
 */
bool myCompass::GetReadyStatus() {
    uint8_t buffer;
    i2c.ReadBit(HMC5883L_ADDR, HMC5883L_RA_STATUS, HMC5883L_STATUS_READY_BIT, &buffer);
    return buffer;
}

// ID_* registers

/** Get identification byte A
 * @return ID_A byte (should be 01001000, ASCII value 'H')
 */
uint8_t myCompass::GetIDA() {
    uint8_t buffer;
    i2c.ReadByte(HMC5883L_ADDR, HMC5883L_RA_ID_A, &buffer);
    return buffer;
}
/** Get identification byte B
 * @return ID_A byte (should be 00110100, ASCII value '4')
 */
uint8_t myCompass::GetIDB() {
    uint8_t buffer;
    i2c.ReadByte(HMC5883L_ADDR, HMC5883L_RA_ID_B, &buffer);
    return buffer;
}
/** Get identification byte C
 * @return ID_A byte (should be 00110011, ASCII value '3')
 */
uint8_t myCompass::GetIDC() {
    uint8_t buffer;
    i2c.ReadByte(HMC5883L_ADDR, HMC5883L_RA_ID_C, &buffer);
    return buffer;
}


// Update rate
// Value | Typical Data Output Rate (Hz)
// ------+------------------------------
// 0     | 0.75
// 1     | 1.5
// 2     | 3
// 3     | 7.5
// 4     | 15 (Default)
// 5     | 30
// 6     | 75
// 7     | Not used

#endif // UseCompass

#endif // MYCOMPASS_H_INCLUDED
