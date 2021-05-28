#pragma once

#include "U8g2lib.h"
#include "time.h"
#include "char_raii.h"

class Oled
{
private:
    U8G2_SH1106_128X64_NONAME_1_HW_I2C _u8g;
    uint8_t _brightness;
    bool _updated;
    Time _time;

    Time _alarm_1;
    Time _alarm_2;

    void _draw();
    char *_make_time_str(const Time &time) const;

public:
    Oled();
    void setup(void);
    void loop(void);

    inline uint8_t brightness() const { return _brightness; }
    inline void set_brightness(uint8_t brightness)
    {
        _brightness = brightness;
        _updated = true;
    }

    inline const Time &current_time() const { return _time; }
    inline void set_current_time(const Time &time)
    {
        _time = time;
        _updated = true;
    }

    inline const Time &get_alarm_1() const { return _alarm_1; }
    inline void set_alarm_1(const Time &time)
    {
        _alarm_1 = time;
        _updated = true;
    }
    inline const Time &get_alarm_2() const { return _alarm_2; }
    inline void set_alarm_2(const Time &time)
    {
        _alarm_2 = time;
        _updated = true;
    }
};
