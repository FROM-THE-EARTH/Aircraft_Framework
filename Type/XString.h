#pragma once

#include "../PlatformDefinition.h"

#ifdef PLATFORM_MBED
#include <string>
using xString = std::string;
std::string to_XString(int x);
std::string to_XString(float x);
#endif

#ifdef PLATFORM_ARDUINO
#include <Arduino.h>
using xString = String;
String to_XString();
String to_XString(int x);
String to_XString(float x);
#endif