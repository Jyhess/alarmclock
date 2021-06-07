#include "inputs/button_with_count.h"

ButtonWithCount::ButtonWithCount(int pin) : _button(pin), _count(0), _count_mode(CountMode::Falling)
{
}

void ButtonWithCount::setup()
{
    _button.setup();
}

void ButtonWithCount::loop(unsigned long now_ms)
{
    _button.loop(now_ms);
    if (_button.has_changed())
    {
        //char buffer[11];
        //snprintf_P(buffer, 11, PSTR("B %d->%d\n"), _last_steady_state, current_state);
        //Serial.print(buffer);
        if (_count_mode == CountMode::Both)
            _count++;
        else if (_count_mode == CountMode::Falling)
        {
            if (_button.has_been_pressed())
                _count++;
        }
        else if (_count_mode == CountMode::Rising)
        {
            if (_button.has_been_released())
                _count++;
        }
    }
}