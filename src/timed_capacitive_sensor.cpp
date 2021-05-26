#include "timed_capacitive_sensor.h"

TimedCapacitiveSensor::TimedCapacitiveSensor(uint8_t sendPin, uint8_t receivePin)
    : _cs(sendPin, receivePin), _activation_time_ms(0), _last_total(0), _threshold(25)
{
}

void TimedCapacitiveSensor::setup()
{
    //_cs.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1 - just as an example
}

void TimedCapacitiveSensor::loop(long now_ms)
{
    _previous_total = _last_total;
    _last_total = _cs.capacitiveSensor(30);
    if (is_active() && ! was_active())
    {
        _activation_time_ms = now_ms;
    }
}
