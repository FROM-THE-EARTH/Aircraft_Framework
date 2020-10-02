#pragma once

#include "../Filter/MadgwickAHRS.h"
#include "../Type/Datas.h"
#include "../Type/XString.h"

class AircraftBase
{
  enum class Sequence
  {
    Waiting,
    ReadyToLaunch,
    InFlight,
    Landing
  };

  enum class Commands
  {
    None,
    Reboot,
    EscapePreparing,
    CheckSensors,
    ClosingServo
  };

  Madgwick madgwick;

  float preTime_ = 0.0f;

  bool recording_ = false;
  bool imuFilter_ = true;

  bool detached_ = false, decelerationStarted_ = false;

  Sequence sequence = Sequence::ReadyToLaunch;

protected:
  Datas datas;

public:
  // main loop
  void begin();

  // initialize modules
  virtual bool initialize() = 0;

  // whether to show debug
  virtual void setDebugMode(bool mode) = 0;

protected:
  AircraftBase()
  {
  }

  //--------------------------------------
  // Defines in Aircraft(Platform) class
  //--------------------------------------
  // time step etc...
  virtual void update() = 0;

  // end processing
  virtual void end() = 0;

  // is all modules available
  virtual bool isReady(bool showDetail = true) = 0;

  // reboot
  virtual void reboot() = 0;

  // get datas
  virtual void getDatas() = 0;

  // write datas
  virtual void writeDatas() = 0;

  // transmit
  virtual void transmit(const xString &str) = 0;

  // transmit
  virtual xString receive() = 0;

  //--------------------------------------
  // Defines in Aircraft class
  //--------------------------------------
  // set launch condition
  virtual bool launchCondition() = 0;

  // set detaching condition
  virtual bool detachCondition() = 0;

  // set opening parachute condition
  virtual bool decelerationCondition() = 0;

  // set landing condition
  virtual bool landingCondition() = 0;

  // detaching operation
  virtual void detachAircraft() = 0;

  // opening parachute operation
  virtual void openParachute() = 0;

  //--------------------------------------
  // Defines in this class
  //--------------------------------------
  // start recording datas
  void beginRecord() { recording_ = true; }

  // end recording datas
  void endRecord() { recording_ = false; }

  // madgwick filter
  void setIMUFilter(bool on) { imuFilter_ = on; }

  // on receive command
  void onReceiveCommand();

  bool isElapsed(float time)
  {
    if (preTime_ < (datas.time - time))
    {
      preTime_ = datas.time;
      return true;
    }
    return false;
  }

private:
  // wait for preparing
  void waiting();

  // wait for launch
  void waitingLaunch();

  // in flight
  void inFlight();

  // landing
  void landing();

  // check received command
  Commands checkCommand(const xString &recv);

  // imu filter
  void applyIMUFilter();
};