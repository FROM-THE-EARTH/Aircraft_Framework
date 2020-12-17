#ifndef INCLUDE_ELAPSEDTIMER_H
#define INCLUDE_ELAPSEDTIMER_H

class ElapsedTimer
{
    float preTime_ = 0.0f;

public:
    // time[s]
    bool isElapsed(float time)
    {
        if (preTime_ < time)
        {
            preTime_ = time;
            return true;
        }
        return false;
    }
};

#endif