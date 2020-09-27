#include "AircraftBase.h"
#include "IM920Wrapper.h"
#include "LPSWrapper.h"
#include "LSMWrapper.h"
#include "mbed.h"

class AircraftMbedBase : public AircraftBase {
protected:
  Timer timer_;

  // modules
  IM920Wrapper *receiver_, *transmitter_;
  LPSWrapper *lps_;
  LSMWrapper *lsm_;

  Commands commands_ = Commands::None;

  AircraftMbedBase(IM920Wrapper *receiver, IM920Wrapper *transmitter,
                   LPSWrapper *lps, LSMWrapper *lsm)
      : AircraftBase(),
        receiver_(receiver), transmitter_(transmitter), lps_(lps), lsm_(lsm) {}

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

  virtual void setDebugMode(bool mode) override {
    transmitter_->setSendToSerial(mode);
    receiver_->setSendToSerial(mode);
  }

private:
  virtual bool isReady(bool showDetail = false) override;

  virtual void end() override;

  virtual void waiting() override;

  virtual void waitingLaunch() override;

  virtual void inFlight() override;

  virtual void landing() override;

  virtual void getDatas() override;

  virtual void writeDatas() override;

  virtual void onReceive() override;

  virtual Commands checkCommand(const std::string &recv) override;
};