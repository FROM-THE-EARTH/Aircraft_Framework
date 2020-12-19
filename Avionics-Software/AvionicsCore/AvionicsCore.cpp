/*
* This source is core systems of Avionics
* Do not write Platform-dependent programs (ex. sensors, timer, printf, ...)
* Platform-dependent programs are written in Avionics class
*/

#include "AvionicsCore.h"

void AvionicsCore::begin()
{
  while (true)
  {
    update();

    getDatas();

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

void AvionicsCore::waiting()
{
  if (elapsedTimer_.isElapsed(3.0f))
  {
    transmit("Waiting " + to_XString(flightData.time) + "s");
  }
}

void AvionicsCore::waitingLaunch()
{
  if (elapsedTimer_.isElapsed(3.0f))
  {
    transmit("Waiting launch");
  }

  if (actions.launch.isMet())
  {
    flightData.launchTime = flightData.time;
    sequence_ = Sequence::InFlight;
    transmit("Launch: Acc=" + to_XString(flightData.accel.length()));
  }
}

void AvionicsCore::inFlight()
{
  if (actions.detach.isMet())
  {
    flightData.detachTime = flightData.time;
    actions.detach.action();
    transmit("Detach");
  }

  if (actions.deceleration.isMet())
  {
    flightData.decelerationTime = flightData.time;
    actions.deceleration.action();
    transmit("Open parachute");
  }

  if (actions.landing.isMet())
  {
    flightData.landingTime = flightData.time;
    actions.landing.action();
    endRecord();
    closeSDCard();
    sequence_ = Sequence::Landing;
    transmit("Landing");
  }

  if (elapsedTimer_.isElapsed(1.0f))
  {
    transmit(to_XString(flightData.time - flightData.launchTime) + "s, " + to_XString(flightData.altitude) + "m");
  }
}

void AvionicsCore::landing()
{
  // transmit gps info
  if (elapsedTimer_.isElapsed(2.0f))
  {
    transmit(to_XString(flightData.latitude) + "N, " + to_XString(flightData.longitude) + "E");
  }
}

void AvionicsCore::onReceiveCommand()
{
  auto cmd = TelemetryJudgeCommand(received().c_str());

  switch (cmd)
  {
  case CmdReboot:
    transmit("Reboot");
    reboot();
    break;

  case CmdEscape:
    if (sequence_ == Sequence::Waiting)
    {
      transmit("Begin recording");
      beginRecord();
      actions.cameraOn.action();
      sequence_ = Sequence::ReadyToLaunch;
    }
    else
    {
      transmit("Cannot escape this sequence");
    }
    break;

  case CmdCheckSensors:
    isReady(true);
    break;

  case CmdCloseParachute:
    if (sequence_ == Sequence::Waiting)
    {
      actions.closeServo.action();
      transmit("Close parachute");
    }
    else
    {
      transmit("Cannot close parachute in this sequence");
    }
    break;

  case CmdOpenParachute:
    if (sequence_ == Sequence::Waiting)
    {
      actions.openServo.action();
      transmit("Open paracute");
    }
    else
    {
      transmit("Cannot open parachute in this sequence");
    }
    break;

  case CmdUnknown:
    transmit("Invalid command");
    break;
  }
}