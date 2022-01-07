#include <Arduino.h>
#include "outputs/sun.h"
#include "js_ms_diff.h"

namespace
{
    const uint16_t switch_duration = 3000;
}

Sun::Sun(uint8_t pin_low, uint8_t pin_mid, uint8_t pin_high)
    : _pin_low(pin_low), _pin_mid(pin_mid), _pin_high(pin_high), _start(0), _target(0), _last(0), _start_time(0)
{
}

void Sun::setup()
{
    pinMode(_pin_low, OUTPUT);
    pinMode(_pin_mid, OUTPUT);
    pinMode(_pin_high, OUTPUT);
    analogWrite(_pin_low, 0);
    analogWrite(_pin_mid, 0);
    analogWrite(_pin_high, 0);
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
        long current = _compute_current(state.now_ms());
        if (current != _last)
        {
            uint8_t low_value = _compute_output(current, 0, 20);
            uint8_t mid_value = _compute_output(current, 20, 180);
            uint8_t high_value = _compute_output(current, 170, 255);
#ifdef DEBUG_ALARM
            char buffer[40];
            snprintf_P(buffer, 40, PSTR("Sun %d->%d %ld=>%d, %d, %d\n"), _start, _target, current, low_value, mid_value, high_value);
            Serial.print(buffer);
#endif
            analogWrite(_pin_low, low_value);
            analogWrite(_pin_mid, mid_value);
            analogWrite(_pin_high, high_value);
            _last = current;
        }
    }
}

long Sun::_compute_current(unsigned long now_ms)
{
    unsigned long elapsed_ms = ms_diff(_start_time, now_ms);
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
    return current;
}

uint8_t Sun::_compute_output(long current, long min, long max) const
{
    if( current < min)
    {
        return 0;
    }
    else if(current >= max)
    {
        return 255;
    }
    else
    {
        current = current - min;
        long delta = (max - min);
        return current * current * 255 / (delta * delta);
    }
}
