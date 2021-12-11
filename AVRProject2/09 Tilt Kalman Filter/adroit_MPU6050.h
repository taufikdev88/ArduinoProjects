/*********************************************************
Project : ADROIT AVR Rev.3
Version : 1
Date    : 3/13/2014
Author  : Eko Henfri Binugroho  
Company : ER2C

Code    : - Routines to access MPU6050 (3axis Accelerometer + Gyroscope)   
          - Filter Routines to calculate IMU data
*********************************************************/

#ifndef _MPU6050_IMU_
#define _MPU6050_IMU_

    // Register dari MPU6050

    #define MPU6050_RA_XG_OFFS_TC       0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
    #define MPU6050_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
    #define MPU6050_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
    #define MPU6050_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
    #define MPU6050_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
    #define MPU6050_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
    #define MPU6050_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS
    #define MPU6050_RA_XA_OFFS_L_TC     0x07
    #define MPU6050_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS
    #define MPU6050_RA_YA_OFFS_L_TC     0x09
    #define MPU6050_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS
    #define MPU6050_RA_ZA_OFFS_L_TC     0x0B
    #define MPU6050_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR
    #define MPU6050_RA_XG_OFFS_USRL     0x14
    #define MPU6050_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR
    #define MPU6050_RA_YG_OFFS_USRL     0x16
    #define MPU6050_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR
    #define MPU6050_RA_ZG_OFFS_USRL     0x18
    #define MPU6050_RA_SMPLRT_DIV       0x19
    #define MPU6050_RA_CONFIG           0x1A
    #define MPU6050_RA_GYRO_CONFIG      0x1B
    #define MPU6050_RA_ACCEL_CONFIG     0x1C
    #define MPU6050_RA_FF_THR           0x1D
    #define MPU6050_RA_FF_DUR           0x1E
    #define MPU6050_RA_MOT_THR          0x1F
    #define MPU6050_RA_MOT_DUR          0x20
    #define MPU6050_RA_ZRMOT_THR        0x21
    #define MPU6050_RA_ZRMOT_DUR        0x22
    #define MPU6050_RA_FIFO_EN          0x23
    #define MPU6050_RA_I2C_MST_CTRL     0x24
    #define MPU6050_RA_I2C_SLV0_ADDR    0x25
    #define MPU6050_RA_I2C_SLV0_REG     0x26
    #define MPU6050_RA_I2C_SLV0_CTRL    0x27
    #define MPU6050_RA_I2C_SLV1_ADDR    0x28
    #define MPU6050_RA_I2C_SLV1_REG     0x29
    #define MPU6050_RA_I2C_SLV1_CTRL    0x2A
    #define MPU6050_RA_I2C_SLV2_ADDR    0x2B
    #define MPU6050_RA_I2C_SLV2_REG     0x2C
    #define MPU6050_RA_I2C_SLV2_CTRL    0x2D
    #define MPU6050_RA_I2C_SLV3_ADDR    0x2E
    #define MPU6050_RA_I2C_SLV3_REG     0x2F
    #define MPU6050_RA_I2C_SLV3_CTRL    0x30
    #define MPU6050_RA_I2C_SLV4_ADDR    0x31
    #define MPU6050_RA_I2C_SLV4_REG     0x32
    #define MPU6050_RA_I2C_SLV4_DO      0x33
    #define MPU6050_RA_I2C_SLV4_CTRL    0x34
    #define MPU6050_RA_I2C_SLV4_DI      0x35
    #define MPU6050_RA_I2C_MST_STATUS   0x36
    #define MPU6050_RA_INT_PIN_CFG      0x37
    #define MPU6050_RA_INT_ENABLE       0x38
    #define MPU6050_RA_DMP_INT_STATUS   0x39
    #define MPU6050_RA_INT_STATUS       0x3A
    #define MPU6050_RA_ACCEL_XOUT_H     0x3B
    #define MPU6050_RA_ACCEL_XOUT_L     0x3C
    #define MPU6050_RA_ACCEL_YOUT_H     0x3D
    #define MPU6050_RA_ACCEL_YOUT_L     0x3E
    #define MPU6050_RA_ACCEL_ZOUT_H     0x3F
    #define MPU6050_RA_ACCEL_ZOUT_L     0x40
    #define MPU6050_RA_TEMP_OUT_H       0x41
    #define MPU6050_RA_TEMP_OUT_L       0x42
    #define MPU6050_RA_GYRO_XOUT_H      0x43
    #define MPU6050_RA_GYRO_XOUT_L      0x44
    #define MPU6050_RA_GYRO_YOUT_H      0x45
    #define MPU6050_RA_GYRO_YOUT_L      0x46
    #define MPU6050_RA_GYRO_ZOUT_H      0x47
    #define MPU6050_RA_GYRO_ZOUT_L      0x48
    #define MPU6050_RA_EXT_SENS_DATA_00 0x49
    #define MPU6050_RA_EXT_SENS_DATA_01 0x4A
    #define MPU6050_RA_EXT_SENS_DATA_02 0x4B
    #define MPU6050_RA_EXT_SENS_DATA_03 0x4C
    #define MPU6050_RA_EXT_SENS_DATA_04 0x4D
    #define MPU6050_RA_EXT_SENS_DATA_05 0x4E
    #define MPU6050_RA_EXT_SENS_DATA_06 0x4F
    #define MPU6050_RA_EXT_SENS_DATA_07 0x50
    #define MPU6050_RA_EXT_SENS_DATA_08 0x51
    #define MPU6050_RA_EXT_SENS_DATA_09 0x52
    #define MPU6050_RA_EXT_SENS_DATA_10 0x53
    #define MPU6050_RA_EXT_SENS_DATA_11 0x54
    #define MPU6050_RA_EXT_SENS_DATA_12 0x55
    #define MPU6050_RA_EXT_SENS_DATA_13 0x56
    #define MPU6050_RA_EXT_SENS_DATA_14 0x57
    #define MPU6050_RA_EXT_SENS_DATA_15 0x58
    #define MPU6050_RA_EXT_SENS_DATA_16 0x59
    #define MPU6050_RA_EXT_SENS_DATA_17 0x5A
    #define MPU6050_RA_EXT_SENS_DATA_18 0x5B
    #define MPU6050_RA_EXT_SENS_DATA_19 0x5C
    #define MPU6050_RA_EXT_SENS_DATA_20 0x5D
    #define MPU6050_RA_EXT_SENS_DATA_21 0x5E
    #define MPU6050_RA_EXT_SENS_DATA_22 0x5F
    #define MPU6050_RA_EXT_SENS_DATA_23 0x60
    #define MPU6050_RA_MOT_DETECT_STATUS    0x61
    #define MPU6050_RA_I2C_SLV0_DO          0x63
    #define MPU6050_RA_I2C_SLV1_DO          0x64
    #define MPU6050_RA_I2C_SLV2_DO          0x65
    #define MPU6050_RA_I2C_SLV3_DO          0x66
    #define MPU6050_RA_I2C_MST_DELAY_CTRL   0x67
    #define MPU6050_RA_SIGNAL_PATH_RESET    0x68
    #define MPU6050_RA_MOT_DETECT_CTRL      0x69
    #define MPU6050_RA_USER_CTRL            0x6A
    #define MPU6050_RA_PWR_MGMT_1           0x6B
    #define MPU6050_RA_PWR_MGMT_2           0x6C
    #define MPU6050_RA_BANK_SEL             0x6D
    #define MPU6050_RA_MEM_START_ADDR       0x6E
    #define MPU6050_RA_MEM_R_W              0x6F
    #define MPU6050_RA_DMP_CFG_1            0x70
    #define MPU6050_RA_DMP_CFG_2            0x71
    #define MPU6050_RA_FIFO_COUNTH          0x72
    #define MPU6050_RA_FIFO_COUNTL          0x73
    #define MPU6050_RA_FIFO_R_W             0x74
    #define MPU6050_RA_WHO_AM_I             0x75

    #define MPU6050_TC_PWR_MODE_BIT         7
    #define MPU6050_TC_OFFSET_BIT           6
    #define MPU6050_TC_OFFSET_LENGTH        6
    #define MPU6050_TC_OTP_BNK_VLD_BIT      0

    #define MPU6050_VDDIO_LEVEL_VLOGIC      0
    #define MPU6050_VDDIO_LEVEL_VDD         1

    #define MPU6050_CFG_EXT_SYNC_SET_BIT    5
    #define MPU6050_CFG_EXT_SYNC_SET_LENGTH 3
    #define MPU6050_CFG_DLPF_CFG_BIT        2
    #define MPU6050_CFG_DLPF_CFG_LENGTH     3

    #define MPU6050_EXT_SYNC_DISABLED       0x0
    #define MPU6050_EXT_SYNC_TEMP_OUT_L     0x1
    #define MPU6050_EXT_SYNC_GYRO_XOUT_L    0x2
    #define MPU6050_EXT_SYNC_GYRO_YOUT_L    0x3
    #define MPU6050_EXT_SYNC_GYRO_ZOUT_L    0x4
    #define MPU6050_EXT_SYNC_ACCEL_XOUT_L   0x5
    #define MPU6050_EXT_SYNC_ACCEL_YOUT_L   0x6
    #define MPU6050_EXT_SYNC_ACCEL_ZOUT_L   0x7

    #define MPU6050_DLPF_BW_256             0x00
    #define MPU6050_DLPF_BW_188             0x01
    #define MPU6050_DLPF_BW_98              0x02
    #define MPU6050_DLPF_BW_42              0x03
    #define MPU6050_DLPF_BW_20              0x04
    #define MPU6050_DLPF_BW_10              0x05
    #define MPU6050_DLPF_BW_5               0x06

    #define MPU6050_GCONFIG_FS_SEL_BIT      4
    #define MPU6050_GCONFIG_FS_SEL_LENGTH   2

    #define MPU6050_GYRO_FS_250             0x00
    #define MPU6050_GYRO_FS_500             0x01
    #define MPU6050_GYRO_FS_1000            0x02
    #define MPU6050_GYRO_FS_2000            0x03

    #define MPU6050_ACONFIG_XA_ST_BIT           7
    #define MPU6050_ACONFIG_YA_ST_BIT           6
    #define MPU6050_ACONFIG_ZA_ST_BIT           5
    #define MPU6050_ACONFIG_AFS_SEL_BIT         4
    #define MPU6050_ACONFIG_AFS_SEL_LENGTH      2
    #define MPU6050_ACONFIG_ACCEL_HPF_BIT       2
    #define MPU6050_ACONFIG_ACCEL_HPF_LENGTH    3

    #define MPU6050_ACCEL_FS_2          0x00
    #define MPU6050_ACCEL_FS_4          0x01
    #define MPU6050_ACCEL_FS_8          0x02
    #define MPU6050_ACCEL_FS_16         0x03

    #define MPU6050_DHPF_RESET          0x00
    #define MPU6050_DHPF_5              0x01
    #define MPU6050_DHPF_2P5            0x02
    #define MPU6050_DHPF_1P25           0x03
    #define MPU6050_DHPF_0P63           0x04
    #define MPU6050_DHPF_HOLD           0x07

    #define MPU6050_TEMP_FIFO_EN_BIT    7
    #define MPU6050_XG_FIFO_EN_BIT      6
    #define MPU6050_YG_FIFO_EN_BIT      5
    #define MPU6050_ZG_FIFO_EN_BIT      4
    #define MPU6050_ACCEL_FIFO_EN_BIT   3
    #define MPU6050_SLV2_FIFO_EN_BIT    2
    #define MPU6050_SLV1_FIFO_EN_BIT    1
    #define MPU6050_SLV0_FIFO_EN_BIT    0

    #define MPU6050_MULT_MST_EN_BIT     7
    #define MPU6050_WAIT_FOR_ES_BIT     6
    #define MPU6050_SLV_3_FIFO_EN_BIT   5
    #define MPU6050_I2C_MST_P_NSR_BIT   4
    #define MPU6050_I2C_MST_CLK_BIT     3
    #define MPU6050_I2C_MST_CLK_LENGTH  4

    #define MPU6050_CLOCK_DIV_348       0x0
    #define MPU6050_CLOCK_DIV_333       0x1
    #define MPU6050_CLOCK_DIV_320       0x2
    #define MPU6050_CLOCK_DIV_308       0x3
    #define MPU6050_CLOCK_DIV_296       0x4
    #define MPU6050_CLOCK_DIV_286       0x5
    #define MPU6050_CLOCK_DIV_276       0x6
    #define MPU6050_CLOCK_DIV_267       0x7
    #define MPU6050_CLOCK_DIV_258       0x8
    #define MPU6050_CLOCK_DIV_500       0x9
    #define MPU6050_CLOCK_DIV_471       0xA
    #define MPU6050_CLOCK_DIV_444       0xB
    #define MPU6050_CLOCK_DIV_421       0xC
    #define MPU6050_CLOCK_DIV_400       0xD
    #define MPU6050_CLOCK_DIV_381       0xE
    #define MPU6050_CLOCK_DIV_364       0xF

    #define MPU6050_I2C_SLV_RW_BIT      7
    #define MPU6050_I2C_SLV_ADDR_BIT    6
    #define MPU6050_I2C_SLV_ADDR_LENGTH 7
    #define MPU6050_I2C_SLV_EN_BIT      7
    #define MPU6050_I2C_SLV_BYTE_SW_BIT 6
    #define MPU6050_I2C_SLV_REG_DIS_BIT 5
    #define MPU6050_I2C_SLV_GRP_BIT     4
    #define MPU6050_I2C_SLV_LEN_BIT     3
    #define MPU6050_I2C_SLV_LEN_LENGTH  4

    #define MPU6050_I2C_SLV4_RW_BIT         7
    #define MPU6050_I2C_SLV4_ADDR_BIT       6
    #define MPU6050_I2C_SLV4_ADDR_LENGTH    7
    #define MPU6050_I2C_SLV4_EN_BIT         7
    #define MPU6050_I2C_SLV4_INT_EN_BIT     6
    #define MPU6050_I2C_SLV4_REG_DIS_BIT    5
    #define MPU6050_I2C_SLV4_MST_DLY_BIT    4
    #define MPU6050_I2C_SLV4_MST_DLY_LENGTH 5

    #define MPU6050_MST_PASS_THROUGH_BIT    7
    #define MPU6050_MST_I2C_SLV4_DONE_BIT   6
    #define MPU6050_MST_I2C_LOST_ARB_BIT    5
    #define MPU6050_MST_I2C_SLV4_NACK_BIT   4
    #define MPU6050_MST_I2C_SLV3_NACK_BIT   3
    #define MPU6050_MST_I2C_SLV2_NACK_BIT   2
    #define MPU6050_MST_I2C_SLV1_NACK_BIT   1
    #define MPU6050_MST_I2C_SLV0_NACK_BIT   0

    #define MPU6050_INTCFG_INT_LEVEL_BIT        7
    #define MPU6050_INTCFG_INT_OPEN_BIT         6
    #define MPU6050_INTCFG_LATCH_INT_EN_BIT     5
    #define MPU6050_INTCFG_INT_RD_CLEAR_BIT     4
    #define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT  3
    #define MPU6050_INTCFG_FSYNC_INT_EN_BIT     2
    #define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1
    #define MPU6050_INTCFG_CLKOUT_EN_BIT        0

    #define MPU6050_INTMODE_ACTIVEHIGH  0x00
    #define MPU6050_INTMODE_ACTIVELOW   0x01

    #define MPU6050_INTDRV_PUSHPULL     0x00
    #define MPU6050_INTDRV_OPENDRAIN    0x01

    #define MPU6050_INTLATCH_50USPULSE  0x00
    #define MPU6050_INTLATCH_WAITCLEAR  0x01

    #define MPU6050_INTCLEAR_STATUSREAD 0x00
    #define MPU6050_INTCLEAR_ANYREAD    0x01

    #define MPU6050_INTERRUPT_FF_BIT            7
    #define MPU6050_INTERRUPT_MOT_BIT           6
    #define MPU6050_INTERRUPT_ZMOT_BIT          5
    #define MPU6050_INTERRUPT_FIFO_OFLOW_BIT    4
    #define MPU6050_INTERRUPT_I2C_MST_INT_BIT   3
    #define MPU6050_INTERRUPT_PLL_RDY_INT_BIT   2
    #define MPU6050_INTERRUPT_DMP_INT_BIT       1
    #define MPU6050_INTERRUPT_DATA_RDY_BIT      0

    #define MPU6050_DMPINT_5_BIT                    5
    #define MPU6050_DMPINT_4_BIT                    4
    #define MPU6050_DMPINT_3_BIT                    3
    #define MPU6050_DMPINT_2_BIT                    2
    #define MPU6050_DMPINT_1_BIT                    1
    #define MPU6050_DMPINT_0_BIT                    0

    #define MPU6050_MOTION_MOT_XNEG_BIT             7
    #define MPU6050_MOTION_MOT_XPOS_BIT             6
    #define MPU6050_MOTION_MOT_YNEG_BIT             5
    #define MPU6050_MOTION_MOT_YPOS_BIT             4
    #define MPU6050_MOTION_MOT_ZNEG_BIT             3
    #define MPU6050_MOTION_MOT_ZPOS_BIT             2
    #define MPU6050_MOTION_MOT_ZRMOT_BIT            0

    #define MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT   7
    #define MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4
    #define MPU6050_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3
    #define MPU6050_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2
    #define MPU6050_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1
    #define MPU6050_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0

    #define MPU6050_PATHRESET_GYRO_RESET_BIT        2
    #define MPU6050_PATHRESET_ACCEL_RESET_BIT       1
    #define MPU6050_PATHRESET_TEMP_RESET_BIT        0

    #define MPU6050_DETECT_ACCEL_ON_DELAY_BIT       5
    #define MPU6050_DETECT_ACCEL_ON_DELAY_LENGTH    2
    #define MPU6050_DETECT_FF_COUNT_BIT             3
    #define MPU6050_DETECT_FF_COUNT_LENGTH          2
    #define MPU6050_DETECT_MOT_COUNT_BIT            1
    #define MPU6050_DETECT_MOT_COUNT_LENGTH         2

    #define MPU6050_DETECT_DECREMENT_RESET          0x0
    #define MPU6050_DETECT_DECREMENT_1              0x1
    #define MPU6050_DETECT_DECREMENT_2              0x2
    #define MPU6050_DETECT_DECREMENT_4              0x3     

    #define MPU6050_USERCTRL_DMP_EN_BIT             7
    #define MPU6050_USERCTRL_FIFO_EN_BIT            6
    #define MPU6050_USERCTRL_I2C_MST_EN_BIT         5
    #define MPU6050_USERCTRL_I2C_IF_DIS_BIT         4
    #define MPU6050_USERCTRL_DMP_RESET_BIT          3
    #define MPU6050_USERCTRL_FIFO_RESET_BIT         2
    #define MPU6050_USERCTRL_I2C_MST_RESET_BIT      1
    #define MPU6050_USERCTRL_SIG_COND_RESET_BIT     0

    #define MPU6050_PWR1_DEVICE_RESET_BIT           7
    #define MPU6050_PWR1_SLEEP_BIT                  6
    #define MPU6050_PWR1_CYCLE_BIT                  5
    #define MPU6050_PWR1_TEMP_DIS_BIT               3
    #define MPU6050_PWR1_CLKSEL_BIT                 2
    #define MPU6050_PWR1_CLKSEL_LENGTH              3

    #define MPU6050_CLOCK_INTERNAL              0x00
    #define MPU6050_CLOCK_PLL_XGYRO             0x01
    #define MPU6050_CLOCK_PLL_YGYRO             0x02
    #define MPU6050_CLOCK_PLL_ZGYRO             0x03
    #define MPU6050_CLOCK_PLL_EXT32K            0x04
    #define MPU6050_CLOCK_PLL_EXT19M            0x05
    #define MPU6050_CLOCK_KEEP_RESET            0x07

    #define MPU6050_PWR2_LP_WAKE_CTRL_BIT       7
    #define MPU6050_PWR2_LP_WAKE_CTRL_LENGTH    2
    #define MPU6050_PWR2_STBY_XA_BIT            5
    #define MPU6050_PWR2_STBY_YA_BIT            4
    #define MPU6050_PWR2_STBY_ZA_BIT            3
    #define MPU6050_PWR2_STBY_XG_BIT            2
    #define MPU6050_PWR2_STBY_YG_BIT            1
    #define MPU6050_PWR2_STBY_ZG_BIT            0

    #define MPU6050_WAKE_FREQ_1P25              0x0
    #define MPU6050_WAKE_FREQ_2P5               0x1
    #define MPU6050_WAKE_FREQ_5                 0x2
    #define MPU6050_WAKE_FREQ_10                0x3

    #define MPU6050_BANKSEL_PRFTCH_EN_BIT       6
    #define MPU6050_BANKSEL_CFG_USER_BANK_BIT   5
    #define MPU6050_BANKSEL_MEM_SEL_BIT         4
    #define MPU6050_BANKSEL_MEM_SEL_LENGTH      5

    #define MPU6050_WHO_AM_I_BIT                6
    #define MPU6050_WHO_AM_I_LENGTH             6

    #define MPU6050_DMP_MEMORY_BANKS            8
    #define MPU6050_DMP_MEMORY_BANK_SIZE        256
    #define MPU6050_DMP_MEMORY_CHUNK_SIZE       16

    //#include <math.h>
    #include <stdlib.h>
    #include "adroit_global.h"
    #include "adroit_i2c.h"

    //------------------------------- Fungsi dari IMU -----------------------------------------------------------------------

    //definitions
    #define MPU6050_ADDR (0x68 <<1)


    //Skala pilihan dari gyro and acc yang digunakan (silakan mofifikasi isinya sesuai dengan kebutuhan)
    #define MPU6050_GYRO_FS         MPU6050_GYRO_FS_1000    // seting skala 2000 derajat/detik 
    #define MPU6050_ACCEL_FS        MPU6050_ACCEL_FS_2      // seting skala percepatan 2 G

    // pilihan skala Gyroscope
    #define MPU6050_GYRO_LSB_250    131.0                   // 250 derajat/detik
    #define MPU6050_GYRO_LSB_500    65.5                    // 500 derajat/detik
    #define MPU6050_GYRO_LSB_1000   32.8                    // 1000 derajat/detik
    #define MPU6050_GYRO_LSB_2000   16.4                    // 2000 derajat/detik

    #if MPU6050_GYRO_FS == MPU6050_GYRO_FS_250
      #define MPU6050_GGAIN MPU6050_GYRO_LSB_250
    #elif MPU6050_GYRO_FS == MPU6050_GYRO_FS_500
      #define MPU6050_GGAIN MPU6050_GYRO_LSB_500
    #elif MPU6050_GYRO_FS == MPU6050_GYRO_FS_1000
      #define MPU6050_GGAIN MPU6050_GYRO_LSB_1000
    #elif MPU6050_GYRO_FS == MPU6050_GYRO_FS_2000
      #define MPU6050_GGAIN MPU6050_GYRO_LSB_2000
    #endif

    // Pilihan Skala dari Accelerometer
    #define MPU6050_ACCEL_LSB_2     16384.0                 // maksimal 2 g (1 g = 9.8 m/detik^2)
    #define MPU6050_ACCEL_LSB_4     8192.0                  // maksimal 4 g
    #define MPU6050_ACCEL_LSB_8     4096.0                  // maksimal 8 g
    #define MPU6050_ACCEL_LSB_16    2048.0                  // maksimal 16 g

    #if MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_2
    #define MPU6050_AGAIN MPU6050_ACCEL_LSB_2
    #elif MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_4
    #define MPU6050_AGAIN MPU6050_ACCEL_LSB_4
    #elif MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_8
    #define MPU6050_AGAIN MPU6050_ACCEL_LSB_8
    #elif MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_16
    #define MPU6050_AGAIN MPU6050_ACCEL_LSB_16
    #endif

    
//fungsi
#pragma used+
void MPU6050_Init();
uint8_t MPU6050_TestConnection();
void MPU6050_SleepDisabled();
void MPU6050_SleepEnabled();
void MPU6050ReadBytes(uint8_t regAddr, uint8_t length, uint8_t *data);
int8_t MPU6050ReadByte(uint8_t regAddr, uint8_t *data);
void MPU6050WriteByte(uint8_t regAddr, uint8_t data);
int8_t mpu6050_readBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
int8_t mpu6050_readBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data);
void mpu6050_writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
void mpu6050_writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data);
void KalibrasiGyroScope(void);
void BacaAccelerometer(void);
void BacaGyroScope(void);
#if ImuFilter == KalmanFilter
    void HitungKalmanRoll(void);
    void HitungKalmanPitch(void);
    void HitungYaw();
#elif ImuFilter == ComplementaryFilter
    void HitungComplementaryRoll(void);
    void HitungComplementaryPitch(void);
    void HitungYaw();
#elif ImuFilter == MahonyFilter
    void HitungMahonyFilter(void);
#endif
float invSqrt(float x);

// Membaca beberapa bytes data dari MPU6050 secara berurutan
void MPU6050ReadBytes(uint8_t regAddr, uint8_t length, uint8_t *data) 
{   int8_t i;
    if(length > 0) 
    {   //request register
        I2C_Start(MPU6050_ADDR | I2C_WRITE);
        I2C_Write(regAddr);
        delay_us(5);
        //read data
        I2C_Start(MPU6050_ADDR | I2C_READ);
        length--;
        for(i=length; i>=0; i--) 
        {   if(i==0)    data[i] = I2C_ReadNak();
            else        data[i] = I2C_ReadAck();
        }
        I2C_Stop();
    }
}

// membaca 1 byte dari MPU6050
int8_t MPU6050ReadByte(uint8_t regAddr, uint8_t * data) 
{   //request register
    I2C_Start(MPU6050_ADDR | I2C_WRITE);
    I2C_Write(regAddr);
    delay_us(5);
    //read data
    I2C_Start(MPU6050_ADDR | I2C_READ);
    data[0] = I2C_ReadNak();
    I2C_Stop();
    return 1;
}

// menuliskan data 1 byte kedalam MPU6050
void MPU6050WriteByte(uint8_t regAddr, uint8_t data) {
    //write data
    I2C_Start(MPU6050_ADDR | I2C_WRITE);
    I2C_Write(regAddr); //reg
    I2C_Write(data);
    I2C_Stop();
}

//Menuliskan beberapa bytes data ke dalam MPU6050
void mpu6050_writeBytes(uint8_t regAddr, uint8_t length, uint8_t* data) {
    uint8_t i;
    if(length > 0) {
        //write data
        I2C_Start(MPU6050_ADDR | I2C_WRITE);
        I2C_Write(regAddr); //reg
        for (i = 0; i < length; i++) {
            I2C_Write((uint8_t) data[i]);
        }
        I2C_Stop();
    }
}

// membaca beberapa bit data dari register pada MPU6050
int8_t mpu6050_readBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
    int8_t count = 0;
    if(length > 0) {
        uint8_t b;
        if ((count = MPU6050ReadByte(regAddr, &b)) != 0) {
            uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
            b &= mask;
            b >>= (bitStart - length + 1);
            *data = b;
        }
    }
    return count;
}

// Membaca 1 bit data dari register pada MPU6050
int8_t mpu6050_readBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
    uint8_t b;
    uint8_t count = MPU6050ReadByte(regAddr, &b);
    *data = b & (1 << bitNum);
    return count;
}

// menuliskan beberapa bits data pada register di MPU6050
void mpu6050_writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    if(length > 0) {
        uint8_t b = 0;
        if (MPU6050ReadByte(regAddr, &b) != 0) { //get current data
            uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
            data <<= (bitStart - length + 1); // shift data into correct position
            data &= mask; // zero all non-important bits in data
            b &= ~(mask); // zero all important bits in existing byte
            b |= data; // combine data with existing byte
            MPU6050WriteByte(regAddr, b);
        }
    }
}

// menuliskan 1 bit data pada register di MPU6050
void mpu6050_writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    MPU6050ReadByte(regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    MPU6050WriteByte(regAddr, b);
}

// mode sleep di non aktifkan
void MPU6050_SleepDisabled() {
    mpu6050_writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 0);
}

// mode sleep diaktifkan
void MPU6050_SleepEnabled() {
    mpu6050_writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 1);
}

// Memeriksa koneksi ke MPU6050
uint8_t MPU6050_TestConnection() {
    uint8_t buffer[1];
    mpu6050_readBits(MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, (uint8_t *)buffer);
    if(buffer[0] == 0x34)   return 1;
    else                    return 0;
}

// Inisialisasi MPU6050
void MPU6050_Init(void) 
{   //menunggu start-up dari mpu6050
    delay_ms(50);
    //mode sleep di non-aktifkan
    MPU6050_SleepDisabled();
    //wake up delay
    delay_ms(10);
    // pengaturan sumber clock dari MPU6050
    // direkomendasikan menggunakan salah satu dari gyroscopes (atau mengunakan sumber clock eksternal)
    mpu6050_writeBits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, MPU6050_CLOCK_PLL_XGYRO);
    //mengeset bandwidth dari  DLPF pada frekwensi 42Hz
    mpu6050_writeBits(MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, MPU6050_DLPF_BW_42);
    // pengaturan frekwensi sampling internal
    MPU6050WriteByte(MPU6050_RA_SMPLRT_DIV, 9); //1khz / (1 + 4) = 200Hz
    //pengaturan range dari gyroscope
    mpu6050_writeBits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, MPU6050_GYRO_FS);
    //pengaturan range dari accelerometer
    mpu6050_writeBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, MPU6050_ACCEL_FS);
                                  
#if UseCompass == 1
    // mem-bypass I2c bus slave untuk koneksi pada sensor magnetometer (HMC5883L)
    // menonaktifkan mode I2C master dari MPU6050
    mpu6050_writeBit(MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, 0);
    // mengaktifkan mode I2C slave bypass dari MPU6050
    mpu6050_writeBit(MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, 1);
#endif
}

// Membaca data Accelerometer 3 Axis
void BacaAccelerometer(void)
{   uint8_t buffer[6];
    MPU6050ReadBytes(MPU6050_RA_ACCEL_XOUT_H, 6, (uint8_t *)buffer);
    Ax = peekw(&buffer[4]); Ay = peekw(&buffer[2]); Az = peekw(&buffer[0]);
}

// Membaca data Gyroscope 3 Axis
void BacaGyroScope(void)
{   uint8_t buffer[6];
    MPU6050ReadBytes(MPU6050_RA_GYRO_XOUT_H, 6, (uint8_t *)buffer);
    Gx = peekw(&buffer[4]); Gy = peekw(&buffer[2]); Gz = peekw(&buffer[0]);
}

float invSqrt(float x) 
{	float halfx = 0.5f * x;
    float y = x;
    int32_t i = *(int32_t*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}
    
float inverse(float x)
{
  // re-interpret as a 32 bit integer
  uint16_t *i = (uint16_t*)&x;
  // adjust exponent
  *i = 0x7F000000 - *i;
  //*i = 0x7EEEEEEE - *i;
  return x;
}



/* Kalman filter variables */
// Q_acclero    = Process noise variance for the accelerometer
// Q_gyro;      = Process noise variance for the gyro bias
// R_measure    = Measurement noise variance - this is actually the variance of the measurement noise


#define     Q_accelero      0.001f 
#define     Q_gyro          0.003f
#define     R_measure       0.03f
//definitions for attitude 1 function estimation
#define WaktuSampling       0.005f // periode frekuensi dalam detik
   
float AccRoll;
// Perhitungan sudut Roll (Perputaran dgn poros sumbu Y) dengan menggunakan Kalman Filter
void HitungKalmanRoll(void)
{   static float P_00 = 0.01F, P_01 = 0.01F, P_10 = 0.01F, P_11 = 0.01F;
    float K_0, K_1, buffer;//, AccRoll;
        
buffer = WaktuSampling * P_11;
P_00 +=  WaktuSampling * (buffer - P_10 - P_01 + Q_accelero);
P_01 -=  buffer;
P_10 -=  buffer;
P_11 +=  WaktuSampling * Q_gyro;
        
buffer = 1.0f/(P_00 + R_measure);      // buffer = S
K_0 = P_00 * buffer;
K_1 = P_10 * buffer;
        
#if PosisiBoard == TIDUR
// Roll ClockWise = positif, CounterClockWise = negatif
Roll +=((float)Gx-GxOffset)*(WaktuSampling/MPU6050_GGAIN);  //0.000609756097561f;    
if(Az<0)
  {   AccRoll = atan((float)-Ay * invSqrt((float)Az*(float)Az  + (float)Ax*(float)Ax))*RAD2DEG;}
else
  {   //AccRoll  = atan2((float)-Ay, (float)-Az) * RAD2DEG;
if(Ay>0)    {   AccRoll = -90.0f - atan((float)Az * invSqrt((float)Ay*(float)Ay  + (float)Ax*(float)Ax))*RAD2DEG;   }
else        {   AccRoll = 90.0f + atan((float)Az * invSqrt((float)Ay*(float)Ay  + (float)Ax*(float)Ax))*RAD2DEG;    }
        }  
#else
Roll +=((float)Gz-GzOffset)*(WaktuSampling/MPU6050_GGAIN);  //0.000609756097561f;    
if(Ax>0)
  {   AccRoll = atan((float)-Ay * invSqrt((float)Az*(float)Az  + (float)Ax*(float)Ax))*RAD2DEG;   }
else
  { if(Ay>0)    
    { AccRoll = atan((float)Ax * invSqrt((float)Az*(float)Az  + (float)Ay*(float)Ay))*RAD2DEG - 90.0f;   }
    else        
    {   AccRoll = 90.0f - atan((float)Ax * invSqrt((float)Az*(float)Az  + (float)Ay*(float)Ay))*RAD2DEG; }
  }
#endif
  
buffer = AccRoll - Roll;     // buffer = y
Roll +=  K_0 * buffer;
GxOffset   +=  K_1 * buffer;
P_00 -= K_0 * P_00;
P_01 -= K_0 * P_01;
P_10 -= K_1 * P_00;
P_11 -= K_1 * P_01;
}
    
// Perhitungan sudut Pitch (Perputaran dgn poros sumbu X) dengan menggunakan Kalman Filter
void HitungKalmanPitch(void)
{   static float P_00 = 0.01F, P_01 = 0.01F, P_10 = 0.01F, P_11 = 0.01F;
float K_0, K_1, buffer, AccPitch;

buffer = WaktuSampling * P_11;
P_00 +=  WaktuSampling * (buffer - P_10 - P_01 + Q_accelero);
P_01 -=  buffer;
P_10 -=  buffer;
P_11 +=  (WaktuSampling * Q_gyro);

buffer = 1.0f/(P_00 + R_measure);      // buffer = S
K_0 = P_00 * buffer;
K_1 = P_10 * buffer;

#if PosisiBoard == TIDUR
Pitch +=((float)Gy-GyOffset)*(WaktuSampling/MPU6050_GGAIN);  //0.000609756097561f;
if(Az<0)
{   AccPitch = atan((float)Ax * invSqrt((float)Az*(float)Az  + (float)Ay*(float)Ay))*RAD2DEG;}
else
{   //AccPitch = atan2((float)Ax, (float)-Az) * RAD2DEG;
if(Ax>0){   AccPitch = 90.0f + atan((float)Az * invSqrt((float)Ax*(float)Ax  + (float)Ay*(float)Ay))*RAD2DEG;    }
else    {   AccPitch = -90.0f - atan((float)Az * invSqrt((float)Ax*(float)Ax  + (float)Ay*(float)Ay))*RAD2DEG;   }
}
#else
Pitch +=((float)Gy-GyOffset)*(WaktuSampling/MPU6050_GGAIN);  //0.000609756097561f;
if(Ax>0)
{   AccPitch = atan((float)Az * invSqrt((float)Ax*(float)Ax  + (float)Ay*(float)Ay))*RAD2DEG;}
else
{   if(Az>0){   AccPitch = 90.0f - atan((float)Ax * invSqrt((float)Az*(float)Az  + (float)Ay*(float)Ay))*RAD2DEG;   }
else    {   AccPitch = atan((float)Ax * invSqrt((float)Az*(float)Az  + (float)Ay*(float)Ay))*RAD2DEG - 90.0f;   }
}
#endif

buffer = AccPitch - Pitch;     // buffer = y
Pitch +=  K_0 * buffer;
GyOffset  +=  K_1 * buffer;
P_00 -= K_0 * P_00;
P_01 -= K_0 * P_01;
P_10 -= K_1 * P_00;
P_11 -= K_1 * P_01;
}


void KalibrasiGyroScope(void)
{   uint8_t i; 
    GxOffset = 0;       GyOffset = 0;   GzOffset = 0;
    for (i=0;i<100;i++)
    {   BacaGyroScope();
        GxOffset+=Gx;   GyOffset+=Gy;   GzOffset+=Gz;
        delay_ms(1);
    }
    GxOffset*=0.01;      GyOffset*=0.01;  GzOffset*=0.01;     // nilai 10000 x nilai asli
}
#pragma used-
#endif
