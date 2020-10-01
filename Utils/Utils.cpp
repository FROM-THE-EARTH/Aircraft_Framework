#include "Utils.h"
#include "../Constants/MathConstants.h"
#include "../PlatformDefinition.h"

#ifdef PLATFORM_MBED
#include <cmath>
#endif

namespace Utils
{
  namespace Math
  {
#ifdef PLATFORM_MBED
    float pow(float x, float y)
    {
      return std::pow(x, y);
    }

    float sqrt(float x)
    {
      return std::sqrt(x);
    }
#endif

#ifdef PLATFORM_ARDUINO
    float pow(float x, float y)
    {
      return pow(x, y);
    }

    float sqrt(float x)
    {
      return sqrt(x);
    }
#endif
  } // namespace math

  //independence
  float calcAltitude(float pressure0, float pressure, float temperature)
  {
    return (Math::pow(pressure0 / pressure, 0.1902225603956629f) - 1) *
           (temperature + 273.15) * 153.8461538461538;
  }
} // namespace utils