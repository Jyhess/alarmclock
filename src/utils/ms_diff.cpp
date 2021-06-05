#include "utils/ms_diff.h"

unsigned long ms_diff(long start_time, long now)
{
    if (start_time < 0 && now >= 0)
    {
        unsigned long _ms_1 = *reinterpret_cast<unsigned long *>(&start_time);
        return static_cast<unsigned long>(now) - _ms_1;
    }
    else if (start_time >= 0 && now < 0)
    {
        unsigned long _ms_2 = *reinterpret_cast<unsigned long *>(&now);
        return _ms_2 - static_cast<unsigned long>(start_time);
    }
    else
    {
        return static_cast<unsigned long>(now - start_time);
    }
}
