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

void AircraftBase::waiting()
{
  if (isElapsed(5.0f))
  {
    transmit("Waiting");
  }
  //beginRecord();
}

void AircraftBase::waitingLaunch()
{
  if (isElapsed(5.0f))
  {
    transmit("Waiting launch");
  }
  if (launchCondition())
  {
    datas.launchTime = datas.time;
    //beginRecord();
    sequence = Sequence::InFlight;
    transmit("Launch");
  }
}

void AircraftBase::inFlight()
{
  if (isElapsed(5.0f))
  {
    transmit(to_XString(datas.time));
  }
  if (!detached_ && detachCondition())
  {
    datas.detachTime = datas.time;
    detachAircraft();
    transmit("Detach");
    detached_ = true;
  }

  if (!decelerationStarted_ && decelerationCondition())
  {
    datas.decelerationTime = datas.time;
    openParachute();
    transmit("Open parachute");
    decelerationStarted_ = true;
  }

  if (landingCondition())
  {
    datas.landingTime = datas.time;
    endRecord();
    transmit("Landing");
    sequence = Sequence::Landing;
  }
}

void AircraftBase::landing()
{
  // transmit gps info
  if (isElapsed(5.0f))
  {
    transmit(to_XString(datas.latitude) + "N, " + to_XString(datas.longitude) + "E");
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

void AircraftBase::onReceiveCommand()
{
  switch (checkCommand(receive()))
  {
  case Commands::Reboot:
    reboot();
    break;

  case Commands::EscapePreparing:
    //beginRecord();
    sequence = Sequence::ReadyToLaunch;
    break;

  case Commands::CheckSensors:
    isReady(true);
    break;

  case Commands::ClosingServo:
    // Close servo
    break;

  case Commands::None:
    transmit("Invalid command");
    break;
  }
}

void AircraftBase::applyIMUFilter()
{
  madgwick.update(datas.accel, datas.gyro, datas.deltaTime);
  //madgwick.update(datas.accel, datas.gyro, datas.magn, datas.deltaTime);

  datas.roll = madgwick.getRoll();
  datas.pitch = madgwick.getPitch();
  datas.yaw = madgwick.getYaw();
}