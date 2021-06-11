#include <Arduino.h>

#include "inputs/button.h"
#include "utils/ms_diff.h"

namespace
{
    static const int debounce_time = 10;
}

Button::Button(uint8_t pin) : _pin(pin), _previous_steady_state(false), _last_steady_state(false), _last_flickerable_state(false), _flickering(false), _last_debounce_time(0)
{
}

void Button::setup()
{
    pinMode(_pin, INPUT_PULLUP);

    _previous_steady_state = digitalRead(_pin);
    _last_steady_state = _previous_steady_state;
    _last_flickerable_state = _previous_steady_state;
}

void Button::loop(unsigned long now_ms)
{
    // read the state of the switch/button:
    bool current_state = digitalRead(_pin) == LOW;

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

    if (ms_diff(_last_debounce_time, now_ms) >= debounce_time)
    {
        _flickering = false;
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // save the the steady state
        _previous_steady_state = _last_steady_state;
        _last_steady_state = current_state;
    }
}

bool Button::has_changed() const
{
    return !_flickering && _previous_steady_state != _last_steady_state;
}

int Button::get_state_raw()
{
    return digitalRead(_pin);
}

bool Button::is_pressed() const
{
    return !_flickering && _last_steady_state;
}

bool Button::has_been_pressed() const
{
    return !_flickering && !_previous_steady_state && _last_steady_state;
}

bool Button::has_been_released() const
{
    return !_flickering && _previous_steady_state && !_last_steady_state;
}

unsigned long Button::change_time(unsigned long now_ms) const
{
    return ms_diff(_last_debounce_time, now_ms);
}
