#pragma once

#include "../../AircraftBase/AircraftBase.h"

class AircraftWrapper : public AircraftBase
{
protected:
  AircraftWrapper()
      : AircraftBase()
  {
  }

public:
  virtual bool initialize() override;

  virtual void setDebugMode(bool mode) override
  {
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