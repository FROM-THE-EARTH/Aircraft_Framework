/*
 * mbed library program
 *  LPS22HB MEMS pressure sensor: 260-1260 hPa absolute digital output barometer
 *   made by STMicroelectronics
 *   http://www.st.com/ja/mems-and-sensors/lps22hb.html
 *
 * Modified for LPS22HB by feunoir
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
/*
 *---------------- REFERENCE ----------------------------------------------------------------------
 * Original Information
 *  http://www.st.com/ja/mems-and-sensors/lps22hb.html
 * AN4833 Application Note (as follows)
 *  http://www.st.com/resource/en/application_note/dm00269729.pdf
 * Device kit
 *  https://strawberry-linux.com/catalog/items?code=12122
 */

#ifndef LPS22HB_H
#define LPS22HB_H

#include "mbed.h"

//  LPS22HB Address
//  7bit address = 0b101110x(0x5c or 0x5d depends on SA0/SDO)
#define LPS22HB_G_CHIP_ADDR  (0x5c << 1)    // SA0(=SDO pin) = Ground
#define LPS22HB_V_CHIP_ADDR  (0x5d << 1)    // SA0(=SDO pin) = Vdd

//   LPS22HB ID
#define I_AM_LPS22HB            0xb1

//  Register's definition
#define LPS22HB_WHO_AM_I        0x0f
#define LPS22HB_RES_CONF        0x1a

#define LPS22HB_CTRL_REG1       0x10
#define LPS22HB_CTRL_REG2       0x11
#define LPS22HB_CTRL_REG3       0x12

#define LPS22HB_STATUS_REG      0x27
#define LPS22HB_PRESS_POUT_XL   0x28
#define LPS22HB_PRESS_OUT_L     0x29
#define LPS22HB_PRESS_OUT_H     0x2a
#define LPS22HB_TEMP_OUT_L      0x2b
#define LPS22HB_TEMP_OUT_H      0x2c

#define LPS22HB_FIFO_CTRL       0x14
#define LPS22HB_FIFO_STATUS     0x26


/** Interface for STMicronics MEMS pressure sensor
 *      Chip: LPS22HB
 *
 * @code
 * #include "mbed.h"
 * #include "LPS22HB.h"
 *
 * // I2C Communication
 * LPS22HB baro(p_sda, p_scl, LPS22HB_G_CHIP_ADDR);
 * // If you connected I2C line not only this device but also other devices,
 * //     you need to declare following method.
 * I2C i2c(dp5,dp27);              // SDA, SCL
 * LPS22HB baro(i2c, LPS22HB_G_CHIP_ADDR);
 *
 * int main() {
 *   while( trure){
 *      baro.get();
 *      printf("Presere: 0x%6.1f, Temperature: 0x%+4.1f\r\n", baro.pressue(), baro.temperature());
 *      wait(1.0);
 *   }
 * }
 * @endcode
 */

class LPS22HB
{
public:
    
    enum lps22hb_odr {
        LPS22HB_PD       = 0x00,
        LPS22HB_ODR_1HZ  = 0x10,
        LPS22HB_ODR_10HZ = 0x20,
        LPS22HB_ODR_25HZ = 0x30,
        LPS22HB_ODR_50HZ = 0x40,
        LPS22HB_ODR_75HZ = 0x50
    };
    
    enum lps22hb_lpf {
        LPS22HB_LPF_DISABLE = 0x00,
        LPS22HB_LPF_BW9     = 0x08,
        LPS22HB_LPF_BW20    = 0x0c
    };
    
    enum lps22hb_drdy {
        LPS22HB_DRDY_DISABLE = 0x00,
        LPS22HB_DRDY_ENABLE  = 0x04
    };

    /** Configure data pin
      * @param data SDA and SCL pins
      * @param device address LPS22HB(SA0=0 or 1), LPS22HB_G_CHIP_ADDR or LPS22HB_V_CHIP_ADDR
      * @param Operation mode FIFO_HW_FILTER(default) or FIFO_BYPASS (Option parameter)
      */
    LPS22HB(PinName p_sda, PinName p_scl, uint8_t addr);
    
    /** Configure data pin (with other devices on I2C line)
      * @param I2C previous definition
      * @param device address LPS22HB(SA0=0 or 1), LPS22HB_G_CHIP_ADDR or LPS22HB_V_CHIP_ADDR
      * @param Operation mode FIFO_HW_FILTER(default) or FIFO_BYPASS (Option parameter)
      */
    LPS22HB(I2C& p_i2c, uint8_t addr);
    
    ~LPS22HB();

    /** Start convertion & data save
      * @param none
      * @return none
      */
    void get(void);

    /** Read pressure data
      * @param none
      * @return pressure
      */
    float pressure(void);
    
    /** Read raw pressure data
      * @param none
      * @return raw pressure
      */
    uint32_t pressure_raw(void);    //  add by user

    /** Read temperature data
      * @param none
      * @return temperature
      */
    float temperature(void);
    
    /** Read raw temperature data
      * @param none
      * @return raw temperature
      */
    int16_t temperature_raw(void);    //  add by user

    /** Read a ID number
      * @param none
      * @return if STM MEMS LPS22HB, it should be I_AM_ LPS22HB
      */
    uint8_t read_id(void);

    /** Read Data Ready flag
      * @param none
      * @return 1 = Ready
      */
    uint8_t data_ready(void);

    /** Set I2C clock frequency
      * @param freq.
      * @return none
      */
    void frequency(int hz);

    /** Read register (general purpose)
      * @param register's address
      * @return register data
      */
    uint8_t read_reg(uint8_t addr);

    /** Write register (general purpose)
      * @param register's address
      * @param data
      * @return none
      */
    void write_reg(uint8_t addr, uint8_t data);
    
    /** Set output data rate
      * @param data rate config
      * @return none
      */
    void set_odr(lps22hb_odr odrcfg = LPS22HB_ODR_75HZ);    //add by user
    
    /** LPF
      * @param LPF config
      * @return none
      */
    void set_lpf(lps22hb_lpf lpfcfg = LPS22HB_LPF_DISABLE); //add by user
    
    /** DRDY
      * @param DRDY config
      * @return none
      */
    void drdy(lps22hb_drdy drdycfg = LPS22HB_DRDY_DISABLE); //add by user
    
    bool whoAmI();
    
protected:
    I2C* i2c_p;
    I2C& _i2c;

    void init(void);

private:
    char dt[6];            // working buffer
    uint8_t LPS22HB_addr;   // Sensor address
    uint8_t LPS22HB_id;     // ID
    uint8_t LPS22HB_ready;  // Device is on I2C line = 1, not = 0
    uint8_t LPS22HB_mode;   // Operation mode
    uint32_t press;        // pressure raw data
    int16_t temp;          // temperature raw data
};

#endif      // LPS22HB_H