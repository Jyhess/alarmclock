#pragma once

#include "state.h"
#include "outputs/oled.h"
#include "outputs/buzzer.h"
#include "outputs/sun.h"

class Outputs
{
private:
    Buzzer _buzzer;
    Sun _sun;
    Oled _oled;

public:
    Outputs(
        uint8_t buzeer_pin, uint8_t sun_pin_low, uint8_t sun_pin_mid, uint8_t sun_pin_high
#ifndef OLED_I2C
        ,
        uint8_t oled_clock, uint8_t oled_data, uint8_t oled_cs, uint8_t oled_dc, uint8_t oled_reset
#endif
    );
    void setup();
    void loop(const State &state);

    const Sun & sun() const { return _sun; }
};
