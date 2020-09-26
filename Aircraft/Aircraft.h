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
  Aircraft(float launchThreshold, float landingTime, float launcherDirection, float launcherElevation);

private:
  virtual bool launchCondition() override;

  virtual bool detachCondition() override;

  virtual bool decelerationCondition() override;

  virtual bool landingCondition() override;

  virtual void detachAircraft() override;

  virtual void openParachute() override;
};