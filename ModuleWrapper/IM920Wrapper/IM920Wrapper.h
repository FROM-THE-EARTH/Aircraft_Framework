#include "IM920.h"
#include "ModuleWrapper.h"
#include <string>

class IM920Wrapper : public IM920, public ModuleWrapper {

  BufferedSerial *serial_;

  bool sendToSerial_ = false;

  // char sendBuffer[256];

public:
  IM920Wrapper(std::string name, PinName tx, PinName rx, PinName busy,
               PinName reset, int baud = IM920_BAUD)
      : IM920(tx, rx, busy, reset, baud), ModuleWrapper(name) {}

  virtual void initialize() override { available = IM920::init(); }

  virtual bool isAvailable() override { return available; }

  void setSerial(BufferedSerial *serial) { serial_ = serial; }

  void setSendToSerial(bool send) { sendToSerial_ = send; }

  // template <typename... Args>
  // void send(int len, const char *format, Args const &... args);

  void transmit(std::string str);

  std::string receive();
};

/*template <typename... Args>
void IM920Wrapper::send(int len, const char *format, Args const &... args) {
  if (sendToSerial_ && serial_->writable()) {
      printf((name_+": ").c_str());
      printf(format, args...);
      printf("\r\n");
  }

  sprintf(sendBuffer, format, args...);
  IM920::send(sendBuffer, len);
}*/