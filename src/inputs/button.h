#pragma once

class Button
{
public:
    enum CountMode
    {
        Falling,
        Rising,
        Both
    };

private:
    int _pin;
    unsigned long _count;
    CountMode _count_mode;

    bool _previous_steady_state;  // the previous steady state from the input pin, used to detect pressed and released event
    bool _last_steady_state;      // the last steady state from the input pin
    bool _last_flickerable_state; // the last flickerable state from the input pin
    bool _flickering;

    long _last_debounce_time; // the last time the output pin was toggled

public:
    Button(int pin);

    void setup();
    void loop(long now_ms);

    inline int get_state() const { return _last_steady_state; }
    int get_state_raw();
    bool is_pressed() const;
    bool has_been_pressed() const;
    bool has_been_released() const;
    inline void set_count_mode(CountMode mode) { _count_mode = mode; }
    inline unsigned long get_count() const { return _count; }
    inline void reset_count() { _count = 0; }
};
