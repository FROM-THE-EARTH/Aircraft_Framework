#pragma once

namespace Utils
{
  namespace Math
  {
    float pow(float x, float y);
    float sqrt(float x);
  } // namespace Math

  float calcAltitude(float pressure0, float pressure, float temperature);
} // namespace Utils