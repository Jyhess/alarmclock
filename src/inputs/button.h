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
    bool _flickering;

    long _last_debounce_time; // the last time the output pin was toggled

public:
    Button(int pin);
    void setup();
    inline void set_debounce_time(unsigned long time) { _debounce_time = time; }
    int get_state() const;
    int get_state_raw();
    bool is_pressed() const;
    bool has_been_pressed() const;
    bool has_been_released() const;
    inline void set_count_mode(CountMode mode) { _count_mode = mode; }
    inline unsigned long get_count() const { return _count; }
    inline void reset_count() { _count = 0; }
    void loop(long now_ms);
};
