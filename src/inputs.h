#pragma once

#include "button.h"

class Inputs
{
public:
    Inputs(int red_pin, int yellow_pin, int green_pin);

    void loop(int now_ms);

    bool red_has_been_pressed() const;
    bool yellow_has_been_pressed() const;
    bool green_has_been_pressed() const;

    inline bool now_ms() const { return _now_ms; }

private:
    long _now_ms;
    Button _red;
    Button _yellow;
    Button _green;
};
