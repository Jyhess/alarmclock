#include <Arduino.h>

#include "inputs/button_no_bounce.h"

namespace
{

}

ButtonNoBounce::ButtonNoBounce(uint8_t pin) : _pin(pin), _previous_state(false), _current_state(false), _press_time(0), _release_time(0)
{
}

void ButtonNoBounce::setup()
{
    pinMode(_pin, INPUT);

    _previous_state = read_state();
    _current_state = _previous_state;
}

void ButtonNoBounce::loop(unsigned long now_ms)
{
    // read the state of the switch/button:
    _previous_state = _current_state;
    _current_state = read_state();

    if (_previous_state != _current_state)
    {
        if(_current_state)
            _press_time = now_ms;
        else
            _release_time = now_ms;
    }
}

bool ButtonNoBounce::read_state()
{
    return digitalRead(_pin) == HIGH;
}

bool ButtonNoBounce::has_changed() const
{
    return _previous_state != _current_state;
}

bool ButtonNoBounce::is_pressed() const
{
    return _current_state;
}

bool ButtonNoBounce::has_been_pressed() const
{
    return !_previous_state && _current_state;
}

bool ButtonNoBounce::has_been_released() const
{
    return _previous_state && !_current_state;
}
