#include "outputs/oled.h"

Oled::Oled() : _u8g(U8G2_R0)
{
}

void Oled::setup()
{
    _u8g.begin();
}

void Oled::loop(const State &state)
{
    if (state.is_updated())
    {
        switch (state.get_step())
        {
        case Step::NORMAL:
        case Step::ALARM_PLAYING:
            _draw_normal(state);
            break;
        case Step::ALARM_SELECT:
            _draw_alarm_select(state);
            break;
        case Step::ALARM_SET_HOUR:
            _draw_alarm_set_hour(state);
            break;
        case Step::ALARM_SET_MINUTE:
            _draw_alarm_set_minute(state);
            break;
        case Step::NO_DISPLAY:
            _u8g.noDisplay();
            break;
        }
    }
}

void Oled::_draw_normal(const State &state)
{
    String time = _make_time_str(state.get_current_time());
    String alarm;

    if (state.is_alarm_on())
    {
        alarm = _make_time_str(state.get_alarm());
    }
    if (state.is_alarm_playing())
    {
        alarm = String("> ") + alarm + String(" <");
    }
    _u8g.display();
    _u8g.setContrast(state.get_display_brightness());
    int alarm_x = _u8g.getDisplayWidth() / 2;
    int alarm_y = 56;
    _u8g.setFont(u8g2_font_fur35_tn);
    int x = (_u8g.getDisplayWidth() - _u8g.getStrWidth(time.c_str())) / 2;

    _u8g.firstPage();
    do
    {
        _u8g.setFont(u8g2_font_fur35_tn);
        _u8g.drawStr(x, 38, time.c_str());
        _draw_item(alarm_x, alarm_y, alarm, true);
    } while (_u8g.nextPage());
}

void Oled::_draw_alarm_select(const State &state)
{
    const String off = "off";
    const String alarm = _make_time_str(state.get_custom_alarm());
    Array<String, PREDEFINED_ALARMS> alarms;
    for (int i = 0; i < PREDEFINED_ALARMS; ++i)
    {
        alarms[i] = _make_time_str(state.get_predefine_alarms()[i]);
    }
    int width = _u8g.getDisplayWidth();
    int x_1 = 1 * width / 6;
    int x_2 = 3 * width / 6;
    int x_3 = 5 * width / 6;
    int height = _u8g.getDisplayHeight();
    int y_1 = 1 * height / 6;
    int y_2 = 3 * height / 6;
    int y_3 = 5 * height / 6;

    const int id = state.get_alarm_index();
    _u8g.firstPage();
    do
    {
        _u8g.setContrast(state.get_display_brightness());
        _u8g.setFont(u8g2_font_fur11_tf);
        _draw_item(x_1, y_1, off, id == NO_ALARM);
        _draw_item(x_3, y_1, alarm, id == CUSTOM_ALARM);
        _draw_item(x_1, y_2, alarms[0], id == 0);
        _draw_item(x_2, y_2, alarms[1], id == 1);
        _draw_item(x_3, y_2, alarms[2], id == 2);
        _draw_item(x_1, y_3, alarms[3], id == 3);
        _draw_item(x_2, y_3, alarms[4], id == 4);
        _draw_item(x_3, y_3, alarms[5], id == 5);
    } while (_u8g.nextPage());
}

void Oled::_draw_item(int x, int y, const String &str, bool selected)
{
    int height = 11;
    if (selected)
    {
        _u8g.setFont(u8g2_font_fur14_tf);
        height = 14;
    }
    int width = _u8g.getStrWidth(str.c_str());
    _u8g.drawStr(x - width / 2, y + height / 2, str.c_str());
    if (selected)
    {
        _u8g.setFont(u8g2_font_fur11_tf);
    }
}

void Oled::_draw_alarm_set_hour(const State &state)
{
    const String time = _make_time_str(state.get_current_time());

    char hours[] = "__";
    sprintf(hours, "%2d", state.get_custom_alarm().get_hour());
    char minutes[] = ":__";
    sprintf(minutes, ":%02d", state.get_custom_alarm().get_minute());

    _u8g.setContrast(state.get_display_brightness());
    int time_x = _u8g.getDisplayWidth() / 2;
    int time_y = 56;
    _u8g.setFont(u8g2_font_fur30_tn);
    int minutes_x = _u8g.getDisplayWidth() / 2 - 4;
    _u8g.setFont(u8g2_font_fur35_tn);
    int hours_width = _u8g.getStrWidth(hours);
    int hours_x = minutes_x - hours_width;

    _u8g.firstPage();
    do
    {
        _u8g.setFont(u8g2_font_fur35_tn);
        _u8g.drawStr(hours_x, 38, hours);
        _u8g.setFont(u8g2_font_fur30_tn);
        _u8g.drawStr(minutes_x, 38, minutes);
        _draw_item(time_x, time_y, time, true);
    } while (_u8g.nextPage());
}

void Oled::_draw_alarm_set_minute(const State &state)
{
    const String time = _make_time_str(state.get_current_time());

    char hours[] = "__:";
    sprintf(hours, "%d:", state.get_custom_alarm().get_hour());
    char minutes[] = "__";
    sprintf(minutes, "%02d", state.get_custom_alarm().get_minute());

    _u8g.setContrast(state.get_display_brightness());
    int time_x = _u8g.getDisplayWidth() / 2;
    int time_y = 56;
    _u8g.setFont(u8g2_font_fur35_tn);
    int minutes_x = _u8g.getDisplayWidth() / 2 + 4;
    _u8g.setFont(u8g2_font_fur30_tn);
    int hours_x = minutes_x - _u8g.getStrWidth(hours);

    _u8g.firstPage();
    do
    {
        _u8g.setFont(u8g2_font_fur30_tn);
        _u8g.drawStr(hours_x, 38, hours);
        _u8g.setFont(u8g2_font_fur35_tn);
        _u8g.drawStr(minutes_x, 38, minutes);
        _draw_item(time_x, time_y, time, true);
    } while (_u8g.nextPage());
}

String Oled::_make_time_str(const Time &time) const
{
    String data("__:__");
    snprintf_P(data.begin(), data.length()+1, PSTR("%d:%02d"), time.get_hour(), time.get_minute());
    return data;
}
