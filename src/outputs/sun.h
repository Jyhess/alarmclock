#pragma once
#include "state.h"

class Sun
{
private:
    uint8_t _pin_low;
    uint8_t _pin_mid;
    uint8_t _pin_high;
    uint8_t _start;
    uint8_t _target;
    uint8_t _last;
    unsigned long _start_time;

public:
    Sun(uint8_t pin_low, uint8_t pin_mid, uint8_t pin_high);
    void setup();
    void loop(const State &state);

    inline bool is_on() const { return _target > 0; }

private:
    long _compute_current(unsigned long now_ms);
    uint8_t _compute_output(long current, long min, long max) const;
};
