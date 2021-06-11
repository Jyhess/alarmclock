#pragma once

#include "state.h"

class Buzzer
{
private:
    uint8_t _pin;
    unsigned long _start_note_ms;
    int _note;
    bool _is_playing;

public:
    Buzzer(uint8_t pin);
    void setup();
    void loop(const State &state);

private:
    void _play(unsigned long now_ms);
};
