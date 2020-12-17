#include "XString.h"

//-------------------------------------------------------------
// For platform mbed
//-------------------------------------------------------------
#ifdef PLATFORM_MBED
std::string to_XString(int x)
{
    return std::to_string(x);
}
std::string to_XString(float x)
{
    return std::to_string(x);
}
#endif


//-------------------------------------------------------------
// For platform arduino
//-------------------------------------------------------------
#ifdef PLATFORM_ARDUINO
String to_XString()
{
    return String();
}
String to_XString(int x)
{
    return String(x);
}
String to_XString(float x)
{
    return String(x);
}
#endif
