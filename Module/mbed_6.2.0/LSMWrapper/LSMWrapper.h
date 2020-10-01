/*
This program is developed under the mbed-os 6.2.0
*/

#include "LSM9DS1/LSM9DS1.h"
#include "../../ModuleWrapper.h"

class LSMWrapper : public LSM9DS1, public ModuleWrapper
{

public:
  LSMWrapper(xString name, PinName sda, PinName scl,
             uint8_t xgAddr = LSM9DS1_AG_I2C_ADDR(1),
             uint8_t mAddr = LSM9DS1_M_I2C_ADDR(1))
      : LSM9DS1(sda, scl, xgAddr, mAddr), ModuleWrapper(name) {}

  virtual void initialize() override
  {
    available = LSM9DS1::begin();
  }

  virtual bool isAvailable() override
  {
    return available && LSM9DS1::whoAmI();
  }
};