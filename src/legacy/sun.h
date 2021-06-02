#pragma once
#include <stdint.h>

class Sun
{
private:
    uint8_t _pin;
    int _current;
    int _target;
    long _switch_duration;
    long _start_time;

public:
    Sun(uint8_t pin);
    void setup();
    void loop(long now_ms);

    void switch_on(int percent);
    void switch_off(long now_ms);
    inline bool is_on() const { return _target > 0; }
};
