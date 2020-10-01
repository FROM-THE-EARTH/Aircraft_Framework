#define PLATFORM_MBED
//#define PLATFORM_ARDUINO

#ifdef PLATFORM_MBED
#include "AircraftMbed/AircraftMbed.h"
#endif

#ifdef PLATFORM_ARDUINO
#include "AircraftArduino/AircraftArduino.h"
#endif