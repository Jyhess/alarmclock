#pragma once

#include <stdio.h>

class Luminosity
{
private:
    uint8_t _pin;
    int _value;

public:
    Luminosity(uint8_t pin);
    void setup();
    void loop(unsigned long now_ms);

    int get_value() const;
    bool is_dark() const;
};
