#pragma once

#include "state.h"

class Buzzer
{
private:
    int _pin;
    long _start_note_ms;
    int _note;
    bool _is_playing;

public:
    Buzzer(int pin);
    void setup();
    void loop(const State &state);

private:
    void _play(long now_ms);
};
