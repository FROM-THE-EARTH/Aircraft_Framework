/*
This program is developed under the mbed-os 6.2.0
*/

#include "ADXL345_I2C/ADXL345_I2C.h"
#include "../../ModuleWrapper.h"
#include <string>

class ADXLWrapper : public ADXL345_I2C, public ModuleWrapper
{

public:
  ADXLWrapper(xString name, PinName sda, PinName scl)
      : ADXL345_I2C(sda, scl), ModuleWrapper(name) {}

  virtual void initialize() override
  {
    available = getDeviceID() == 0xe5;
 
    //Go into standby mode to configure the device.
    setPowerControl(0x00);
 
    //resolution
    setDataFormatControl(0x0B);
    
    //data rate.
    setDataRate(ADXL345_1600HZ);
 
    //Measurement mode.
    setPowerControl(0x08);
  }

  virtual bool isAvailable() override
  {
    return available;
  }
};