#include "Aircraft.h"

bool Aircraft::launchCondition(){
  return datas.accel.length() > launchThreshold_;
}

bool Aircraft::detachCondition() {
  return (datas.altitude > 500);
}

bool Aircraft::decelerationCondition() {
  return (datas.altitude < datas.maxAltitude - 10);
}

bool Aircraft::landingCondition(){
  return std::chrono::duration_cast<std::chrono::milliseconds>(
          timer_.elapsed_time())
          .count() > landingTime_;
}

void Aircraft::detachAircraft(){
  
}

void Aircraft::openParachute(){
  
}