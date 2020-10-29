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

  const bool hasGPS_;
  const bool imuFilter_;
  const bool useMagnInMadgwick_;

  bool recording_ = false;
  bool detached_ = false, decelerationStarted_ = false;

  Sequence sequence_ = Sequence::ReadyToLaunch;

protected:
  Datas datas;

  const xString csvHeader = "time,temperature,pressure,accX,accY,accZ,gyroX,gyroY,gyroZ,magnX,magnY,magnZ,longitude,latitude";

public:
  // main loop
  void begin();

  // initialize modules
  virtual bool initialize() = 0;

  // whether to show debug
  virtual void setDebugMode(bool mode) = 0;

  // return datas
  const Datas &data() const { return datas; }

  // conditions.
  bool (*Condition_Launch)();
  bool (*Condition_Detach)() = Function::Condition::None;
  bool (*Condition_Deceleration)();
  bool (*Condition_Landing)();

  // operations. called once
  void (*Operation_Detach)() = Function::Operation::None;
  void (*Operation_OpenParachute)();
  void (*Operation_CloseServo)();

protected:
  AvionicsBase(bool hasGPS, bool imuFilter, bool useMagnInMadgwick)
      : hasGPS_(hasGPS),
        imuFilter_(imuFilter),
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

  xString getCSVFormattedData() const
  {
    //const xString csvHeader = "time,temperature,pressure,accX,accY,accZ,gyroX,gyroY,gyroZ,magnX,magnY,magnZ,longitude,latitude";

    return (
        to_XString(datas.time) + to_XString(datas.temperature) + to_XString(datas.pressure)
        + to_XString(datas.accel.x) + to_XString(datas.accel.y) + to_XString(datas.accel.z)
        + to_XString(datas.gyro.x) + to_XString(datas.gyro.y) + to_XString(datas.gyro.z)
        + to_XString(datas.magn.x) + to_XString(datas.magn.y) + to_XString(datas.magn.z)
        + to_XString(datas.longitude) + to_XString(datas.latitude)
        );
  }

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