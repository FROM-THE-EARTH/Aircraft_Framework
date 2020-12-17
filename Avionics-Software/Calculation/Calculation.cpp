#include "Calculation.h"
#include <math.h>

namespace Calc
{
    float altitude(float pressure0, float pressure, float temperature)
    {
        return (pow(pressure0 / pressure, 0.1902225603956629f) - 1) *
               (temperature + 273.15) * 153.8461538461538;
    }
} // namespace Calc