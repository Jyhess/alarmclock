#include "outputs/outputs.h"

Outputs::Outputs(
    int buzeer_pin, int sun_pin
#ifndef OLED_I2C
    ,
    uint8_t oled_clock, uint8_t oled_data, uint8_t oled_cs, uint8_t oled_dc, uint8_t oled_reset
#endif
    ) : _buzzer(buzeer_pin), _sun(sun_pin)
#ifndef OLED_I2C
        ,
        _oled(oled_clock, oled_data, oled_cs, oled_dc, oled_reset)
#endif
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
