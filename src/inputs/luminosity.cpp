#include <Arduino.h>

#include "inputs/luminosity.h"

namespace
{
    const uint8_t dark_limit = 15;
}

Luminosity::Luminosity(uint8_t pin) : _pin(pin), _value(128)
{
}

void Luminosity::setup() {}

void Luminosity::loop(unsigned long now_ms)
{
    // Time filtered
    _value = (static_cast<unsigned long>(_value) * 9 + analogRead(_pin)) / 10;
}

int Luminosity::get_value() const { return _value; }

bool Luminosity::is_dark() const { return get_value() < dark_limit; }
