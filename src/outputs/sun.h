#pragma once
#include "state.h"

class Sun
{
private:
    uint8_t _pin;
    int8_t _start;
    int8_t _target;
    int8_t _last;
    unsigned long _start_time;

public:
    Sun(uint8_t pin);
    void setup();
    void loop(const State &state);

    inline bool is_on() const { return _target > 0; }
};
