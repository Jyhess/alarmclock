#pragma once

#include "U8g2lib.h"
#include "time.h"
#include "char_raii.h"
#include "data.h"

class Oled
{
private:
    U8G2_SH1106_128X64_NONAME_1_HW_I2C _u8g;

    void _draw(const Data &data);
    char *_make_time_str(const Time &time) const;

public:
    Oled();
    void setup(void);
    void loop(const Data & data);
};
