#include "timed_capacitive_sensor.h"

TimedCapacitiveSensor::TimedCapacitiveSensor(uint8_t sendPin, uint8_t receivePin)
    : _cs(sendPin, receivePin), _activation_time_ms(0), _previous_value(0), _threshold(50), _increase_time(3000), _increasing(false), _percent(0)
{
}

void TimedCapacitiveSensor::setup()
{
    _cs.set_CS_AutocaL_Millis(10000);
}

void TimedCapacitiveSensor::_update_percent(long value, long now_ms)
{
    if (_increasing)
    {
        long elapsed_ms = now_ms - _activation_time_ms;
        if (elapsed_ms > _increase_time)
            _percent = 100;
        else
            _percent = elapsed_ms * 100 / _increase_time;
        Serial.printf("%i increasing => %i\n", value, _percent);
    }
    else
    {
        _percent = 0; //100 - (now_ms - _activation_time_ms) * 100 / _increase_time;
        Serial.printf("%i decreasing => %i\n", value, _percent);
    }
}

void TimedCapacitiveSensor::loop(long now_ms)
{
    long value = _cs.capacitiveSensor(30);

    if (_previous_value > 0)
    {
        if (value > 0 && _activation_time_ms > 0)
        {
            _update_percent(value, now_ms);
        }
        else
        {
            _activation_time_ms = 0;
            Serial.printf("%i stop touching => %i\n", value, _percent);
        }
    }
    else if (value > _threshold)
    {
        _increasing = !_increasing;
        _activation_time_ms = now_ms;
        Serial.printf("%i start touching => %i\n", value, _increasing);
        _update_percent(value, now_ms);
    }
    else
    {
        //Serial.printf("%d no touch\n", value);
        value = 0;
    }

    _previous_value = value;
}
