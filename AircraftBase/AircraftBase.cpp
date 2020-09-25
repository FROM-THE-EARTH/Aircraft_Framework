#include "AircraftBase.h"

void AircraftBase::begin() {
  while (1) {
    getDatas();
    
    switch (scene) {
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