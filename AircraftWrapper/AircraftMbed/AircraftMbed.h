/*
This program is developed under the mbed-os 6.2.0
*/

#include "AircraftBase.h"

#include "IM920Wrapper.h"
#include "LPSWrapper.h"
#include "LSMWrapper.h"
#include "TimerWrapper.h"
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

  Commands commands_ = Commands::None;

  AircraftWrapper()
      : AircraftBase(),
        receiver_("Receiver_A", p28, p27, p29, p30),
        transmitter_("Sender_A", p28, p27, p29, p30),
        lps_("LPS331_A", p9, p10, LPS331_I2C_SA0_HIGH),
        lsm_("LSM9DS1_A", p9, p10)
  {
  }

  // set launch condition
  virtual bool launchCondition() = 0;

  // set detaching condition
  virtual bool detachCondition() = 0;

  // set opening parachute condition
  virtual bool decelerationCondition() = 0;

  // set landing condition
  virtual bool landingCondition() = 0;

  // detaching operation
  virtual void detachAircraft() = 0;

  // opening parachute operation
  virtual void openParachute() = 0;

public:
  virtual bool initialize() override;

  virtual void setDebugMode(bool mode) override
  {
    transmitter_.setSendToSerial(mode);
    receiver_.setSendToSerial(mode);
  }

private:
  virtual void update() override;

  virtual bool isReady(bool showDetail = false) override;

  virtual void end() override;

  virtual void waiting() override;

  virtual void waitingLaunch() override;

  virtual void inFlight() override;

  virtual void landing() override;

  virtual void getDatas() override;

  virtual void writeDatas() override;

  virtual void onReceive() override;
};