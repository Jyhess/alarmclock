#include "outputs/oled.h"

namespace
{
    const u8g2_uint_t time_y = 38;
    const u8g2_uint_t alarm_y = 56;
    const uint8_t *font_big = u8g2_font_fur30_tn;
    const uint8_t *font_big_select = u8g2_font_fur35_tn;
    const uint8_t *font_small = u8g2_font_fur11_tn;
    const uint8_t *font_small_select = u8g2_font_fur14_tn;
}

#ifdef OLED_I2C
Oled::Oled() : _u8g(U8G2_R0)
#else
Oled::Oled(uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset) : _u8g(U8G2_R0, clock, data, cs, dc, reset)
#endif
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
        case Step::ALARM_SET_MINUTE:
            _draw_alarm_set(state);
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
        //alarm = String("> ") + alarm + String(" <");
        alarm = alarm + String(" > ") + String(state.get_alarm_percent()) + String("%");
    }
    _u8g.display();
    _u8g.setContrast(state.get_display_brightness());
    const u8g2_uint_t alarm_x = _u8g.getDisplayWidth() / 2;
    _u8g.setFont(font_big_select);
    const u8g2_uint_t x = (_u8g.getDisplayWidth() - _u8g.getStrWidth(time.c_str())) / 2;

    _u8g.firstPage();
    do
    {
        _u8g.setFont(font_big_select);
        _u8g.drawStr(x, time_y, time.c_str());
        _draw_item(alarm_x, alarm_y, alarm, true);
    } while (_u8g.nextPage());
}

void Oled::_draw_alarm_select(const State &state)
{
    const String alarm = _make_time_str(state.get_custom_alarm());
    Array<String, PREDEFINED_ALARMS> alarms;
    for (uint8_t i = 0; i < PREDEFINED_ALARMS; ++i)
    {
        alarms[i] = _make_time_str(state.get_predefine_alarm(i));
    }
    const u8g2_uint_t width = _u8g.getDisplayWidth();
    const u8g2_uint_t x_1 = 1 * width / 6;
    const u8g2_uint_t x_2 = 3 * width / 6;
    const u8g2_uint_t x_3 = 5 * width / 6;
    const u8g2_uint_t height = _u8g.getDisplayHeight();
    const u8g2_uint_t y_1 = 1 * height / 6;
    const u8g2_uint_t y_2 = 3 * height / 6;
    const u8g2_uint_t y_3 = 5 * height / 6;

    const uint8_t id = state.get_alarm_index();
    _u8g.firstPage();
    do
    {
        _u8g.setContrast(state.get_display_brightness());
        _u8g.setFont(font_small);
        _draw_item(x_1, y_1, "--:--", id == NO_ALARM);
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
    u8g2_uint_t height = 11;
    if (selected)
    {
        _u8g.setFont(font_small_select);
        height = 14;
    }
    const u8g2_uint_t width = _u8g.getStrWidth(str.c_str());
    _u8g.drawStr(x - width / 2, y + height / 2, str.c_str());
    if (selected)
    {
        _u8g.setFont(font_small);
    }
}

void Oled::_draw_alarm_set(const State &state)
{
    const String time = _make_time_str(state.get_current_time());

    char hours[] = "__";
    sprintf(hours, "%2d", state.get_custom_alarm().get_hour());
    char minutes[] = "__";
    sprintf(minutes, "%02d", state.get_custom_alarm().get_minute());

    const uint8_t *font_hour = font_big;
    const uint8_t *font_minute = font_big_select;
    const u8g2_uint_t time_x = _u8g.getDisplayWidth() / 2;
    const u8g2_uint_t time_y_ = _u8g.getDisplayHeight() / 6;
    _u8g.setFont(font_big);
    const u8g2_uint_t collon_x = time_x - _u8g.getStrWidth(":");
    const u8g2_uint_t minutes_x = _u8g.getDisplayWidth() / 2 + 3;
    if (state.get_step() == ALARM_SET_HOUR)
    {
        font_hour = font_big_select;
        font_minute = font_big;
    }
    _u8g.setContrast(state.get_display_brightness());
    _u8g.setFont(font_hour);
    const u8g2_uint_t hours_width = _u8g.getStrWidth(hours);
    const u8g2_uint_t hours_x = collon_x - hours_width - 2;
    const u8g2_uint_t alarm_y_ = 60;

    _u8g.firstPage();
    do
    {
        _u8g.setFont(font_big);
        _u8g.drawStr(collon_x, alarm_y_, ":");
        _u8g.setFont(font_hour);
        _u8g.drawStr(hours_x, alarm_y_, hours);
        _u8g.setFont(font_minute);
        _u8g.drawStr(minutes_x, alarm_y_, minutes);
        _draw_item(time_x, time_y_, time, true);
    } while (_u8g.nextPage());
}

String Oled::_make_time_str(const Time &time) const
{
    String data("__:__");
    snprintf_P(data.begin(), data.length() + 1, PSTR("%d:%02d"), time.get_hour(), time.get_minute());
    return data;
}
