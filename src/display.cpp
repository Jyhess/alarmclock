#include "display.h"

Display::Display(uint8_t pinClk, uint8_t pinDIO) : _tm_1637(pinClk, pinDIO), _brightness(0)
{

}

void Display::setup()
{
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
    _tm_1637.setSegments(data);
    _tm_1637.setBrightness(_brightness, true);
}

void Display::display_time(const RtcDateTime &dt)
{
    int hour = dt.Hour();
    int minute = dt.Minute();
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
    if (hour >= 10)
    {
        data[0] = _tm_1637.encodeDigit(hour / 10);
    }
    else
    {
        data[0] = 0x00;
    }
    data[1] = _tm_1637.encodeDigit(hour % 10) | SEG_DP;
    data[2] = _tm_1637.encodeDigit(minute / 10);
    data[3] = _tm_1637.encodeDigit(minute % 10);
    _tm_1637.setSegments(data);
}
