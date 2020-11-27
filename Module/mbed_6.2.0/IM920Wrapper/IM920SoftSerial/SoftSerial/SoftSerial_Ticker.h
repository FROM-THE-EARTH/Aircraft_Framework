//A modified version of the regular ticker/timeout libraries to allow us to do timeout without losing accuracy
 
#ifndef FLEXTICKER_H
#define FLEXTICKER_H
 
#include "mbed.h"
 
class FlexTicker: public Ticker {
    public:

    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void)) {
        _function = callback(tptr, mptr);
    }
 
    /** Detach the function
     */
    void detach() {
        remove();
    }
    
    void setNext(int delay) {
        insert(event.timestamp + delay);
    }
    
    void prime(void) {
        event.timestamp = us_ticker_read();
    }
 
protected:
    virtual void handler() {
        _function.call();
    }
 
    unsigned int _delay;
    Callback<void()> _function;
};
 
#endif
