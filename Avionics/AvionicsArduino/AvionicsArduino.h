#pragma once

#include "../../AvionicsBase/AvionicsBase.h"

class Avionics : public AvionicsBase
{
public:
  Avionics(bool hasFlightPin, bool imuFilter = true, bool useMagnInMadgwick = false)
      : AvionicsBase(hasFlightPin, imuFilter, useMagnInMadgwick)
  {
  }

  virtual bool initialize() override;

  virtual void setDebugMode(bool mode) override
  {
  }

  virtual bool flightpin() override
  {
    return hasFlightPin;
  }

private:
  virtual void update() override;

  virtual void end() override;

  virtual void reboot() override
  {
  }

  virtual bool isReady(bool showDetail = false) override;

  virtual void getDatas() override;

  virtual void writeDatas() override;

  virtual void transmit(const xString& str) override{
  };

  virtual xString receive() override{
    return "";
  };

  void onReceive(){
    onReceiveCommand();
  }
};