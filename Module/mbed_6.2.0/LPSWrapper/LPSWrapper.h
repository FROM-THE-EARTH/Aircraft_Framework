/*
This program is developed under the mbed-os 6.2.0
*/

#include "LPS331_I2C/LPS331_I2C.h"
#include "../../ModuleWrapper.h"

class LPSWrapper : public LPS331_I2C, public ModuleWrapper
{

public:
  LPSWrapper(xString name, PinName sda, PinName scl, bool sa0)
      : LPS331_I2C(sda, scl, sa0), ModuleWrapper(name)
  {
  }

  virtual void initialize() override
  {
    LPS331_I2C::setActive(true);
  }

  virtual bool isAvailable() override
  {
    return LPS331_I2C::isAvailable();
  }
};