#pragma once
#include <CapacitiveSensor.h>

class TimedCapacitiveSensor
{
private:
    CapacitiveSensor _cs;
    long _activation_time_ms;
    long _previous_value;
    long _threshold;
    long _increase_time;
    bool _increasing;
    long _percent;

    void _update_percent(long value, long now_ms);

public:
    TimedCapacitiveSensor(uint8_t sendPin, uint8_t receivePin);
    void setup();
    void loop(long now_ms);

    inline bool increasing() const { return _increasing && _activation_time_ms > 0; }
    inline bool decreasing() const { return !_increasing && _activation_time_ms > 0; }
    inline int percent() const { return _percent; }
};
