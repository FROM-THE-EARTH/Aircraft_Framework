#include "AircraftBase.h"
#include "Utils.h"

void AircraftBase::begin()
{
  while (true)
  {
    update();

    getDatas();

    if (imuFilter_)
    {
      applyIMUFilter();
    }

    if (recording_)
    {
      writeDatas();
    }

    switch (scene)
    {
    case Scene::Waiting:
      waiting();
      break;

    case Scene::ReadyToLaunch:
      waitingLaunch();
      break;

    case Scene::InFlight:
      inFlight();
      break;

    case Scene::Landing:
      landing();
      break;
    }
  }
}

AircraftBase::Commands AircraftBase::checkCommand(const std::string &recv)
{
  if (recv == "reset")
  {
    return Commands::ResetMbed;
  }
  if (recv == "escape")
  {
    return Commands::EscapePreparing;
  }
  if (recv == "check")
  {
    return Commands::CheckSensors;
  }
  if (recv == "svclose")
  {
    return Commands::ClosingServo;
  }
  return Commands::None;
}

void AircraftBase::applyIMUFilter()
{
  madgwick.update(datas.accel, datas.gyro, datas.deltaTime);
  //madgwick.update(datas.accel, datas.gyro, datas.magn, datas.deltaTime);

  datas.roll = madgwick.getRoll();
  datas.pitch = madgwick.getPitch();
  datas.yaw = madgwick.getYaw();
}