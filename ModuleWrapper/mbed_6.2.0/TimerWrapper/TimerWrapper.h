/*
This program is developed under the mbed-os 6.2.0
*/

#include "mbed.h"

class TimerWrapper
{
    Kernel::Clock::time_point bootTime_;
    Kernel::Clock::time_point nowTime_;
    Kernel::Clock::time_point preTime_;

public:
    void start();

    void update();

    float now();

    float delta();
};