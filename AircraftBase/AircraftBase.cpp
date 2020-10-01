#include "AircraftBase.h"
#include "../Utils/Utils.h"

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

    switch (sequence)
    {
    case Sequence::Waiting:
      waiting();
      break;

    case Sequence::ReadyToLaunch:
      waitingLaunch();
      break;

    case Sequence::InFlight:
      inFlight();
      break;

    case Sequence::Landing:
      landing();
      break;
    }
  }
}

AircraftBase::Commands AircraftBase::checkCommand(const xString &recv)
{
  if (recv == "reboot")
  {
    return Commands::Reboot;
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