#pragma once

#include "utils/time.h"
#include "inputs/button_capacitive.h"
#include "inputs/rtc.h"
#include "inputs/luminosity.h"

class Inputs
{
public:
    Inputs(uint8_t red_pin, uint8_t yellow_pin, uint8_t green_pin, uint8_t luminosity_pin);

    void setup();
    void loop();

    inline Rtc &rtc() { return _rtc; }

    bool red_has_been_pressed() const;
    bool yellow_has_been_pressed() const;
    bool green_has_been_pressed() const;
    unsigned long red_long_pressed(unsigned long threshold) const;
    unsigned long yellow_long_pressed(unsigned long threshold) const;
    unsigned long green_long_pressed(unsigned long threshold) const;

    int get_luminosity() const { return _luminosity.get_value(); }
    bool is_dark() const { return _luminosity.is_dark(); }

    inline unsigned long now_ms() const { return _now_ms; }

    inline const Time &get_time() const { return _time; }

private:
    unsigned long _now_ms;

    ButtonCapacitive _red;
    ButtonCapacitive _yellow;
    ButtonCapacitive _green;
    Luminosity _luminosity;

    Rtc _rtc;
    unsigned long _last_rtc_read;
    Time _time;

    void _read_rtc();
};
