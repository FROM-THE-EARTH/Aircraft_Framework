#include "../AircraftWrapper.h"

#ifdef PLATFORM_ARDUINO

#include "../../Utils/Utils.h"
#include "../../Constants/Nature.h"

bool AircraftWrapper::initialize()
{
  return isReady();
}

void AircraftWrapper::update()
{
}

bool AircraftWrapper::isReady(bool showDetail)
{
  return false;
}

void AircraftWrapper::end()
{
}

void AircraftWrapper::getDatas()
{
}

void AircraftWrapper::writeDatas()
{
}

#endif