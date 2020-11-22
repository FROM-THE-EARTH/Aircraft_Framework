/*
This program is developed under the mbed-os 6.2.0
*/

#include "AvionicsBase.h"

#include "IM920Wrapper.h"
#include "IM920SoftSerial.h"
#include "LPS22HBWrapper.h"
#include "LSMWrapper.h"
#include "ADXLWrapper.h"
#include "TimerWrapper.h"
#include "GPS.h"
#include "SDCardWrapper.h"

#include "mbed.h"

class Avionics : public AvionicsBase
{
protected:
  TimerWrapper timer_;

  //Modules
  IM920SoftSerial receiver_;
  IM920Wrapper transmitter_;
  LPS22HBWrapper lps_;
  LSMWrapper lsm_;
  ADXLWrapper adxl_;
  GPS gps_;
  SDCardWrapper sd_;

public:
  Avionics(bool imuFilter = true, bool useMagnInMadgwick = false)
      : AvionicsBase(true, imuFilter, useMagnInMadgwick),
        receiver_("Receiver", p15, p16),
        transmitter_("Sender", p28, p27, p29, p30),
        lps_("LPS33HW", p9, p10),
        lsm_("LSM9DS1", p9, p10),
        adxl_("ADXL345", p9, p10),
        gps_(p13, p14),
        sd_("SD", p5, p6, p7, p8)
  {
  }

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
    sd_.close();
    __NVIC_SystemReset();
  }

  virtual bool isReady(bool showDetail = true) override;

  virtual void getDatas() override;

  virtual void writeDatas() override;

  virtual void transmit(const xString &str) override
  {
    transmitter_.transmit(str);
  };

  virtual xString received() override
  {
    return receiver_.received();
  };

  virtual void closeSDCard() override{
    sd_.close();
  }

  void onReceive(){
    onReceiveCommand();
  }
};