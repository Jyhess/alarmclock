#include "utils/time.h"

Time Time::from_seconds(uint16_t seconds)
{
    uint16_t hours = seconds / 3600;
    uint8_t minutes = (seconds - hours * 3600) / 60;
    return Time(hours % 60, minutes);
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
