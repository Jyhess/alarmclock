#include "oled.h"

namespace
{
    static int LINE_1 = 21;
    static int LINE_2 = 42;
    static int LINE_3 = 63;
    static int COL_1 = 2;
    static int COL_2 = 42;
    static int COL_3 = 83;
}

Oled::Oled() : _u8g(U8G2_R0)
{
}

void Oled::setup()
{
    _u8g.begin();
}

void Oled::loop(const Outputs &outputs)
{
    if (outputs.is_updated())
    {
        switch (outputs.get_step())
        {
        case Step::NORMAL:
            _draw_normal(outputs);
            break;
        case Step::ALARM_SELECT:
            _draw_alarm_select(outputs);
            break;
        case Step::ALARM_SET_HOUR:
            _draw_alarm_set_hour(outputs);
            break;
        case Step::ALARM_SET_MINUTE:
            _draw_alarm_set_minute(outputs);
            break;
        }
    }
}

void Oled::_draw_normal(const Outputs &outputs)
{
    String time = _make_time_str(outputs.get_current_time());
    String alarm;

    if (outputs.is_alarm_on())
    {
        alarm = _make_time_str(outputs.get_alarm());
    }

    _u8g.firstPage();
    do
    {
        _u8g.setContrast(outputs.get_display_brightness());
        _u8g.setFont(u8g2_font_fur35_tn);
        _u8g.drawStr(COL_1, 38, time.c_str());
        _u8g.setFont(u8g2_font_fur14_tn);
        _u8g.drawStr(COL_3 - 5, LINE_3, alarm.c_str());
    } while (_u8g.nextPage());
}

void Oled::_draw_alarm_select(const Outputs &outputs)
{
    String alarm = _make_time_str(outputs.get_alarm());
    Array<String, PREDEFINED_ALARMS> alarms;
    for (int i = 0; i < PREDEFINED_ALARMS; ++i)
    {
        alarms[i] = _make_time_str(outputs.get_predefine_alarms()[i]);
    }

    const uint8_t *small = u8g2_font_fur11_tf;
    const uint8_t *selected = u8g2_font_fur14_tf;
    _u8g.firstPage();
    do
    {
        _u8g.setContrast(outputs.get_display_brightness());
        _u8g.setFont(outputs.get_alarm_index() == NO_ALARM ? selected : small);
        _u8g.drawStr(COL_1, LINE_1, " off");
        _u8g.setFont(outputs.get_alarm_index() == CUSTOM_ALARM ? selected : small);
        _u8g.drawStr(COL_3, LINE_1, alarm.c_str());
        _u8g.setFont(outputs.get_alarm_index() == 0 ? selected : small);
        _u8g.drawStr(COL_1, LINE_2, alarms[0].c_str());
        _u8g.setFont(outputs.get_alarm_index() == 1 ? selected : small);
        _u8g.drawStr(COL_2, LINE_2, alarms[1].c_str());
        _u8g.setFont(outputs.get_alarm_index() == 2 ? selected : small);
        _u8g.drawStr(COL_3, LINE_2, alarms[2].c_str());
        _u8g.setFont(outputs.get_alarm_index() == 3 ? selected : small);
        _u8g.drawStr(COL_1, LINE_3, alarms[0].c_str());
        _u8g.setFont(outputs.get_alarm_index() == 4 ? selected : small);
        _u8g.drawStr(COL_2, LINE_3, alarms[1].c_str());
        _u8g.setFont(outputs.get_alarm_index() == 5 ? selected : small);
        _u8g.drawStr(COL_3, LINE_3, alarms[2].c_str());
    } while (_u8g.nextPage());
}

void Oled::_draw_alarm_set_hour(const Outputs &outputs)
{
    String time = _make_time_str(outputs.get_current_time());
    String alarm = _make_time_str(outputs.get_alarm());

    _u8g.firstPage();
    do
    {
        _u8g.setContrast(outputs.get_display_brightness());
        _u8g.setFont(u8g2_font_fur35_tn);
        _u8g.drawStr(COL_1, 38, alarm.c_str());
        _u8g.setFont(u8g2_font_fur14_tn);
        _u8g.drawStr(COL_1, LINE_3, time.c_str());
        _u8g.drawFrame(0, 0, 56, 39);
    } while (_u8g.nextPage());
}

void Oled::_draw_alarm_set_minute(const Outputs &outputs)
{
    String time = _make_time_str(outputs.get_current_time());
    String alarm = _make_time_str(outputs.get_alarm());

    _u8g.firstPage();
    do
    {
        _u8g.setContrast(outputs.get_display_brightness());
        _u8g.setFont(u8g2_font_fur35_tn);
        _u8g.drawStr(COL_1, 38, alarm.c_str());
        _u8g.setFont(u8g2_font_fur14_tn);
        _u8g.drawStr(COL_1, LINE_3, time.c_str());
        _u8g.drawFrame(65, 0, 56, 39);
    } while (_u8g.nextPage());
}

String Oled::_make_time_str(const Time &time) const
{
    String data("  :  ");
    char *begin = data.begin();
    if (time.get_hour() < 10)
        begin++;
    sprintf(begin, "%2d:%02d", time.get_hour(), time.get_minute());
    return data;
}
