#pragma once

#include "U8g2lib.h"
#include "time.h"
#include "WString.h"
#include "outputs.h"

class Oled
{
public:
    Oled();
    void setup(void);
    void loop(const Outputs &outputs);

private:
    U8G2_SH1106_128X64_NONAME_1_HW_I2C _u8g;

    void _draw_normal(const Outputs &outputs);
    void _draw_alarm_select(const Outputs &outputs);
    void _draw_alarm_set_hour(const Outputs &outputs);
    void _draw_alarm_set_minute(const Outputs &outputs);
    String _make_time_str(const Time &time) const;
};
