/*
 *  I2C/SPI digital pressure sensor "LPS331AP" library for I2C mode.
 *
 *  Copyright(c) -2013 unos@NYAMFG, 
 *  Released under the MIT License: http://mbed.org/license/mit
 *
 *  revision: see LPS331_I2C.h.
 */

#include "LPS331_I2C.h"

LPS331_I2C::LPS331_I2C(PinName sda, PinName scl, bool sa0) : _i2c(sda, scl)
{
    if(sa0 == LPS331_I2C_SA0_HIGH) {
        _address = LPS331_I2C_ADDRESS_SA0_HIGH;
    } else {
        _address = LPS331_I2C_ADDRESS_SA0_LOW;
    }
    _i2c.frequency(400 * 1000);
    _ctrlreg1 = 0x20;
}

LPS331_I2C::~LPS331_I2C()
{
}

char LPS331_I2C::whoami()
{
    return _read(0x0f);
}

bool LPS331_I2C::isAvailable()
{
    if(whoami() == 0xbb) { return true; }
    
    return false;
}

void LPS331_I2C::setResolution(char pressure_avg, char temp_avg)
{
    _write(0x10, ((temp_avg & 0x07) << 4) | (pressure_avg & 0x0f));
}

void LPS331_I2C::setActive(bool is_active)
{
    if(is_active) {
        _ctrlreg1 |= 0x80;
    } else {
        _ctrlreg1 &= ~0x80;
    }

    _write(0x20, _ctrlreg1);
}

void LPS331_I2C::setDataRate(char datarate)
{
    datarate &= 0x07;
    
    _ctrlreg1 &= ~(0x07 << 4);
    _ctrlreg1 |= datarate << 4;
    
    _write(0x20, _ctrlreg1);
}

    
float LPS331_I2C::getPressure()
{
    char data[3];
    float pressure = 0;

    _read_multibyte(0x28, data, 3);
    
    pressure  = data[0];
    pressure += data[1] << 8;
    pressure += data[2] << 16;
    pressure /= 4096.0;
    
    return pressure;
}

float LPS331_I2C::getTemperature()
{
    char data[2];
    short temp = 0;
    
    _read_multibyte(0x2b, data, 2);    
    
    temp  = data[0];
    temp |= data[1] << 8;
    
    return (float)(42.5 + temp / 480.0);
}


void LPS331_I2C::_write(char subaddress, char data)
{
    _i2c.start();
    _i2c.write(_address);
    _i2c.write(subaddress);
    _i2c.write(data);
    _i2c.stop();
}

char LPS331_I2C::_read(char subaddress)
{
    char result = 0;
    
    _i2c.start();
    _i2c.write(_address);
    _i2c.write(subaddress);
    
    _i2c.start();
    _i2c.write(_address | 1);
    result = _i2c.read(0);
    
    _i2c.stop();
    
    return result;
}

void LPS331_I2C::_read_multibyte(char startsubaddress, char* data, char count)
{
    _i2c.start();
    _i2c.write(_address);
    _i2c.write(startsubaddress | 0x80);
    
    _i2c.start();
    _i2c.write(_address | 1);

    for(int i = 0; i < count; i++) {
        data[i] = _i2c.read((i == count - 1) ? 0 : 1);
    }
    
    _i2c.stop();
}

