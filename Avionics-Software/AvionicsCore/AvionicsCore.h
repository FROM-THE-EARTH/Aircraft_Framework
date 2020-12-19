#ifndef INCLUDE_AVIONICSCORE_H
#define INCLUDE_AVIONICSCORE_H

#include "../Type/FlightData.h"
#include "../Type/XString.h"
#include "../Type/ElapsedTimer.h"
#include "../telemetry-protocol/telemetry.h"
#include "../FlightAction/FlightAction.h"

class AvionicsCore
{
  enum class Sequence
  {
    Waiting,
    ReadyToLaunch,
    InFlight,
    Landing
  };
  Sequence sequence_ = Sequence::Waiting;

  const xString csvHeader = "time,temperature,pressure,accX,accY,accZ,gyroX,gyroY,gyroZ,magnX,magnY,magnZ,largeAccX,largeAccY,largeAccZ,longitude,latitude";

  ElapsedTimer elapsedTimer_;

  bool recording_ = false;

  float basePressure = 1020.2f;

public:
  struct Actions
  {
    FlightAction launch;
    FlightAction detach;
    FlightAction deceleration;
    FlightAction landing;
    FlightAction closeServo;
    FlightAction openServo;
    FlightAction cameraOn;
    FlightAction cameraOff;
  };
  Actions actions;
  FlightData flightData;

  //----------------------------------------------------------------------------
  // Core System
  //----------------------------------------------------------------------------
public:
  // main loop
  void begin();

  // whether to show debug
  virtual void setDebugMode(bool mode) = 0;

private:
  // wait for preparing
  void waiting();

  // wait for launch
  void waitingLaunch();

  // in flight
  void inFlight();

  // landing
  void landing();

  // start recording datas
  void beginRecord() { recording_ = true; }

  // end recording datas
  void endRecord() { recording_ = false; }

  // on receive command
  void onReceiveCommand();

  xString getCSVFormattedData() const
  {
    //csvHeader =
    //time,temperature,pressure,accX,accY,accZ,gyroX,gyroY,gyroZ,magnX,magnY,magnZ,largeAccX,largeAccY,largeAccZ,longitude,latitude

    xString d = to_XString(flightData.time) + ",";
    d += to_XString(flightData.temperature) + "," + to_XString(flightData.pressure) + ",";
    d += to_XString(flightData.accel.x) + "," + to_XString(flightData.accel.y) + "," + to_XString(flightData.accel.z) + ",";
    d += to_XString(flightData.gyro.x) + "," + to_XString(flightData.gyro.y) + "," + to_XString(flightData.gyro.z) + ",";
    d += to_XString(flightData.magn.x) + "," + to_XString(flightData.magn.y) + "," + to_XString(flightData.magn.z) + ",";
    d += to_XString(flightData.largeAcc.x) + "," + to_XString(flightData.largeAcc.y) + "," + to_XString(flightData.largeAcc.z) + ",";
    d += to_XString(flightData.longitude) + "," + to_XString(flightData.latitude) + ",";

    return d;
  }

  //----------------------------------------------------------------------------
  // Platform-dependent
  //----------------------------------------------------------------------------
public:
  // initialize modules
  virtual bool initialize() = 0;

protected:
  // time step etc...
  virtual void update() = 0;

  // end processing
  virtual void end() = 0;

  // is all modules available
  virtual bool isReady(bool showDetail = true) = 0;

  // get datas
  virtual void getDatas() = 0;

  // write datas
  virtual void writeDatas() = 0;

  // reboot
  virtual void reboot() = 0;

  // close sdcard when landing
  virtual void closeSDCard() = 0;

  // transmit
  virtual void transmit(const xString &str) = 0;

  // transmit
  virtual xString received() = 0;
};

#endif