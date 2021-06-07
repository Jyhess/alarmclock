#pragma once
#include "state.h"

class Sun
{
private:
    int _pin;
    int _start;
    int _target;
    int _last;
    unsigned long _start_time;

public:
    Sun(int pin);
    void setup();
    void loop(const State &state);

    inline bool is_on() const { return _target > 0; }
};
