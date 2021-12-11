/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 5/29/2015
Author  : Eko Henfri Binugroho
          modified from I2Cdev library collection - HMC5883L I2C device class
          Based on Honeywell HMC5883L datasheet, 10/2010 (Form #900405 Rev B)
          6/12/2012 by Jeff Rowberg <jeff@rowberg.net>
Company : ER2C
Code    : Heading Calculation using Honeywell HMC5883L 3 axis magneto sensor

This program is free software: you can redistribute it and/or modify it under
the terms of the version 3 GNU General Public License as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*********************************************************/

#ifndef myHMC5883L_H_INCLUDED
#define myHMC5883L_H_INCLUDED

#if UseCompass == 1

//definitions
#define HMC5883L_ADDR (0x1E<<1) //device address
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

#define HMC5883L_AVERAGING_1        0x00    // 1 data per reading (default-> no averaging)
#define HMC5883L_AVERAGING_2        0x01    // average from 2 data per reading
#define HMC5883L_AVERAGING_4        0x02    // average from 4 data per reading
#define HMC5883L_AVERAGING_8        0x03    // average from 8 data per reading

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

enum    Axis{xAxis=0, yAxis, zAxis };

class myHMC5883L {
    public:
        int8_t  Init();
        void    Start(void);
        void    Stop(void);
        // DATA* registers
        void    ReadMagnetoRaw(void);
        float   FlatHeading(void);
        float   TiltedHeading(void);
        void    HardIronCallibration(void);
        //void    SoftIronCallibration(void);     // belum diimplementasikan pada versi ini

        int16_t Mx, My, Mz;
        //float magScale[3];
        int16_t magOffset[3];
        float   Scale[3];
    private:
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
        // STATUS register
        bool    GetLockStatus();
        bool    GetReadyStatus();
        // MODE register
        uint8_t GetMode();
        void    SetMode(uint8_t mode);

        int8_t  TestConnection(void);
        uint8_t mode;
        //float hdgY;
}kompas;


// HardIronCallibration adalah proses untuk mencari nilai Offset dan nilai Scale dari masing-masing sumbu
// sensor magnetometer x, y dan z.

// Adapun langkah  yang dilakukan pada saat melakukan proses kalibrasi adalah:
// 1. posisikan board dalam kondisi TIDUR (mendatar), lalu putar perlahan secara mendatar sampai minimal
//    satu putaran penuh (minimal 360 derajat)
// 2. selanjutnya posisikan board dalam kondisi tegak, lalu putar kembali mendatar secara perkahan sampai
//    minimal satu putaran penuh (minimal 360 derajat)
// 3. setalah langkah 1 dan 2 dilakukan, tekan tombol 1, maka data kalibrasi akan disimpan pada EEPROM
// 4. proses kaibrasi selesai.

// Karena data kalibrasi kompas disimpan ke EEPROM, maka fuse bit EESAVE harus diaktifkan (nilai=0)
// Secara default bit EESAVE tidak aktif, sehingga data EEPROM akan terhapus saat AVR di ERASE, sesaat
// sebelum dilakukan proses flash data ke AVR (saat penulisan program dari PC ke AVR).
// Pengaturan bit EESAVE dapat dilakukan dengan pengaturan fuse bit, misal dengan menggunakan
// tool Khazama AVR programmer (http://khazama.com/project/programmer/)

void    myHMC5883L::HardIronCallibration(void)
{   // kalibrasi Hard Iron Distortion
    int16_t magMax[3], magMin[3];
    float magTotal;
    uint8_t i=0;
    Stop();
    lcd.Hapus();
    lcd.TulisTengah (0, "Kalibrasi Kompas");
    lcd.TulisTengah (1, "Putar 360Derajat");
    ReadMagnetoRaw();
    magMax[xAxis] = Mx;     magMin[xAxis] = Mx;
    magMax[yAxis] = My;     magMin[yAxis] = My;
    magMax[zAxis] = Mz;     magMin[zAxis] = Mz;
    _delay_ms(14);      // rate 75Hz
    while (!Tombol1)
    {   ReadMagnetoRaw();
        if(Mx < magMin[xAxis])      magMin[xAxis] = (Mx + magMin[xAxis]*2)/3;
        else if(Mx > magMax[xAxis]) magMax[xAxis] = (Mx + magMax[xAxis]*2)/3;
        if(My < magMin[yAxis])      magMin[yAxis] = (My + magMin[yAxis]*2)/3;
        else if(My > magMax[yAxis]) magMax[yAxis] = (My + magMax[yAxis]*2)/3;
        if(Mz < magMin[zAxis])      magMin[zAxis] = (Mz + magMin[zAxis]*2)/3;
        else if(Mz > magMax[zAxis]) magMax[zAxis] = (Mz + magMax[zAxis]*2)/3;
        _delay_ms(13);  // rate 75Hz
        LED1(++i%2);
    }
    magTotal = (float)((magMax[0]-magMin[0])+(magMax[1]-magMin[1])+(magMax[2]-magMin[2]));
    for(i=0;i<3;i++)
    {   Scale[i] = magTotal/(float)((magMax[i]-magMin[i])*3);
        eeprom_update_float(&magScale[i], Scale[i]);
        magOffset[i] = (magMax[i]+magMin[i])/2;
        eeprom_update_word(&magOffSetRom[i], magOffset[i]);
    }
    lcd.HapusPinggir(1);
    lcd.TulisTengah (1, "<<< Selesai. >>>");
    buzzer.Nada2();
    lcd.Hapus();
}

/*
void    myHMC5883L::SoftIronCallibration(void)
{   // kalibrasi Soft Iron Distortion
    uint8_t i=0;
    float Theta, InvR, LastInvR=1;
    Stop();
    while (!Tombol2)
    {   ReadMagnetoRaw();
        Mx-=magOffset[xAxis];
        My-=magOffset[yAxis];
        InvR = invSqrt(Mx*Mx + My*My);
        if(InvR<LastInvR)     // apabila radius sekarang lebih besar (nilai inverse radius lebih kecil)
        {   LastInvR = InvR;
            Theta = asin(My*InvR);
        }
        LED1(++i%2);
    }


    //===============================================
          // Apply Soft iron compensation
          // This removes the distortion caused by ferrous
          // material that is on the sensor platform
          // (turn the elipse into a circle)

          //===============================================
          //Apply correction to the X axis

          //rotate the XY vector
          magRot_x = sen_data->magnetom_x * cos(sen_offset->magnetom_XY_Theta) + sen_data->magnetom_y *sin(sen_offset->magnetom_XY_Theta);
          magRot_y = sen_data->magnetom_y * cos(sen_offset->magnetom_XY_Theta) - sen_data->magnetom_x *sin(sen_offset->magnetom_XY_Theta);

          //Scale the X
          sen_data->magnetom_x = magRot_x/sen_offset->magnetom_XY_Scale;

          //rotate the XY vector back
          sen_data->magnetom_x = sen_data->magnetom_x * cos(-sen_offset->magnetom_XY_Theta) + magRot_y * sin(-sen_offset->magnetom_XY_Theta);
          sen_data->magnetom_y = magRot_y * cos(-sen_offset->magnetom_XY_Theta) + sen_data->magnetom_x * sin(-sen_offset->magnetom_XY_Theta);

          //===============================================
          //Apply correction to the Z axis

          //rotate the YZ vector
          magRot_z = sen_data->magnetom_z * cos(sen_offset->magnetom_YZ_Theta) + sen_data->magnetom_y *sin(sen_offset->magnetom_YZ_Theta);
          magRot_y = sen_data->magnetom_y * cos(sen_offset->magnetom_YZ_Theta) - sen_data->magnetom_z *sin(sen_offset->magnetom_YZ_Theta);

          //Scale the Z
          sen_data->magnetom_z = magRot_z/sen_offset->magnetom_YZ_Scale;

          //rotate the YZ vector back
          sen_data->magnetom_z = sen_data->magnetom_z * cos(-sen_offset->magnetom_YZ_Theta) + magRot_y * sin(-sen_offset->magnetom_YZ_Theta);
          sen_data->magnetom_y = magRot_y * cos(-sen_offset->magnetom_YZ_Theta) + sen_data->magnetom_z * sin(-sen_offset->magnetom_YZ_Theta);

}

*/

// Start proses sampling pembacaan data magneto meter (melalui interupsi timer 1)
// lihat file "mySystem.h"
void    myHMC5883L::Start(void)
{   myFlag.CompassStart = 1;
}

// Stop proses sampling pembacaan data magneto meter (melalui interupsi timer 1)
// lihat file "mySystem.h"
void    myHMC5883L::Stop(void)
{   myFlag.CompassStart = 0;
}

// SimpleHeading melakukan kalhulasi arah dengan mengandalkan pengukuran magnetisasi dari 2 arah
// saja. Data arah valid hanya ketika megneto meter berada dalam kondisi datar (roll=0, pitch=0)
float myHMC5883L::FlatHeading(void) {
    #if PosisiBoard == TIDUR
    float  Heading = atan2 ((My-magOffset[yAxis]) * Scale[yAxis] , (Mx-magOffset[xAxis])* Scale[xAxis]) * RAD2DEG;
    #else
    float  Heading = atan2 ((My-magOffset[yAxis]) * Scale[yAxis] , -(Mz-magOffset[zAxis])* Scale[zAxis]) * RAD2DEG;
    #endif // PosisiBoard
    if (Heading<0) Heading+=360;
    return Heading;
}

// Tilt compensated Heading hanya valid ketika IMU aktif (Kalkulasi Roll dan Pitch Aktif)
// Perhitungan IMU aktif saat nilai myFlag.ImuStart = 1
// sudut yang bisa dikompensasi terbatas +-45 derajat
float myHMC5883L::TiltedHeading(void) {
    float   Heading;
    float   cosRoll =  cos((imu.Roll)*DEG2RAD);
    float   sinRoll =  sin((imu.Roll)*DEG2RAD);
    float   cosPitch = cos(imu.Pitch*DEG2RAD);
    float   sinPitch = sin(imu.Pitch*DEG2RAD);
    #if PosisiBoard == TIDUR
    float   magX =  (float)(Mx-magOffset[xAxis])*Scale[xAxis] * cosPitch + \
                    (float)(My-magOffset[yAxis])*Scale[yAxis] * sinRoll * sinPitch - \
                    (float)(Mz-magOffset[zAxis])*Scale[zAxis] * cosRoll * sinPitch;

    float   magY =  (float)(My-magOffset[yAxis])*Scale[yAxis] * cosRoll + \
                    (float)(Mz-magOffset[zAxis])*Scale[zAxis] * sinRoll;
    #else
    float   magX =  (float)-(Mz-magOffset[zAxis])*Scale[zAxis] * cosPitch + \
                    (float)(My-magOffset[yAxis])*Scale[yAxis] * sinRoll * sinPitch - \
                    (float)(Mx-magOffset[xAxis])*Scale[xAxis] * cosRoll * sinPitch;

    float   magY =  (float)(My-magOffset[yAxis])*Scale[yAxis] * cosRoll + \
                    (float)(Mx-magOffset[xAxis])*Scale[xAxis] * sinRoll;
    #endif // PosisiBoard

    Heading = atan2 (magY , magX) * RAD2DEG;
    if (Heading<0) Heading+=360;
    return Heading;
};

// memeriksa apakah koneksi MCU dengan HMC5883L berhasil
// return 1 = koneksi berhasil
// return 0 = koneksi gagal
int8_t  myHMC5883L::TestConnection(void)
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

// Inisialisasi HMC5883L (Averaging, Rate, Bias, Gain, Mode)
// return 1 = inisialisasi berhasil
// return 0 = inisialisasi gagal
int8_t  myHMC5883L::Init()
{   if(TestConnection())
    {   // menuliskan data pada register CONFIG A
        i2c.Start(HMC5883L_ADDR | I2C_WRITE);
        i2c.Write(HMC5883L_RA_CONFIG_A);
        // averaging 8 data, rate 30 Hz, bias normal
        i2c.Write((HMC5883L_AVERAGING_8<<5)|(HMC5883L_RATE_30<<2)|HMC5883L_BIAS_NORMAL);
        i2c.Stop();
        // memlilih MODE
        SetMode(HMC5883L_MODE_CONTINUOUS);
        // memilih gain dari magneto meter
        SetGain(HMC5883L_GAIN_1090);
        return 1;
    }
    else
    {   return 0;
    }
}
// CONFIG_A register

/** Get number of samples averaged per measurement.
 * @return Current samples averaged per measurement (0-3 for 1/2/4/8 respectively)
 * @lihat deklarasi HMC5883L_AVERAGING_8
 * @lihat deklarasi HMC5883L_RA_CONFIG_A
 * @lihat deklarasi HMC5883L_CRA_AVERAGE_BIT
 * @lihat deklarasi HMC5883L_CRA_AVERAGE_LENGTH
 */
uint8_t myHMC5883L::GetSampleAveraging() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_AVERAGE_BIT, HMC5883L_CRA_AVERAGE_LENGTH, &buffer);
    return buffer;
}
/** Set number of samples averaged per measurement.
 * @param averaging New samples averaged per measurement setting(0-3 for 1/2/4/8 respectively)
 * @lihat deklarasi HMC5883L_RA_CONFIG_A
 * @lihat deklarasi HMC5883L_CRA_AVERAGE_BIT
 * @lihat deklarasi HMC5883L_CRA_AVERAGE_LENGTH
 */
void    myHMC5883L::SetSampleAveraging(uint8_t averaging) {
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
 * @lihat deklarasi HMC5883L_RATE_15
 * @lihat deklarasi HMC5883L_RA_CONFIG_A
 * @lihat deklarasi HMC5883L_CRA_RATE_BIT
 * @lihat deklarasi HMC5883L_CRA_RATE_LENGTH
 */
uint8_t myHMC5883L::GetDataRate() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT, HMC5883L_CRA_RATE_LENGTH, &buffer);
    return buffer;
}
/** Set data output rate value.
 * @param rate Rate of data output to registers
 * @lihat deklarasi getDataRate()
 * @lihat deklarasi HMC5883L_RATE_15
 * @lihat deklarasi HMC5883L_RA_CONFIG_A
 * @lihat deklarasi HMC5883L_CRA_RATE_BIT
 * @lihat deklarasi HMC5883L_CRA_RATE_LENGTH
 */
void myHMC5883L::SetDataRate(uint8_t rate) {
    i2c.WriteBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT, HMC5883L_CRA_RATE_LENGTH, rate);
}

/** Get measurement bias value.
 * @return Current bias value (0-2 for normal/positive/negative respectively)
 * @lihat deklarasi HMC5883L_BIAS_NORMAL
 * @lihat deklarasi HMC5883L_RA_CONFIG_A
 * @lihat deklarasi HMC5883L_CRA_BIAS_BIT
 * @lihat deklarasi HMC5883L_CRA_BIAS_LENGTH
 */
uint8_t myHMC5883L::GetMeasurementBias() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_BIAS_BIT, HMC5883L_CRA_BIAS_LENGTH, &buffer);
    return buffer;
}

/** Set measurement bias value.
 * @param bias New bias value (0-2 for normal/positive/negative respectively)
 * @lihat deklarasi HMC5883L_BIAS_NORMAL
 * @lihat deklarasi HMC5883L_RA_CONFIG_A
 * @lihat deklarasi HMC5883L_CRA_BIAS_BIT
 * @lihat deklarasi HMC5883L_CRA_BIAS_LENGTH
 */
void myHMC5883L::SetMeasurementBias(uint8_t bias) {
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
 * @lihat deklarasi HMC5883L_GAIN_1090
 * @lihat deklarasi HMC5883L_RA_CONFIG_B
 * @lihat deklarasi HMC5883L_CRB_GAIN_BIT
 * @lihat deklarasi HMC5883L_CRB_GAIN_LENGTH
 */
uint8_t myHMC5883L::GetGain() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_CONFIG_B, HMC5883L_CRB_GAIN_BIT, HMC5883L_CRB_GAIN_LENGTH, &buffer);
    return buffer;
}

/** Set magnetic field gain value.
 * @param gain New magnetic field gain value
 * @lihat deklarasi getGain()
 * @lihat deklarasi HMC5883L_RA_CONFIG_B
 * @lihat deklarasi HMC5883L_CRB_GAIN_BIT
 * @lihat deklarasi HMC5883L_CRB_GAIN_LENGTH
 */
void myHMC5883L::SetGain(uint8_t gain) {
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
 * @lihat deklarasi HMC5883L_MODE_CONTINUOUS
 * @lihat deklarasi HMC5883L_MODE_SINGLE
 * @lihat deklarasi HMC5883L_MODE_IDLE
 * @lihat deklarasi HMC5883L_RA_MODE
 * @lihat deklarasi HMC5883L_MODEREG_BIT
 * @lihat deklarasi HMC5883L_MODEREG_LENGTH
 */
uint8_t myHMC5883L::GetMode() {
    uint8_t buffer;
    i2c.ReadBits(HMC5883L_ADDR, HMC5883L_RA_MODE, HMC5883L_MODEREG_BIT, HMC5883L_MODEREG_LENGTH, &buffer);
    return buffer;
}
/** Set measurement mode.
 * @param newMode New measurement mode
 * @lihat deklarasi getMode()
 * @lihat deklarasi HMC5883L_MODE_CONTINUOUS
 * @lihat deklarasi HMC5883L_MODE_SINGLE
 * @lihat deklarasi HMC5883L_MODE_IDLE
 * @lihat deklarasi HMC5883L_RA_MODE
 * @lihat deklarasi HMC5883L_MODEREG_BIT
 * @lihat deklarasi HMC5883L_MODEREG_LENGTH
 */
void myHMC5883L::SetMode(uint8_t newMode) {
    // use this method to guarantee that bits 7-2 are set to zero, which is a
    // requirement specified in the datasheet; it's actually more efficient than
    // using the I2Cdev.writeBits method
    i2c.WriteByte(HMC5883L_ADDR, HMC5883L_RA_MODE, (newMode & 3));
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
 * @lihat deklarasi HMC5883L_RA_DATAX_H
 */

void myHMC5883L::ReadMagnetoRaw(void) {
    Data16Bit   Comp;
    if(myHMC5883L::mode == HMC5883L_MODE_CONTINUOUS )
    {   i2c.Start(HMC5883L_ADDR | I2C_WRITE);
        i2c.Write(HMC5883L_RA_DATAX_H);
        i2c.Stop();
    }
    else if (myHMC5883L::mode == HMC5883L_MODE_SINGLE )
    {   SetMode(HMC5883L_MODE_SINGLE);
    }
    i2c.Start_Wait(HMC5883L_ADDR | I2C_READ);
    Comp.d8u[1] = i2c.ReadAck();    // AVR = little-endian, dan HMC5883L = big-endian
    Comp.d8u[0] = i2c.ReadAck();
    Mx = Comp.d16s;
    Comp.d8u[1] = i2c.ReadAck();
    Comp.d8u[0] = i2c.ReadAck();
    Mz = Comp.d16s;
    Comp.d8u[1] = i2c.ReadAck();
    Comp.d8u[0] = i2c.ReadNak();
    My = Comp.d16s;
    i2c.Stop();
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
 * @lihat deklarasi HMC5883L_RA_STATUS
 * @lihat deklarasi HMC5883L_STATUS_LOCK_BIT
 */
bool myHMC5883L::GetLockStatus() {
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
 * @lihat deklarasi HMC5883L_RA_STATUS
 * @lihat deklarasi HMC5883L_STATUS_READY_BIT
 */
bool myHMC5883L::GetReadyStatus() {
    uint8_t buffer;
    i2c.ReadBit(HMC5883L_ADDR, HMC5883L_RA_STATUS, HMC5883L_STATUS_READY_BIT, &buffer);
    return buffer;
}

#endif // UseCompass

#endif // myHMC5883L_H_INCLUDED
