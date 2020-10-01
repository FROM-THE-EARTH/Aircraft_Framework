/*
 *  I2C/SPI digital pressure sensor "LPS331AP" library for I2C mode.
 *
 *  http://www.st.com/web/en/resource/technical/document/datasheet/DM00036196.pdf
 *
 *  Copyright(c) -2013 unos@NYAMFG, 
 *  Released under the MIT License: http://mbed.org/license/mit
 *
 *  revision 1.1    22-Oct-2013     Add multibyte read, Change temperature and pressure reading method.
 *  revision 1.0    20-Oct-2013     1st release, Does not support interrupts.
 */

#ifndef LPS331_I2C_H
#define LPS331_I2C_H

#include "mbed.h"

// SA0 status configuration values.
#define LPS331_I2C_SA0_HIGH         true
#define LPS331_I2C_SA0_LOW          false

// Pressure configuration values.
#define LPS331_I2C_PRESSURE_AVG_1   0x00
#define LPS331_I2C_PRESSURE_AVG_2   0x01
#define LPS331_I2C_PRESSURE_AVG_4   0x02
#define LPS331_I2C_PRESSURE_AVG_8   0x03
#define LPS331_I2C_PRESSURE_AVG_16  0x04
#define LPS331_I2C_PRESSURE_AVG_32  0x05
#define LPS331_I2C_PRESSURE_AVG_64  0x06
#define LPS331_I2C_PRESSURE_AVG_128 0x07
#define LPS331_I2C_PRESSURE_AVG_256 0x08
#define LPS331_I2C_PRESSURE_AVG_384 0x09
#define LPS331_I2C_PRESSURE_AVG_512 0x0a

// Temperature configuration values.
#define LPS331_I2C_TEMP_AVG_1       0x00
#define LPS331_I2C_TEMP_AVG_2       0x01
#define LPS331_I2C_TEMP_AVG_4       0x02
#define LPS331_I2C_TEMP_AVG_8       0x03
#define LPS331_I2C_TEMP_AVG_16      0x04
#define LPS331_I2C_TEMP_AVG_32      0x05
#define LPS331_I2C_TEMP_AVG_64      0x06
#define LPS331_I2C_TEMP_AVG_128     0x07

// Data Rate                                   Pressure / Temperature 
#define LPS331_I2C_DATARATE_ONESHOT 0x00    // OneShot    OneShot
#define LPS331_I2C_DATARATE_1HZ     0x01    // 1Hz        1Hz
#define LPS331_I2C_DATARATE_7HZ     0x02    // 7Hz        1Hz
#define LPS331_I2C_DATARATE_12_5HZ  0x03    // 12.5Hz     1Hz
#define LPS331_I2C_DATARATE_25HZ    0x04    // 25Hz       1Hz
#define LPS331_I2C_DATARATE_7HZ_T   0x05    // 7Hz        7Hz
#define LPS331_I2C_DATARATE_12_5HZ_T 0x06   // 12.5Hz     12.5Hz
#define LPS331_I2C_DATARATE_25HZ_T  0x07    // 25Hz       25Hz (*)
// (*) Not allowed with PRESSURE_AVG_512 & TEMP_AVG_128.
//     More information , see datasheet.

// I2C Address.
#define LPS331_I2C_ADDRESS_SA0_HIGH 0xba   
#define LPS331_I2C_ADDRESS_SA0_LOW  0xb8

class LPS331_I2C
{
public:
    LPS331_I2C(PinName sda, PinName scl, bool sa0);
    ~LPS331_I2C();

    char whoami();
    bool isAvailable();
    
    void setResolution(char pressure_avg, char temp_avg);
    void setActive(bool is_active);
    void setDataRate(char datarate);

    float getPressure();
    float getTemperature();

    void _write(char subaddress, char data);
    char _read(char subaddress);
    void _read_multibyte(char startsubaddress, char* data, char count);
        
private:
    I2C _i2c;
    char _address;
    char _ctrlreg1;
};


#endif /* LPC331_I2C_H */

