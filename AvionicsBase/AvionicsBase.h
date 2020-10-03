#pragma once

#include "../Filter/MadgwickAHRS.h"
#include "../Type/Datas.h"
#include "../Type/XString.h"

namespace Function
{
  namespace Condition
  {
    static bool None() { return false; }
  } // namespace Condition
  namespace Operation
  {
    static void None() {}
  } // namespace Operation
} // namespace Function

class AvionicsBase
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

  Madgwick madgwick_;

  float preTime_ = 0.0f;

  const bool imuFilter_;
  const bool useMagnInMadgwick_;

  bool recording_ = false;

  bool detached_ = false, decelerationStarted_ = false;

  Sequence sequence_ = Sequence::ReadyToLaunch;

protected:
  Datas datas;

public:
  // main loop
  void begin();

  // initialize modules
  virtual bool initialize() = 0;

  // whether to show debug
  virtual void setDebugMode(bool mode) = 0;

  // return datas
  const Datas &data() const { return datas; }

  // conditions
  bool (*Condition_Launch)();
  bool (*Condition_Detach)() = Function::Condition::None;
  bool (*Condition_Deceleration)();
  bool (*Condition_Landing)();

  // operations
  void (*Operation_Detach)() = Function::Operation::None;
  void (*Operation_OpenParachute)();

protected:
  AvionicsBase(bool imuFilter, bool useMagnInMadgwick)
      : imuFilter_(imuFilter),
        useMagnInMadgwick_(useMagnInMadgwick)
  {
  }

  //--------------------------------------
  // Defines in Avionics(Platform) class
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
  // Defines in this class
  //--------------------------------------
  // start recording datas
  void beginRecord() { recording_ = true; }

  // end recording datas
  void endRecord() { recording_ = false; }

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
  bool checkFunctions();

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