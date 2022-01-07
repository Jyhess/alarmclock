#pragma once

#include "inputs/button.h"

class ButtonWithCount
{
public:
    enum CountMode
    {
        Falling,
        Rising,
        Both
    };

    explicit ButtonWithCount(uint8_t pin);
    void setup();
    void loop(unsigned long now_ms);

    inline void set_count_mode(CountMode mode) { _count_mode = mode; }
    inline unsigned long get_count() const { return _count; }
    inline void reset_count() { _count = 0; }

private:
    Button _button;
    unsigned long _count;
    CountMode _count_mode;
};
