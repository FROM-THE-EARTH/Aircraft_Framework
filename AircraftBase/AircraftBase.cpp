#include "AircraftBase.h"
#include "Utils.h"

void AircraftBase::begin()
{
  while (true)
  {
    update();

    getDatas();

    updateQuaternion();

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

void AircraftBase::updateQuaternion()
{
  datas.quaternion += datas.quaternion.angularVelocityApplied(Utils::dpsToAngularVel(datas.gyro)) * datas.deltaTime;
  datas.quaternion = datas.quaternion.normalized();
}