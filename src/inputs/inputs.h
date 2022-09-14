#pragma once

#include "js_time.h"
#include "inputs/button_capacitive.h"
#include "inputs/rtc.h"
#include "inputs/luminosity.h"
#include "inputs/vibration.h"

class Inputs
{
public:
    Inputs(uint8_t send_pin, uint8_t middle_pin, uint8_t left_pin, uint8_t right_pin, uint8_t luminosity_pin, uint8_t vibration_pin);

    void setup();
    void loop();

    inline Rtc &rtc() { return _rtc; }

    bool middle_has_been_pressed(unsigned long threshold = 200) const;
    bool left_has_been_pressed(unsigned long threshold = 200) const;
    bool right_has_been_pressed(unsigned long threshold = 200) const;
    unsigned long middle_long_pressed(unsigned long threshold) const;
    unsigned long left_long_pressed(unsigned long threshold) const;
    unsigned long right_long_pressed(unsigned long threshold) const;

    int get_luminosity() const { return _luminosity.get_value(); }
    bool is_dark() const { return _luminosity.is_dark(); }
    uint8_t get_vibration() const { return _vibration.get_value(); }
    bool has_moved() const { return _vibration.has_moved(); }

    inline unsigned long now_ms() const { return _now_ms; }

    inline const TimeS &get_time() const { return _time; }

private:
    unsigned long _now_ms;

    ButtonCapacitive _middle;
    ButtonCapacitive _left;
    ButtonCapacitive _right;
    Luminosity _luminosity;
    Vibration _vibration;

    Rtc _rtc;
    unsigned long _last_rtc_read;
    TimeS _time;

    void _read_rtc();
};
