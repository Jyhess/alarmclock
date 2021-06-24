#include <Arduino.h>

#include "utils/pitches.h"
#include "utils/ms_diff.h"
#include "outputs/buzzer.h"

namespace
{
    //int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
    //unsigned long durations[] = {4, 8, 8, 4, 4, 4, 4, 4};
    const int melody[] = {
        NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5,
        NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5,
        NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5,
        NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5};
    const uint8_t durations[] = {
        8, 8, 8, 4, 4, 4,
        4, 5, 8, 8, 8, 8,
        8, 8, 8, 4, 4, 4,
        4, 5, 8, 8, 8, 8};
    //int melody[] = {
    //    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
    //    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
    //    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
    //    NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4,
    //    NOTE_F4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_D4,
    //    0, NOTE_A3, NOTE_C4, NOTE_B3, NOTE_D4, NOTE_B3, NOTE_E4, NOTE_F4,
    //    NOTE_F4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4,
    //    NOTE_D4, 0, 0, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_F4,
    //    NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4,
    //    NOTE_A4, NOTE_D4, 0, NOTE_D4, NOTE_E3, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4,
    //    NOTE_D4, 0, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_D4};
    //int durations[] = {
    //    4, 8, 4, 8, 4, 8, 8, 8, 8, 4, 8, 4, 8, 4, 8, 8, 8, 8, 4, 8, 4, 8,
    //    4, 8, 8, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8,
    //    8, 4, 8, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 4, 8, 4,
    //    4, 8, 8, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8,
    //    8, 4, 8, 8, 8, 4, 4, 4, 8, 4, 8, 8, 8, 4, 4, 8, 8};
    const uint8_t nb_notes = sizeof(melody) / sizeof(melody[0]);
    const unsigned int ms_per_note = 1000;
}

Buzzer::Buzzer(uint8_t pin) : _pin(pin), _start_note_ms(0), _note(0), _is_playing(false)
{
}

void Buzzer::setup()
{
}

void Buzzer::loop(const State &state)
{
    if (_is_playing && (!state.is_alarm_playing() || state.get_alarm_percent() < 100))
    {
        _is_playing = false;
        noTone(_pin);
        //Serial.println("Buzzer stopped");
    }
    else if (!_is_playing && state.is_alarm_playing() && state.get_alarm_percent() >= 100)
    {
        _is_playing = true;
        _note = 0;
        _start_note_ms = state.now_ms();
        //Serial.printf("Buzzer started %d\n", _start_note_ms);
        _play(state.now_ms());
    }
    if (_is_playing)
    {
        _play(state.now_ms());
    }
}

void Buzzer::_play(unsigned long now_ms)
{
    unsigned int note_duration = ms_per_note / durations[_note];
    unsigned long note_played_ms = ms_diff(_start_note_ms, now_ms);
    if (note_played_ms >= (note_duration * 1.3))
    {
        _note = (_note + 1) % nb_notes;
        if (melody[_note])
        {
            tone(_pin, melody[_note], ms_per_note / durations[_note]);
        }
        _start_note_ms = now_ms;
    }
}
