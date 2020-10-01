#include "TimerWrapper.h"

void TimerWrapper::start()
{
    bootTime_ = Kernel::Clock::now();
    nowTime_ = bootTime_;
    preTime_ = nowTime_;
}

void TimerWrapper::update()
{
    preTime_ = nowTime_;

    nowTime_ = Kernel::Clock::now();
}

float TimerWrapper::now()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(nowTime_ - bootTime_).count() * 0.000001f;
}

float TimerWrapper::delta()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(nowTime_ - preTime_).count() * 0.000001f;
}