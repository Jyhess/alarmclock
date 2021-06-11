#pragma once

#include <stdint.h>

class Button
{
private:
    uint8_t _pin;

    bool _previous_steady_state;  // the previous steady state from the input pin, used to detect pressed and released event
    bool _last_steady_state;      // the last steady state from the input pin
    bool _last_flickerable_state; // the last flickerable state from the input pin
    bool _flickering;

    unsigned long _last_debounce_time; // the last time the output pin was toggled

public:
    Button(uint8_t pin);

    void setup();
    void loop(unsigned long now_ms);

    inline int get_state() const { return _last_steady_state; }
    int get_state_raw();
    bool has_changed() const;
    bool is_pressed() const;
    bool has_been_pressed() const;
    bool has_been_released() const;
    unsigned long change_time(unsigned long now_ms) const;
};

