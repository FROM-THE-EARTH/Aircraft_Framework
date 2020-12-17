#ifndef INCLUDE_AVIONICS_H
#define INCLUDE_AVIONICS_H

#include "../Type/Datas.h"
#include "../Type/XString.h"
#include "../Type/ElapsedTimer.h"
#include "../Filter/MadgwickAHRS.h"
#include "../telemetry-protocol/telemetry.h"

namespace Function
{
  namespace Condition
  {
    static bool None() { return false; }
    static bool Always() { return true; }
  } // namespace Condition
  namespace Operation
  {
    static void None() {}
  } // namespace Operation
} // namespace Function

class Avionics
{
  enum class Sequence
  {
    Waiting,
    ReadyToLaunch,
    InFlight,
    Landing
  };

  const bool hasGPS_;
  const bool imuFilter_;
  const bool useMagnInMadgwick_;

  const xString csvHeader = "time,temperature,pressure,accX,accY,accZ,gyroX,gyroY,gyroZ,magnX,magnY,magnZ,largeAccX,largeAccY,largeAccZ,longitude,latitude,roll,pitch,yaw";

  Datas datas;
  Madgwick madgwick_;
  ElapsedTimer elapsedTimer_;
  Sequence sequence_ = Sequence::Waiting;

  bool recording_ = false;
  bool detached_ = false, decelerationStarted_ = false;

  float basePressure = 1020.2f;

public:
  //--------------------------------------
  // Core
  //--------------------------------------
  // main loop
  void begin();

  // initialize modules
  bool initialize();

  // whether to show debug
  void setDebugMode(bool mode);

  // return datas
  const Datas &data() const { return datas; }

  // conditions.
  bool (*Condition_Launch)();
  bool (*Condition_Detach)() = Function::Condition::None;
  bool (*Condition_Deceleration)();
  bool (*Condition_Landing)();

  // operations. called once

  // called when condition is true
  void (*Operation_Detach)() = Function::Operation::None;

  // called when condition is true
  void (*Operation_OpenParachute)();

  // called when command "svclose" received
  void (*Operation_CloseServo)();

  // called when command "escape" received
  void (*Operation_CameraOn)() = Function::Operation::None;

  // called when landing
  void (*Operation_CameraOff)() = Function::Operation::None;

  Avionics(bool hasGPS, bool imuFilter = true, bool useMagnInMadgwick = false)
      : hasGPS_(hasGPS),
        imuFilter_(imuFilter),
        useMagnInMadgwick_(useMagnInMadgwick)
  {
  }

private:

  //--------------------------------------
  // Platform-dependent
  //--------------------------------------
  // time step etc...
  void update();

  // end processing
  void end();

  // is all modules available
  bool isReady(bool showDetail = true);

  // reboot
  void reboot();

  // get datas
  void getDatas();

  // write datas
  void writeDatas();

  // close sdcard when landing
  void closeSDCard();

  // transmit
  void transmit(const xString &str);

  // transmit
  xString received();

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
    //csvHeader =
    //time,temperature,pressure,accX,accY,accZ,gyroX,gyroY,gyroZ,magnX,magnY,magnZ,largeAccX,largeAccY,largeAccZ,longitude,latitude,roll,pitch,yaw

    return (
        to_XString(datas.time) + "," + to_XString(datas.temperature) + "," + to_XString(datas.pressure) + "," + to_XString(datas.accel.x) + "," + to_XString(datas.accel.y) + "," + to_XString(datas.accel.z) + "," + to_XString(datas.gyro.x) + "," + to_XString(datas.gyro.y) + "," + to_XString(datas.gyro.z) + "," + to_XString(datas.magn.x) + "," + to_XString(datas.magn.y) + "," + to_XString(datas.magn.z) + "," + to_XString(datas.largeAcc.x) + "," + to_XString(datas.largeAcc.y) + "," + to_XString(datas.largeAcc.z) + "," + to_XString(datas.longitude) + "," + to_XString(datas.latitude) + "," + to_XString(datas.roll) + "," + to_XString(datas.pitch) + "," + to_XString(datas.yaw));
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

  // imu filter
  void applyIMUFilter();
};

#endif