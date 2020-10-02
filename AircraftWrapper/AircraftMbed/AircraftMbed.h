/*
This program is developed under the mbed-os 6.2.0
*/

#include "AircraftBase.h"

#include "../../Module/mbed_6.2.0/IM920Wrapper/IM920Wrapper.h"
#include "../../Module/mbed_6.2.0/LPSWrapper/LPSWrapper.h"
#include "../../Module/mbed_6.2.0/LSMWrapper/LSMWrapper.h"
#include "../../Module/mbed_6.2.0/TimerWrapper/TimerWrapper.h"
#include "mbed.h"

class AircraftWrapper : public AircraftBase
{
protected:
  TimerWrapper timer_;

  //Modules
  IM920Wrapper receiver_;
  IM920Wrapper transmitter_;
  LPSWrapper lps_;
  LSMWrapper lsm_;
  //SDFileSystem sd(p5, p6, p7, p8, "sd");
  //DigitalIn flightPin(p12);
  //PwmOut servo_1(p21), servo_2(p22), servo_3(p23);

  AircraftWrapper()
      : AircraftBase(),
        receiver_("Receiver_A", p28, p27, p29, p30),
        transmitter_("Sender_A", p28, p27, p29, p30),
        lps_("LPS331_A", p9, p10, LPS331_I2C_SA0_HIGH),
        lsm_("LSM9DS1_A", p9, p10)
  {
  }

public:
  virtual bool initialize() override;

  virtual void setDebugMode(bool mode) override
  {
    transmitter_.setSendToSerial(mode);
    receiver_.setSendToSerial(mode);
  }

private:
  virtual void update() override;

  virtual void end() override;

  virtual void reboot() override
  {
    __NVIC_SystemReset();
  }

  virtual bool isReady(bool showDetail = true) override;

  virtual void getDatas() override;

  virtual void writeDatas() override;

  virtual void transmit(const xString& str) override{
    transmitter_.transmit(str);
  };

  virtual xString receive() override{
    return receiver_.receive();
  };

  void onReceive(){
    onReceiveCommand();
  }
};