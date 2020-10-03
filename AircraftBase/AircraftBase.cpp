#include "AircraftBase.h"
#include "../Utils/Utils.h"

void AircraftBase::begin()
{
  if (!checkFunctions())
  {
    transmit("[WARNING] Some function is not defined");
    transmit("[WARNING] Processing will be stopped");
    return;
  }

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

    switch (sequence_)
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

bool AircraftBase::checkFunctions()
{
  bool ok = true;

  ok &= Condition_Launch != nullptr;
  ok &= Condition_Detach != nullptr;
  ok &= Condition_Deceleration != nullptr;
  ok &= Condition_Landing != nullptr;
  ok &= Operation_Detach != nullptr;
  ok &= Operation_OpenParachute != nullptr;

  return ok;
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
  if (Condition_Launch())
  {
    datas.launchTime = datas.time;
    //beginRecord();
    sequence_ = Sequence::InFlight;
    transmit("Launch");
  }
}

void AircraftBase::inFlight()
{
  if (isElapsed(5.0f))
  {
    transmit(to_XString(datas.time));
  }
  if (!detached_ && Condition_Detach())
  {
    datas.detachTime = datas.time;
    Operation_Detach();
    transmit("Detach");
    detached_ = true;
  }

  if (!decelerationStarted_ && Condition_Deceleration())
  {
    datas.decelerationTime = datas.time;
    Operation_OpenParachute();
    transmit("Open parachute");
    decelerationStarted_ = true;
  }

  if (Condition_Landing())
  {
    datas.landingTime = datas.time;
    endRecord();
    transmit("Landing");
    sequence_ = Sequence::Landing;
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
    sequence_ = Sequence::ReadyToLaunch;
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
  if (useMagnInMadgwick_)
  {
    madgwick_.update(datas.accel, datas.gyro, datas.magn, datas.deltaTime);
  }
  else
  {
    madgwick_.update(datas.accel, datas.gyro, datas.deltaTime);
  }

  datas.roll = madgwick_.getRoll();
  datas.pitch = madgwick_.getPitch();
  datas.yaw = madgwick_.getYaw();
}