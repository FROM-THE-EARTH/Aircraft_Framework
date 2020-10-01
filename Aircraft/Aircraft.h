#pragma once

#include "../AircraftWrapper/AircraftWrapper.h"

class Aircraft : public AircraftWrapper
{
  const float launchThreshold_; //[G]

  // (now - openParachuteTime) > landingTime => switching to Landing sequence from InFlight sequence
  const float landingTime_; //[s]

public:
  Aircraft(float launchThreshold, float landingTime);

private:
  virtual bool launchCondition() override;

  virtual bool detachCondition() override;

  virtual bool decelerationCondition() override;

  virtual bool landingCondition() override;

  virtual void detachAircraft() override;

  virtual void openParachute() override;
};