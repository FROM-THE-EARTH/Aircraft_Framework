#include "Avionics.h"

bool Avionics::initialize()
{
  return isReady();
}

void Avionics::update()
{
}

bool Avionics::isReady(bool showDetail)
{
  bool allModulesAvailable = true;

  return allModulesAvailable;
}

void Avionics::end()
{
  // end processing
}

void Avionics::getDatas()
{
}

void Avionics::writeDatas()
{
}