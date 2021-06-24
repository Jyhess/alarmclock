#include "utils/time.h"

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
