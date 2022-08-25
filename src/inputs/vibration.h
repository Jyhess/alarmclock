#pragma once

#include <stdio.h>

class Vibration
{
private:
    uint8_t _pin;
    uint8_t _value;

public:
    explicit Vibration(uint8_t pin);
    void setup();
    void loop(unsigned long now_ms);

    int get_value() const;
    bool has_moved() const;
};
