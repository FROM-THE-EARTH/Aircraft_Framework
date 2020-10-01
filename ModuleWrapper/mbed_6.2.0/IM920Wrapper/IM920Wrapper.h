/*
This program is developed under the mbed-os 6.2.0
*/

#include "IM920.h"
#include "ModuleWrapper.h"
#include <string>

class IM920Wrapper : public IM920, public ModuleWrapper
{

  bool sendToSerial_ = false;

public:
  IM920Wrapper(std::string name, PinName tx, PinName rx, PinName busy,
               PinName reset, int baud = IM920_BAUD)
      : IM920(tx, rx, busy, reset, baud), ModuleWrapper(name) {}

  virtual void initialize() override
  {
    available = IM920::init();
  }

  virtual bool isAvailable() override
  {
    return available;
  }

  void setSendToSerial(bool send)
  {
    sendToSerial_ = send;
  }

  void transmit(const std::string &str);

  std::string receive();
};