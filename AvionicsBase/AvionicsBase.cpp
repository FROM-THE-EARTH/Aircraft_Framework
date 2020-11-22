#include "AvionicsBase.h"
#include "../Utils/Utils.h"

void AvionicsBase::begin()
{
  if (!checkFunctions())
  {
    transmit("[WARNING] Some function is undefined");
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

bool AvionicsBase::checkFunctions()
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

void AvionicsBase::waiting()
{
  if (isElapsed(3.0f))
  {
    transmit("Waiting " + to_XString(datas.time) + "s");
  }
}

void AvionicsBase::waitingLaunch()
{
  if (isElapsed(3.0f))
  {
    transmit("Waiting launch");
  }

  if (Condition_Launch())
  {
    datas.launchTime = datas.time;
    sequence_ = Sequence::InFlight;
    transmit("Launch: Acc=" + to_XString(datas.accel.length()));
  }
}

void AvionicsBase::inFlight()
{
  if (!detached_ && Condition_Detach())
  {
    datas.detachTime = datas.time;
    Operation_Detach();
    detached_ = true;
    transmit("Detach");
  }

  if (!decelerationStarted_ && Condition_Deceleration())
  {
    datas.decelerationTime = datas.time;
    Operation_OpenParachute();
    decelerationStarted_ = true;
    transmit("Open parachute");
  }

  if (decelerationStarted_ && Condition_Landing())
  {
    datas.landingTime = datas.time;
    Operation_CameraOff();
    endRecord();
    closeSDCard();
    sequence_ = Sequence::Landing;
    transmit("Landing");
  }

  if (isElapsed(1.0f))
  {
    transmit(to_XString(datas.time - datas.launchTime) + "s, " + to_XString(datas.altitude) + "m");
  }
}

void AvionicsBase::landing()
{
  // transmit gps info
  if (hasGPS_ && isElapsed(2.0f))
  {
    transmit(to_XString(datas.latitude) + "N, " + to_XString(datas.longitude) + "E");
  }
}

AvionicsBase::Commands AvionicsBase::checkCommand(const xString &recv)
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
    return Commands::CloseServo;
  }

  if (recv == "openpara")
  {
    return Commands::OpenParachute;
  }

  return Commands::None;
}

void AvionicsBase::onReceiveCommand()
{
  switch (checkCommand(received()))
  {
  case Commands::Reboot:
    transmit("Reboot");
    reboot();
    break;

  case Commands::EscapePreparing:
    if (sequence_ == Sequence::Waiting)
    {
      transmit("Begin recording");
      beginRecord();
      Operation_CameraOn();
      sequence_ = Sequence::ReadyToLaunch;
    }
    else
    {
      transmit("Cannot escape this sequence");
    }
    break;

  case Commands::CheckSensors:
    isReady(true);
    break;

  case Commands::CloseServo:
    if (sequence_ == Sequence::Waiting)
    {
      Operation_CloseServo();
      transmit("Close servo");
    }
    else
    {
      transmit("Cannot close servo in this sequence");
    }
    break;

  case Commands::OpenParachute:
    if (sequence_ == Sequence::Waiting)
    {
      Operation_OpenParachute();
      transmit("Open paracute");
    }
    else
    {
      transmit("Cannot open parachute in this sequence");
    }
    break;

  case Commands::None:
    transmit("Invalid command");
    break;
  }
}

void AvionicsBase::applyIMUFilter()
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