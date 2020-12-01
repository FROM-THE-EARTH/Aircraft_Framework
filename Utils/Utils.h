#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

#include "../Constants/MathConstants.h"

#define PLATFORM_MBED

namespace Utils
{
  namespace Math
  {
#ifdef PLATFORM_MBED
    float pow(float x, float y);

    float sqrt(float x);
#endif

#ifdef PLATFORM_ARDUINO
    float pow(float x, float y);

    float sqrt(float x);
#endif
  } // namespace Math

  float calcAltitude(float pressure0, float pressure, float temperature);
} // namespace Utils


#endif