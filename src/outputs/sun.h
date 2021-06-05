#pragma once
#include "state.h"

class Sun
{
private:
    int _pin;
    int _current;
    int _target;
    long _switch_duration;
    long _start_time;

public:
    Sun(int pin);
    void setup();
    void loop(const State &state);

    void switch_on(int percent);
    void switch_off(long now_ms);
    inline bool is_on() const { return _target > 0; }
};
