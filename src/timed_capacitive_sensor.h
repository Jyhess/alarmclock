#pragma once
#include <CapacitiveSensor.h>

class TimedCapacitiveSensor
{
private:
    CapacitiveSensor _cs;
    long _activation_time_ms;
    long _last_total;
    long _previous_total;
    long _threshold;

public:
    TimedCapacitiveSensor(uint8_t sendPin, uint8_t receivePin);
    void setup();
    void loop(long now_ms);
    inline bool is_active() const
    {
        return _last_total > _threshold;
    }
    inline bool was_active() const
    {
        return _previous_total > _threshold;
    }
    inline long activation_duration(long now_ms) const
    {
        if (is_active())
        {
            return now_ms - _activation_time_ms;
        }
        return 0;
    }
    inline long total() const
    {
        return _last_total;
    }
};
