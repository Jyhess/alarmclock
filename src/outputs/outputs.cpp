#include "outputs/outputs.h"

Outputs::Outputs(int buzeer_pin, int sun_pin) : _buzzer(buzeer_pin), _sun(sun_pin)
{
}

void Outputs::setup()
{
    _oled.setup();
    _buzzer.setup();
    _sun.setup();
}

void Outputs::loop(const State &state)
{
    _oled.loop(state);
    _buzzer.loop(state);
    _sun.loop(state);
}
