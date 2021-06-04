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
    unsigned long _debounce_time;
    unsigned long _count;
    CountMode _count_mode;

    int _previous_steady_state;  // the previous steady state from the input pin, used to detect pressed and released event
    int _last_steady_state;      // the last steady state from the input pin
    int _last_flickerable_state; // the last flickerable state from the input pin
    int _current_state;          // the current reading from the input pin

    unsigned long _last_debounce_time; // the last time the output pin was toggled

public:
    Button(int pin);
    void set_debounce_time(unsigned long time);
    int get_state(void) const;
    int get_state_raw(void);
    bool is_pressed(void) const;
    bool is_released(void) const;
    void set_count_mode(CountMode mode);
    unsigned long get_count(void) const;
    void reset_count(void);
    void loop(long now_ms);
};
