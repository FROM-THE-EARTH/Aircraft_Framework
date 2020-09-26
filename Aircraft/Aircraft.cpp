#include "Aircraft.h"

Aircraft::Aircraft(float launchThreshold, float landingTime, float launcherDirection, float launcherElevation)
    : receiver_("Receiver_A", p28, p27, p29, p30),
      transmitter_("Sender_A", p28, p27, p29, p30),
      lps331_("LPS331_A", p9, p10, LPS331_I2C_SA0_HIGH),
      lsm_("LSM9DS1_A", p9, p10),

      launchThreshold_(launchThreshold),
      landingTime_(landingTime),

      AircraftMbedBase(&receiver_, &transmitter_, &lps331_, &lsm_)
{
  datas.quaternion = Quaternion(launcherDirection, launcherElevation);
}

bool Aircraft::launchCondition()
{
  return datas.accel.length() > launchThreshold_;
}

bool Aircraft::detachCondition()
{
  return (datas.altitude > 500);
}

bool Aircraft::decelerationCondition()
{
  return (datas.altitude < datas.maxAltitude - 10);
}

bool Aircraft::landingCondition()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             timer_.elapsed_time())
             .count() > landingTime_;
}

void Aircraft::detachAircraft()
{
}

void Aircraft::openParachute()
{
}