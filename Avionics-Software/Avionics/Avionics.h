#ifndef INCLUDE_AVIONICS_H
#define INCLUDE_AVIONICS_H

#include "../AvionicsCore/AvionicsCore.h"

class Avionics : public AvionicsCore
{
public:
  // whether to show debug
  virtual void setDebugMode(bool mode) override{}

  // initialize modules
  virtual bool initialize() override;

private:
  // time step etc...
  virtual void update() override;

  // end processing
  virtual void end() override;

  // is all modules available
  virtual bool isReady(bool showDetail = true) override;

  // get datas
  virtual void getDatas() override;

  // write datas
  virtual void writeDatas() override;

  // reboot
  virtual void reboot() override {};

  // close sdcard when landing
  virtual void closeSDCard() override{}

  // transmit
  virtual void transmit(const xString &str) override{}

  // transmit
  virtual xString received() override{return xString();}
};

#endif