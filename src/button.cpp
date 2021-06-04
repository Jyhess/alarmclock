#include <Arduino.h>
#include "button.h"
#include "utils/ms_diff.h"

Button::Button(int pin) : _pin(pin), _debounce_time(0), _count(0), _count_mode(CountMode::Falling), _last_debounce_time(0)
{
    pinMode(_pin, INPUT_PULLUP);

    _previous_steady_state = digitalRead(_pin);
    _last_steady_state = digitalRead(_pin);
    _last_flickerable_state = digitalRead(_pin);
}

void Button::set_debounce_time(unsigned long time)
{
    _debounce_time = time;
}

int Button::get_state(void) const
{
    return _last_steady_state;
}

int Button::get_state_raw(void)
{
    return digitalRead(_pin);
}

bool Button::is_pressed(void) const
{
    if (_previous_steady_state == HIGH && _last_steady_state == LOW)
        return true;
    else
        return false;
}

bool Button::is_released(void) const
{
    if (_previous_steady_state == LOW && _last_steady_state == HIGH)
        return true;
    else
        return false;
}

void Button::set_count_mode(CountMode mode)
{
    _count_mode = mode;
}

unsigned long Button::get_count(void) const
{
    return _count;
}

void Button::reset_count(void)
{
    _count = 0;
}

void Button::loop(long now_ms)
{
    // read the state of the switch/button:
    _current_state = digitalRead(_pin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch/button changed, due to noise or pressing:
    if (_current_state != _last_flickerable_state)
    {
        // reset the debouncing timer
        _last_debounce_time = now_ms;
        // save the the last flickerable state
        _last_flickerable_state = _current_state;
    }

    if (ms_diff(_last_debounce_time, now_ms) >= _debounce_time)
    {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // save the the steady state
        _previous_steady_state = _last_steady_state;
        _last_steady_state = _current_state;
    }

    if (_previous_steady_state != _last_steady_state)
    {
        if (_count_mode == CountMode::Both)
            _count++;
        else if (_count_mode == CountMode::Falling)
        {
            if (_previous_steady_state == HIGH && _last_steady_state == LOW)
                _count++;
        }
        else if (_count_mode == CountMode::Rising)
        {
            if (_previous_steady_state == LOW && _last_steady_state == HIGH)
                _count++;
        }
    }
}
