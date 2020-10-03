#include "../Aircraft.h"

#ifdef PLATFORM_ARDUINO

#include "../../Utils/Utils.h"
#include "../../Constants/Nature.h"

bool Aircraft::initialize()
{
  return isReady();
}

void Aircraft::update()
{
}

bool Aircraft::isReady(bool showDetail)
{
  return false;
}

void Aircraft::end()
{
}

void Aircraft::getDatas()
{
}

void Aircraft::writeDatas()
{
}

#endif