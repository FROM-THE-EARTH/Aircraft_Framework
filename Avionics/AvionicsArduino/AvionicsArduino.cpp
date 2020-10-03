#include "../Avionics.h"

#ifdef PLATFORM_ARDUINO

#include "../../Utils/Utils.h"
#include "../../Constants/Nature.h"

bool Avionics::initialize()
{
  return isReady();
}

void Avionics::update()
{
}

bool Avionics::isReady(bool showDetail)
{
  return false;
}

void Avionics::end()
{
}

void Avionics::getDatas()
{
}

void Avionics::writeDatas()
{
}

#endif