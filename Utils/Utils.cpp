#pragma once

#include "Utils.h"
#include "MathConstants.h"

#include <cmath>

namespace Utils
{
  float calcAltitude(float pressure0, float pressure, float temperature)
  {
    return (std::pow(pressure0 / pressure, 0.1902225603956629f) - 1) *
           (temperature + 273.15) * 153.8461538461538;
  }
}