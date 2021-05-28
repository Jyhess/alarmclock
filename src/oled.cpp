#include "oled.h"

Oled::Oled() : _u8g(U8G2_R0), _brightness(0), _last_brightness(0)
{
}

void Oled::setup()
{
    _u8g.begin();
}

void Oled::loop(void)
{
    if( _time != _last_time || _brightness != _last_brightness)
    {
        _draw();
        _last_time = _time;
        _last_brightness = _brightness;
    }
}

void Oled::display_time(const RtcDateTime &dt)
{
    _time = dt;
}

void Oled::_draw()
{
    int hour = _time.Hour();
    int minute = _time.Minute();
    char data[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    sprintf(data, "%2d:%02d", hour, minute);

    _u8g.firstPage();
    do
    {
        _u8g.setContrast(_brightness);
        _u8g.setFont(u8g2_font_fur35_tn);
        _u8g.drawStr(1, 36, data);
        _u8g.setColorIndex(1);

        _u8g.setFont(u8g2_font_fur14_tn);
        _u8g.drawStr(1, 63, "12:34");
        _u8g.drawStr(80, 63, "56:78");
        _u8g.setColorIndex(1);
    } while (_u8g.nextPage());
}