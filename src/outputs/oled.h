#pragma once

#include "U8g2lib.h"
#include "js_time.h"
#include "state.h"

class Oled
{
public:
#ifdef OLED_I2C
    Oled();
#else
    Oled(uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset);
#endif
    void setup(void);
    void loop(const State &state);

private:
#ifdef OLED_I2C
    U8G2_SH1106_128X64_NONAME_1_HW_I2C _u8g;
#else
    U8G2_SSD1309_128X64_NONAME2_1_4W_SW_SPI _u8g;
#endif
    void _draw_normal(const State &state);
    void _draw_alarm_select(const State &state);
    void _draw_alarm_set(const State &state);
    void _draw_item(int x, int y, const char* str, bool selected);
    static void _make_time_cstr(char * data, const Time &time);
};
