#include "../PlatformDefinition.h"

#ifdef PLATFORM_MBED
#include "AircraftMbed/AircraftMbed.h"
#endif

#ifdef PLATFORM_ARDUINO
#include "AircraftArduino/AircraftArduino.h"
#endif