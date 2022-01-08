#include "js_time.h"

Time Time::from_minutes(uint16_t minutes)
{
    uint16_t hours = minutes / 60;
    return Time(hours % 24, minutes - hours * 60);
}

Time Time::from_seconds(uint32_t seconds)
{
    return Time::from_minutes(seconds / 60);
}

Time &Time::add_hour(int16_t hours)
{
    while (hours < 0)
        hours += 24;
    hours += _hour;
    _hour = hours % 24;
    return *this;
}

Time &Time::add_minute(int16_t minutes, bool propagate)
{
    int16_t hours = 0;
    while (minutes < 0)
    {
        minutes += 60;
        hours -= 1;
    }
    minutes += _minute;
    if (propagate)
    {
        hours += minutes / 60;
        add_hour(hours);
    }
    _minute = minutes % 60;
    return *this;
}

Time &Time::add_hm(int16_t hours, int16_t minutes, bool propagate)
{
    add_minute(minutes, propagate);
    return add_hour(hours);
}
