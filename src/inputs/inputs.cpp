#include "inputs/inputs.h"
#include "js_ms_diff.h"

Inputs::Inputs(uint8_t send_pin, uint8_t middle_pin, uint8_t left_pin, uint8_t right_pin, uint8_t luminosity_pin, uint8_t vibration_pin)
 : _now_ms(0), _middle(send_pin, middle_pin), _left(send_pin, left_pin), _right(send_pin, right_pin), _luminosity(luminosity_pin), _vibration(vibration_pin), _last_rtc_read(0)
{
}

void Inputs::setup()
{
    _rtc.setup();
    _middle.setup();
    _right.setup();
    _left.setup();
    _luminosity.setup();
    _vibration.setup();
    _read_rtc();
}

void Inputs::loop()
{
    _now_ms = millis();
    _middle.loop(_now_ms);
    _left.loop(_now_ms);
    _right.loop(_now_ms);
    _luminosity.loop(_now_ms);
    _vibration.loop(_now_ms);
    if (ms_diff(_last_rtc_read, _now_ms) > 100)
    {
        _read_rtc();
    }
}

bool Inputs::middle_has_been_pressed(unsigned long threshold) const
{
    return _middle.has_been_released() && ms_diff(_middle.press_time(), _now_ms) < threshold;
}

bool Inputs::left_has_been_pressed(unsigned long threshold) const
{
    return _left.has_been_released() && ms_diff(_left.press_time(), _now_ms) < threshold;
}

bool Inputs::right_has_been_pressed(unsigned long threshold) const
{
    return _right.has_been_released() && ms_diff(_right.press_time(), _now_ms) < threshold;
}

unsigned long Inputs::middle_long_pressed(unsigned long threshold) const
{
    return _middle.is_pressed() && ms_diff(_middle.press_time(), _now_ms) > threshold;
}

unsigned long Inputs::left_long_pressed(unsigned long threshold) const
{
    return _left.is_pressed() && ms_diff(_left.press_time(), _now_ms) > threshold;
}

unsigned long Inputs::right_long_pressed(unsigned long threshold) const
{
    return _right.is_pressed() && ms_diff(_right.press_time(), _now_ms) > threshold;
}

void Inputs::_read_rtc()
{
    _time = _rtc.get_time();
    _last_rtc_read = _now_ms;
}
