#include "Aircraft.h"

bool Aircraft::detachFlag() {
  return (datas.altitude > 500);
  
  //if single aircraft
  //return false;
}

bool Aircraft::decelerationFlag() {
  return (datas.altitude < datas.maxAltitude - 10);
}