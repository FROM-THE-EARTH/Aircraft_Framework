#pragma once

#include "../../AircraftBase/AircraftBase.h"

class AircraftWrapper : public AircraftBase
{
protected:
  Commands commands_ = Commands::None;

  AircraftWrapper()
      : AircraftBase()
  {
  }

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

public:
  virtual bool initialize() override;

  virtual void setDebugMode(bool mode) override
  {
  }

private:
  virtual void update() override;

  virtual void waiting() override;

  virtual void waitingLaunch() override;

  virtual void inFlight() override;

  virtual void landing() override;

  virtual void end() override;

  virtual void reboot() override
  {
  }

  virtual bool isReady(bool showDetail = false) override;

  virtual void getDatas() override;

  virtual void writeDatas() override;

  virtual void onReceive() override;
};