#include "../PlatformDefinition.h"

#ifdef PLATFORM_MBED
#include "AvionicsMbed/AvionicsMbed.h"
#endif

#ifdef PLATFORM_ARDUINO
#include "AvionicsArduino/AvionicsArduino.h"
#endif