#include "inputs.h"

Inputs::Inputs(int red_pin, int yellow_pin, int green_pin) : _now_ms(0), _red(red_pin), _yellow(yellow_pin), _green(green_pin)
{
}

void Inputs::loop(int now_ms)
{
    _now_ms = now_ms;
    _red.loop(now_ms);
    _yellow.loop(now_ms);
    _green.loop(now_ms);
}

bool Inputs::red_has_been_pressed() const
{
    return _red.is_pressed();
}

bool Inputs::yellow_has_been_pressed() const
{
    return _yellow.is_pressed();
}
bool Inputs::green_has_been_pressed() const
{
    return _green.is_pressed();
}
