#include <Arduino.h>

#include "inputs/vibration.h"

namespace
{
    const uint8_t smoothing = 10;
    const uint8_t move_limit = 100;
}

Vibration::Vibration(uint8_t pin) : _pin(pin), _value(0)
{
}

void Vibration::setup()
{
    pinMode(_pin, INPUT);
}

void Vibration::loop(unsigned long now_ms)
{
    // Time filtered
    _value = (static_cast<uint16_t>(_value) * (smoothing-1) + analogRead(_pin)) / smoothing;
    //_value = pulseIn(_pin, HIGH);
}

int Vibration::get_value() const { return _value; }

bool Vibration::has_moved() const { return get_value() > move_limit; }
