#pragma once

#include "U8g2lib.h"
#include <RtcDS3231.h>

class Oled
{
private:
    U8G2_SH1106_128X64_NONAME_1_HW_I2C _u8g;
    uint8_t _brightness;
    uint8_t _last_brightness;
    RtcDateTime _last_time;
    RtcDateTime _time;

    void _draw();

public:
    Oled();
    void setup(void);
    void loop(void);
    const RtcDateTime &last_time() const { return _last_time; }
    void display_time(const RtcDateTime &dt);
};
