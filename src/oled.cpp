#include "oled.h"

Oled::Oled() : _u8g(U8G2_R0)
{
}

void Oled::setup()
{
    _u8g.begin();
}

void Oled::loop(const Data &data)
{
    if (data.is_updated())
    {
        _draw(data);
    }
}

void Oled::_draw(const Data & data)
{
    CharRaii time = CharRaii(_make_time_str(data.get_current_time()));
    CharRaii alarm_1 = CharRaii(_make_time_str(data.get_alarm_1()));
    CharRaii alarm_2 = CharRaii(_make_time_str(data.get_alarm_2()));

    _u8g.firstPage();
    do
    {
        _u8g.setContrast(data.get_display_brightness());
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
