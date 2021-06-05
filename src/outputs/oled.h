#pragma once

#include "U8g2lib.h"
#include "utils/time.h"
#include "WString.h"
#include "state.h"

class Oled
{
public:
    Oled();
    void setup(void);
    void loop(const State &state);

private:
    U8G2_SH1106_128X64_NONAME_1_HW_I2C _u8g;

    void _draw_normal(const State &state);
    void _draw_alarm_select(const State &state);
    void _draw_alarm_set_hour(const State &state);
    void _draw_alarm_set_minute(const State &state);
    void _draw_item(int x, int y, const String &str, bool selected);
    String _make_time_str(const Time &time) const;
};
