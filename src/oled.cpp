#include "oled.h"

Oled::Oled() : _u8g(U8G2_R0), _brightness(0), _updated(true), _alarm_1(12, 34), _alarm_2(1, 2)
{
}

void Oled::setup()
{
    _u8g.begin();
}

void Oled::loop(void)
{
    if (_updated)
    {
        _draw();
        _updated = false;
    }
}

void Oled::_draw()
{
    CharRaii time = CharRaii(_make_time_str(_time));
    CharRaii alarm_1 = CharRaii(_make_time_str(_alarm_1));
    CharRaii alarm_2 = CharRaii(_make_time_str(_alarm_2));

    _u8g.firstPage();
    do
    {
        _u8g.setContrast(_brightness);
        _u8g.setFont(u8g2_font_fur35_tn);
        _u8g.drawStr(1, 36, time.data());
        _u8g.setColorIndex(1);

        _u8g.setFont(u8g2_font_fur14_tn);
        _u8g.drawStr(1, 63, alarm_1.data());
        _u8g.drawStr(80, 63, alarm_2.data());
        _u8g.setColorIndex(1);
    } while (_u8g.nextPage());
}

char *Oled::_make_time_str(const Time &time) const
{
    CharRaii data(6);
    sprintf(data.data(), "%2d:%02d", time.get_hour(), time.get_minute());
    return data.move();
}
