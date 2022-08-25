#pragma once

#include <stdint.h>


class ButtonCapacitive
{
private:
    uint8_t _pin;

    bool _previous_state;
    bool _current_state;
    unsigned long _press_time;
    unsigned long _release_time;

public:
    explicit ButtonCapacitive(uint8_t pin);

    void setup();
    void loop(unsigned long now_ms);

    bool read_state();

    inline bool get_state() const { return _current_state; }
    bool has_changed() const;
    bool is_pressed() const;
    bool has_been_pressed() const;
    bool has_been_released() const;
    inline unsigned long press_time() const { return _press_time; }
    inline unsigned long release_time() const { return _release_time; };
};

