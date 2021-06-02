#pragma once

#include "time.h"

class Outputs
{
private:
    bool _updated;

    int _display_brightness;
    Time _time;
    Time _alarm_1;
    Time _alarm_2;

public:
    inline Outputs() : _updated(false), _display_brightness(0)
    {
    }

    inline bool is_updated() const { return _updated; }
    void clear_updated() { _updated = false; }

    inline int get_display_brightness() const { return _display_brightness; }
    inline void set_display_brightness(int display_brightness)
    {
        _display_brightness = display_brightness;
        _updated = true;
    }

    inline const Time &get_current_time() const { return _time; }
    inline void set_current_time(const Time &time)
    {
        _time = time;
        _updated = true;
    }

    inline const Time &get_alarm_1() const { return _alarm_1; }
    inline void set_alarm_1(const Time &time)
    {
        _alarm_1 = time;
        _updated = true;
    }
    inline const Time &get_alarm_2() const { return _alarm_2; }
    inline void set_alarm_2(const Time &time)
    {
        _alarm_2 = time;
        _updated = true;
    }
};