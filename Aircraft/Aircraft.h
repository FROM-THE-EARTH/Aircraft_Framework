#include "AircraftMbedBase.h"

class Aircraft : public AircraftMbedBase {

public:
  Aircraft(float launchThreshold, float landingTime, float basePressure,
           IM920Wrapper *receiver, IM920Wrapper *transmitter, LPSWrapper *lps,
           LSMWrapper *imu)
      : AircraftMbedBase(launchThreshold, landingTime, basePressure, receiver,
                         transmitter, lps, imu) {}

private:
  // define detach condition
  virtual bool detachFlag() override;

  // define opening parachute condition
  virtual bool decelerationFlag() override;
};