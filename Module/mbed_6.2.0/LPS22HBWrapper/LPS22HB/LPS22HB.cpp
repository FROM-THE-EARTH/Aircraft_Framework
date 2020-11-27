/*
 * mbed library program
 *  LPS22HB MEMS pressure sensor: 260-1260 hPa absolute digital output barometer
 *   made by STMicroelectronics
 *   http://www.st.com/ja/mems-and-sensors/lps22hb.html
 *
 * Modified for LPS22HB by Taro Watanabe
 *  http://mbed.org/users/feunoir/
 * 
 * Copyright (c) 2015 Kenji Arai / JH1PJL
 *  http://www.page.sannet.ne.jp/kenjia/index.html
 *  http://mbed.org/users/kenjiArai/
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "LPS22HB.h"

LPS22HB::LPS22HB (PinName p_sda, PinName p_scl, uint8_t addr)
    :
    i2c_p(new I2C(p_sda, p_scl)),
    _i2c(*i2c_p)
{
    LPS22HB_addr = addr;
    init();
}

LPS22HB::LPS22HB (I2C& p_i2c, uint8_t addr)
    :
    i2c_p(NULL),
    _i2c(p_i2c)
{
    LPS22HB_addr = addr;
    init();
}

LPS22HB::~LPS22HB()
{
    if( NULL != i2c_p )
        delete  i2c_p;
}

/////////////// Initialize ////////////////////////////////
void LPS22HB::init(void)
{
    _i2c.frequency(400000);
    // Check acc is available of not
    dt[0] = LPS22HB_WHO_AM_I;
    _i2c.write(LPS22HB_addr, dt, 1, true);
    _i2c.read(LPS22HB_addr, dt, 1, false);
    if (dt[0] == I_AM_LPS22HB) {
        LPS22HB_id = I_AM_LPS22HB;
        LPS22HB_ready = 1;
    } else {
        LPS22HB_id = 0;
        LPS22HB_ready = 0;
        return;     // acc chip is NOT on I2C line then terminate
    }
    
    set_odr(LPS22HB_ODR_1HZ);
}

/////////////// Start conv. and gwt all data //////////////
void LPS22HB::get(void)
{
    if (LPS22HB_ready == 0) {
        press = 0;
        temp = 0;
        return;
    }
    dt[0] = LPS22HB_PRESS_POUT_XL | 0x80;
    _i2c.write(LPS22HB_addr, dt, 1, true);
    _i2c.read(LPS22HB_addr, dt, 3, false);
    press = dt[2] << 16 | dt[1] << 8 | dt[0];
    dt[0] = LPS22HB_TEMP_OUT_L | 0x80;
    _i2c.write(LPS22HB_addr, dt, 1, true);
    _i2c.read(LPS22HB_addr, dt, 2, false);
    temp = dt[1] << 8 | dt[0];
}

/////////////// Read data from sensor /////////////////////
float LPS22HB::pressure()
{
    return (float)press / 4096.0f;
}

/////////////// Read raw data from sensor /////////////////
uint32_t LPS22HB::pressure_raw()
{
    return press;
}

/////////////// Read data from sensor /////////////////////
float LPS22HB::temperature()
{
    return (float)temp / 100.0f;
}

/////////////// Read raw data from sensor /////////////////
int16_t LPS22HB::temperature_raw()
{
    return temp;
}

/////////////// ID ////////////////////////////////////////
uint8_t LPS22HB::read_id()
{
    dt[0] = LPS22HB_WHO_AM_I;
    _i2c.write(LPS22HB_addr, dt, 1, true);
    _i2c.read(LPS22HB_addr, dt, 1, false);
    return (uint8_t)dt[0];
}

/////////////// I2C Freq. /////////////////////////////////
void LPS22HB::frequency(int hz)
{
    _i2c.frequency(hz);
}

/////////////// General purpose R/W ///////////////////////
uint8_t LPS22HB::read_reg(uint8_t addr)
{
    if (LPS22HB_ready == 1) {
        dt[0] = addr;
        _i2c.write(LPS22HB_addr, dt, 1, true);
        _i2c.read(LPS22HB_addr, dt, 1, false);
    } else {
        dt[0] = 0xff;
    }
    return (uint8_t)dt[0];
}

void LPS22HB::write_reg(uint8_t addr, uint8_t data)
{
    if (LPS22HB_ready == 1) {
        dt[0] = addr;
        dt[1] = data;
        _i2c.write(LPS22HB_addr, dt, 2, false);
    }
}
    
/////////////// ODR ///////////////////////////////////////
void LPS22HB::set_odr(lps22hb_odr odrcfg)
{
    uint8_t temp = read_reg(LPS22HB_CTRL_REG1);
    temp &= 0xff ^ 0x70;
    temp |= odrcfg;
    write_reg(LPS22HB_CTRL_REG1, temp);
}

/////////////// LPF ///////////////////////////////////////
void LPS22HB::set_lpf(lps22hb_lpf lpfcfg)
{
    uint8_t temp = read_reg(LPS22HB_CTRL_REG1);
    temp &= 0xff ^ 0x0c;
    temp |= lpfcfg;
    write_reg(LPS22HB_CTRL_REG1, temp);
}

/////////////// DRDY //////////////////////////////////////
void LPS22HB::drdy(lps22hb_drdy drdycfg)
{
    uint8_t temp = read_reg(LPS22HB_CTRL_REG3);
    temp &= 0xff ^ 0x04;
    temp |= drdycfg;
    write_reg(LPS22HB_CTRL_REG3, temp);
}

bool LPS22HB::whoAmI()
{
    dt[0] = LPS22HB_WHO_AM_I;
    _i2c.write(LPS22HB_addr, dt, 1, true);
    _i2c.read(LPS22HB_addr, dt, 1, false);

    return dt[0] == I_AM_LPS22HB;
}