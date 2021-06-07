#include "utils/ms_diff.h"

unsigned long ms_diff(unsigned long start_time, unsigned long now)
{
    if(start_time > now)
    {
        // long overflow
        return 0xFFFF - start_time + now;
    }
    else
    {
        return now - start_time;
    }
}
