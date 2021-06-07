#include <Arduino.h>

#include "inputs/button.h"
#include "utils/ms_diff.h"

Button::Button(int pin) : _pin(pin), _debounce_time(50), _count(0), _count_mode(CountMode::Falling), _previous_steady_state(LOW), _last_steady_state(LOW), _last_flickerable_state(LOW), _flickering(false), _last_debounce_time(0)
{
}

void Button::setup()
{
    pinMode(_pin, INPUT_PULLUP);

    _previous_steady_state = digitalRead(_pin);
    _last_steady_state = _previous_steady_state;
    _last_flickerable_state = _previous_steady_state;
}

int Button::get_state() const
{
    return _last_steady_state;
}

int Button::get_state_raw()
{
    return digitalRead(_pin);
}

bool Button::is_pressed() const
{
    return !_flickering && _last_steady_state == LOW;
}

bool Button::has_been_pressed() const
{
    return !_flickering && _previous_steady_state == HIGH && _last_steady_state == LOW;
}

bool Button::has_been_released() const
{
    return !_flickering && _previous_steady_state == LOW && _last_steady_state == HIGH;
}

void Button::loop(long now_ms)
{
    // read the state of the switch/button:
    int current_state = digitalRead(_pin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch/button changed, due to noise or pressing:
    if (current_state != _last_flickerable_state)
    {
        // reset the debouncing timer
        _last_debounce_time = now_ms;
        // save the the last flickerable state
        _last_flickerable_state = current_state;
        _flickering = true;
    }

    if (ms_diff(_last_debounce_time, now_ms) >= _debounce_time)
    {
        _flickering = false;
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // save the the steady state
        _previous_steady_state = _last_steady_state;
        _last_steady_state = current_state;
    }

    if (!_flickering && _previous_steady_state != _last_steady_state)
    {
        //char buffer[11];
        //snprintf_P(buffer, 11, PSTR("B %d->%d\n"), _last_steady_state, current_state);
        //Serial.print(buffer);
        if (_count_mode == CountMode::Both)
            _count++;
        else if (_count_mode == CountMode::Falling)
        {
            if (has_been_pressed())
                _count++;
        }
        else if (_count_mode == CountMode::Rising)
        {
            if (has_been_released())
                _count++;
        }
    }
}
