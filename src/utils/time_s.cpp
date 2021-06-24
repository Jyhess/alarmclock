#include "utils/time_s.h"

TimeS &TimeS::add_hms(int16_t hours, int16_t minutes, int16_t seconds)
{
    while (seconds < 0)
    {
        seconds += 60;
        minutes -= 1;
    }
    seconds += _second;
    minutes += seconds / 60;
    _hm.add_hm(hours, minutes, true);
    _second = seconds % 60;
    return *this;
}
