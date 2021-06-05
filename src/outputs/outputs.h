#pragma once

#include "state.h"
#include "outputs/oled.h"
#include "outputs/buzzer.h"
#include "outputs/sun.h"


class Outputs
{
private:
    Oled _oled;
    Buzzer _buzzer;
    Sun _sun;

public:
    Outputs(int buzeer_pin, int sun_pin);
    void setup();
    void loop(const State &state);
};
