#include "AircraftWrapper.h"

#ifdef PLATFORM_ARDUINO

#include "Utils.h"
#include "Nature.h"

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

void AircraftWrapper::waiting()
{
}

void AircraftWrapper::waitingLaunch()
{
}

void AircraftWrapper::inFlight()
{
}

void AircraftWrapper::landing()
{
}

void AircraftWrapper::getDatas()
{
}

void AircraftWrapper::writeDatas()
{
}

void AircraftWrapper::onReceive()
{
}

#endif