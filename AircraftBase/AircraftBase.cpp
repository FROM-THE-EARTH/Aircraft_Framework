#include "AircraftBase.h"

void AircraftBase::begin()
{
  while (true)
  {
    getDatas();

    if (recording)
      writeDatas();

    switch (scene)
    {
    case Scene::Waiting:
      waiting();
      break;

    case Scene::ReadyToLaunch:
      waitingLaunch();
      break;

    case Scene::InFlight:
      inFlight();
      break;

    case Scene::Landing:
      landing();
      break;
    }
  }
}