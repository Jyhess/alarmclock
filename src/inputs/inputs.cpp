#include "inputs/inputs.h"
#include "utils/ms_diff.h"

Inputs::Inputs(int red_pin, int yellow_pin, int green_pin) : _now_ms(0), _red(red_pin), _yellow(yellow_pin), _green(green_pin), _last_rtc_read(0)
{
}

void Inputs::setup()
{
    _rtc.setup();
}

void Inputs::loop()
{
    _now_ms = millis();
    _red.loop(_now_ms);
    _yellow.loop(_now_ms);
    _green.loop(_now_ms);
    if (ms_diff(_last_rtc_read, _now_ms) > 1000)
    {
        RtcDateTime now = _rtc.GetDateTime();
        _time = Time(now.Hour(), now.Minute());
        _last_rtc_read = _now_ms;
    }
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
