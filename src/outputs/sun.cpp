#include <Arduino.h>
#include "outputs/sun.h"
#include "utils/ms_diff.h"

Sun::Sun(int pin) : _pin(pin), _current(0), _target(0), _switch_duration(3000), _start_time(0)
{
}

void Sun::setup()
{
    pinMode(_pin, OUTPUT);
    analogWrite(_pin, 0);
}

void Sun::loop(const State &state)
{
    if (_current != _target)
    {
        long elapsed_ms = ms_diff(_start_time, state.now_ms());
        long change = 255 * elapsed_ms / _switch_duration;
        long current = 0;
        if (_current > _target)
        {
            current = _current - change;
            if (current <= _target)
            {
                current = _target;
                _current = _target;
            }
        }
        else
        {
            current = _current + change;
            if (current > _target)
            {
                current = _target;
                _current = _target;
            }
        }
        //Serial.printf("Sun switching %d => %d => %d\n", _current, current, _target);
        analogWrite(_pin, current);
    }
}

void Sun::switch_on(int percent)
{
    if (percent == _current)
        return;
    //Serial.printf("Sun switch on %d\n", percent);
    _target = percent * 255 / 100;
    _current = _target;
    analogWrite(_pin, _current);
}

void Sun::switch_off(long now_ms)
{
    if (_target == 0)
        return;
    //Serial.printf("Sun switch off\n");
    _target = 0;
    _start_time = now_ms;
}
