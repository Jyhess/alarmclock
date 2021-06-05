#pragma once

#include "inputs/button.h"
#include "inputs/rtc.h"
#include "utils/time.h"

class Inputs
{
public:
    Inputs(int red_pin, int yellow_pin, int green_pin);

    void setup();
    void loop();

    bool red_has_been_pressed() const;
    bool yellow_has_been_pressed() const;
    bool green_has_been_pressed() const;

    inline long now_ms() const { return _now_ms; }

    inline const Time &get_time() const { return _time; }

private:
    long _now_ms;

    Button _red;
    Button _yellow;
    Button _green;

    Rtc _rtc;
    long _last_rtc_read;
    Time _time;
};
