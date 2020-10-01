#include "Aircraft.h"

Aircraft::Aircraft(float launchThreshold, float landingTime)
    : 
      launchThreshold_(launchThreshold),
      landingTime_(landingTime),
      AircraftWrapper()
{
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
  return  (datas.launchTime-datas.time) > landingTime_;
}

void Aircraft::detachAircraft()
{
}

void Aircraft::openParachute()
{
}