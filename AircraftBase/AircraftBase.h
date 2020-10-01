#include "Datas.h"
#include "MadgwickAHRS.h"
#include <string>

class AircraftBase
{
  Madgwick madgwick;
  
  bool recording_ = false;
  bool imuFilter_ = true;

protected:
  enum class Scene
  {
    Waiting,
    ReadyToLaunch,
    InFlight,
    Landing
  };

  enum class Commands
  {
    None,
    ResetMbed,
    EscapePreparing,
    CheckSensors,
    ClosingServo
  };

  Datas datas;
  Scene scene = Scene::Waiting;

public:
  // main loop
  void begin();

protected:
  AircraftBase()
  {}

  // time step etc...
  virtual void update() = 0;

  // whether to show debug
  virtual void setDebugMode(bool mode) = 0;

  // initialize modules
  virtual bool initialize() = 0;

  // is all modules available
  virtual bool isReady(bool showDetail = true) = 0;

  // end processing
  virtual void end() = 0;

  // wait for preparing
  virtual void waiting() = 0;

  // wait for launch
  virtual void waitingLaunch() = 0;

  // in flight
  virtual void inFlight() = 0;

  // landing
  virtual void landing() = 0;

  // get datas
  virtual void getDatas() = 0;

  // write datas
  virtual void writeDatas() = 0;

  // on receive command
  virtual void onReceive() = 0;

  // check received command
  Commands checkCommand(const std::string &recv);

  // start recording datas
  void beginRecord() { recording_ = true; }

  // end recording datas
  void endRecord() { recording_ = false; }

  // madgwick filter
  void setIMUFilter(bool on) { imuFilter_ = on; }

private:
  void applyIMUFilter();
};