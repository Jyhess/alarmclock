#include "inputs/inputs.h"
#include "js_ms_diff.h"

Inputs::Inputs(uint8_t red_pin, uint8_t yellow_pin, uint8_t green_pin, uint8_t luminosity_pin, uint8_t vibration_pin)
 : _now_ms(0), _red(red_pin), _yellow(yellow_pin), _green(green_pin), _luminosity(luminosity_pin), _vibration(vibration_pin), _last_rtc_read(0)
{
}

void Inputs::setup()
{
    _rtc.setup();
    _red.setup();
    _green.setup();
    _yellow.setup();
    _luminosity.setup();
    _vibration.setup();
    _read_rtc();
}

void Inputs::loop()
{
    _now_ms = millis();
    _red.loop(_now_ms);
    _yellow.loop(_now_ms);
    _green.loop(_now_ms);
    _luminosity.loop(_now_ms);
    _vibration.loop(_now_ms);
    if (ms_diff(_last_rtc_read, _now_ms) > 100)
    {
        _read_rtc();
    }
}

bool Inputs::red_has_been_pressed(unsigned long threshold) const
{
    return _red.has_been_released() && ms_diff(_red.press_time(), _now_ms) < threshold;
}

bool Inputs::yellow_has_been_pressed(unsigned long threshold) const
{
    return _yellow.has_been_released() && ms_diff(_yellow.press_time(), _now_ms) < threshold;
}

bool Inputs::green_has_been_pressed(unsigned long threshold) const
{
    return _green.has_been_released() && ms_diff(_green.press_time(), _now_ms) < threshold;
}

unsigned long Inputs::red_long_pressed(unsigned long threshold) const
{
    return _red.is_pressed() && ms_diff(_red.press_time(), _now_ms) > threshold;
}

unsigned long Inputs::yellow_long_pressed(unsigned long threshold) const
{
    return _yellow.is_pressed() && ms_diff(_yellow.press_time(), _now_ms) > threshold;
}

unsigned long Inputs::green_long_pressed(unsigned long threshold) const
{
    return _green.is_pressed() && ms_diff(_green.press_time(), _now_ms) > threshold;
}

void Inputs::_read_rtc()
{
    _time = _rtc.get_time();
    _last_rtc_read = _now_ms;
}
