#include "utils/ms_diff.h"

unsigned long ms_diff(long ms_1, long ms_2)
{
    if (ms_1 < 0 && ms_2 >= 0)
    {
        unsigned long _ms_1 = *reinterpret_cast<unsigned long *>(&ms_1);
        return static_cast<unsigned long>(ms_2) - _ms_1;
    }
    else if (ms_1 >= 0 && ms_2 < 0)
    {
        unsigned long _ms_2 = *reinterpret_cast<unsigned long *>(&ms_2);
        return _ms_2 - static_cast<unsigned long>(ms_1);
    }
    else
    {
        return static_cast<unsigned long>(ms_2 - ms_1);
    }
}
