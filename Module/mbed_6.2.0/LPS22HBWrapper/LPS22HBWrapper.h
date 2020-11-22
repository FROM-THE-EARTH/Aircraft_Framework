/*
This program is developed under the mbed-os 6.2.0
*/

#include "LPS22HB/LPS22HB.h"
#include  "ModuleWrapper.h"

class LPS22HBWrapper : public LPS22HB, public ModuleWrapper
{

public:
  LPS22HBWrapper(xString name, PinName sda, PinName scl)
      : LPS22HB(sda, scl, LPS22HB_G_CHIP_ADDR), ModuleWrapper(name)
  {
  }

  virtual void initialize() override
  {
  }

  virtual bool isAvailable() override
  {
      return LPS22HB::whoAmI();
  }
};