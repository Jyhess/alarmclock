#pragma once

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
    void loop(long now_ms);
    void play();
    void stop();
};
