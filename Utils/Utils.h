#pragma once

#include "Vec3.h"

namespace Utils
{
  float calcAltitude(float pressure0, float pressure, float temperature);

  Vec3 dpsToAngularVel(const Vec3 &v);

}