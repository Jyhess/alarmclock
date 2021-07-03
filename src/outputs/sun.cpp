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
    auto target = state.get_sun_value();
    if (state.is_alarm_playing())
    {
        target = state.get_alarm_value();
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
        unsigned long change = elapsed_ms * 255  / switch_duration;
        long current = 0;  // Need long to have computation place
        if (_start > _target)
        {
            current = static_cast<long>(_start) - change;
            if (current <= _target)
            {
                current = _target;
                _start = _target;
            }
        }
        else
        {
            current = static_cast<long>(_start) + change;
            if (current > _target)
            {
                current = _target;
                _start = _target;
            }
        }
        if (current != _last)
        {
            uint8_t value = current * current / 255;
#ifdef DEBUG_ALARM
            char buffer[30];
            snprintf_P(buffer, 30, PSTR("Sun %d->%d %d=>%d\n"), _start, _target, current, value);
            Serial.print(buffer);
#endif
            analogWrite(_pin, value);
            _last = current;
        }
    }
}
