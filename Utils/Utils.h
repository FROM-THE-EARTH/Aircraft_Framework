#pragma once

namespace Utils
{
  float calcAltitude(float pressure0, float pressure, float temperature);

  namespace Math{
    float pow(float x, float y);
    float sqrt(float x);
  }
}