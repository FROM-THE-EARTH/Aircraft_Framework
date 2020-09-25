#include "AircraftBase.h"
#include "IM920Wrapper.h"
#include "LPSWrapper.h"
#include "LSMWrapper.h"

#define MBED_NO_GLOBAL_USING_DIRECTIVE
#include "mbed.h"

class AircraftMbedBase : public AircraftBase {
protected:
  Timer timer_;

  // modules
  BufferedSerial pc_;
  IM920Wrapper *receiver_, *transmitter_;
  LPSWrapper *lps_;
  LSMWrapper *lsm_;

  Commands commands_ = Commands::None;

  AircraftMbedBase(float launchThreshold, float landingTime, float basePressure,
                   IM920Wrapper *receiver, IM920Wrapper *transmitter,
                   LPSWrapper *lps, LSMWrapper *lsm)
      : AircraftBase(launchThreshold, landingTime, basePressure),
        pc_(USBTX, USBRX), receiver_(receiver), transmitter_(transmitter),
        lps_(lps), lsm_(lsm) {}

  virtual bool detachFlag() = 0;
  virtual bool decelerationFlag() = 0;

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

  virtual bool detachAircraft() override { return detachFlag(); }
  virtual bool openParachute() override { return decelerationFlag(); }

  virtual void inFlight() override;

  virtual void landing() override;

  virtual void getDatas() override;

  virtual void writeDatas() override;

  virtual void onReceive() override;

  virtual Commands checkCommand(std::string recv) override;
};