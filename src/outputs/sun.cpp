#include <Arduino.h>
#include "outputs/sun.h"
#include "utils/ms_diff.h"

namespace
{
    int switch_duration = 3000;
}

Sun::Sun(int pin) : _pin(pin), _start(0), _target(0), _last(0), _start_time(0)
{
}

void Sun::setup()
{
    pinMode(_pin, OUTPUT);
    analogWrite(_pin, 0);
}

void Sun::loop(const State &state)
{
    int target = state.get_sun_percent();
    if (target != _target)
    {
        // Reset timer only if not already changing
        // TODO: Manage if target change direction
        if (_start == _target)
            _start_time = state.now_ms();
        _target = target;
    }
    if (_start != _target)
    {
        long elapsed_ms = ms_diff(_start_time, state.now_ms());
        long change = 100 * elapsed_ms / switch_duration;
        long current = 0;
        if (_start > _target)
        {
            current = _start - change;
            if (current <= _target)
            {
                current = _target;
                _start = _target;
            }
        }
        else
        {
            current = _start + change;
            if (current > _target)
            {
                current = _target;
                _start = _target;
            }
        }
        if (current != _last)
        {
            Serial.printf("Sun switching %d", _start);
            Serial.printf(" => %d", current);
            _last = current;
            current = current * current * 255 / (100 * 100);
            Serial.printf(" => %d", current);
            Serial.printf(" => %d\n", _target);
            analogWrite(_pin, current);
        }
    }
}

void Sun::switch_on(int percent)
{
    if (percent == _start)
        return;
    //Serial.printf("Sun switch on %d\n", percent);
    _target = percent;
    _start = _target;
    analogWrite(_pin, _start);
}

void Sun::switch_off(long now_ms)
{
    if (_target == 0)
        return;
    //Serial.printf("Sun switch off\n");
    _target = 0;
    _start_time = now_ms;
}
