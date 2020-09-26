#include "AircraftMbedBase.h"

class Aircraft : public AircraftMbedBase
{
  const float launchThreshold_;
  const float landingTime_;

  //Modules
  IM920Wrapper receiver_;
  IM920Wrapper transmitter_;
  LPSWrapper lps331_;
  LSMWrapper lsm_;
// SDFileSystem sd(p5, p6, p7, p8, "sd");
// DigitalIn flightPin(p12);
// PwmOut servo_1(p21), servo_2(p22), servo_3(p23);

public:
  Aircraft(float launchThreshold, float landingTime)
      :receiver_("Receiver_A", p28, p27, p29, p30),
      transmitter_("Sender_A", p28, p27, p29, p30),
      lps331_("LPS331_A", p9, p10, LPS331_I2C_SA0_HIGH),
      lsm_("LSM9DS1_A", p9, p10),

      launchThreshold_(launchThreshold),
      landingTime_(landingTime),
      
      AircraftMbedBase(&receiver_, &transmitter_, &lps331_, &lsm_)
      {}

private:
  virtual bool launchCondition() override;

  virtual bool detachCondition() override;

  virtual bool decelerationCondition() override;

  virtual bool landingCondition() override;

  virtual void detachAircraft() override;

  virtual void openParachute() override;
};