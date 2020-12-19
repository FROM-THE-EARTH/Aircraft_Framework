#ifndef INCLUDE_FLIGHTACTION_H
#define INCLUDE_FLIGHTACTION_H

class FlightAction
{
    bool worked_ = false;

public:
    bool (*conditions)() = []() { return false; };
    void (*action)() = []() {};

    bool isMet()
    {
        if (!worked_ && conditions())
        {
            worked_ = true;
            return true;
        }
        return false;
    }

    bool isWorked()const{
        return worked_;
    }
};

#endif