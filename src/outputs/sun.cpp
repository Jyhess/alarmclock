#include <Arduino.h>
#include "outputs/sun.h"
#include "utils/ms_diff.h"

namespace
{
    const uint16_t switch_duration = 3000;
}

Sun::Sun(uint8_t pin) : _pin(pin), _start(0), _target(0), _last(0), _start_time(0)
{
}

void Sun::setup()
{
    pinMode(_pin, OUTPUT);
    analogWrite(_pin, 0);
}

void Sun::loop(const State &state)
{
    uint8_t target = state.get_sun_percent();
    if (state.is_alarm_playing())
    {
        target = state.get_alarm_percent();
    }
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
        unsigned long elapsed_ms = ms_diff(_start_time, state.now_ms());
        uint8_t change = 100 * elapsed_ms / switch_duration;
        unsigned long current = 0;  // Need unsigned long to have computation place
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
            uint8_t value = current * current * 255 / (100 * 100);
            // char buffer[30];
            // snprintf_P(buffer, 30, PSTR("Sun %d->%d %d%%=%d\n"), _start, _target, current, value);
            // Serial.print(buffer);
            analogWrite(_pin, value);
            _last = current;
        }
    }
}
