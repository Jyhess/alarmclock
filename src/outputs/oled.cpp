#include "outputs/oled.h"

namespace
{
    const u8g2_uint_t time_y = 38;
    const u8g2_uint_t alarm_y = 56;
    const uint8_t *font_big = u8g2_font_fur35_tn;
    const uint8_t *font_medium = u8g2_font_fur14_tn;
    const uint8_t *font_small = u8g2_font_fur11_tr;
    const u8g2_uint_t big_font_height = 35;
    const u8g2_uint_t small_font_height = 11;
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
    char time[6];
    _make_time_cstr(time, state.get_current_time().hm());

    char alarm[6] = "";
    if (state.is_alarm_on())
    {
        _make_time_cstr(alarm, state.get_alarm());
    }

    char debug_str[6] = "";
    if( state.get_debug_value().has_value() )
    {
        snprintf_P(debug_str, 6, PSTR("%d"), state.get_debug_value().value());
    }

    char alarm_percent[6] = "";
    if (state.is_alarm_playing())
    {
        snprintf_P(alarm_percent, 6, PSTR("%d"), state.get_alarm_value());
    }
    _u8g.display();
    _u8g.setContrast(state.get_display_brightness());
    _u8g.setFont(font_big);
    const u8g2_uint_t width = _u8g.getDisplayWidth();
    const u8g2_uint_t time_x = (width - _u8g.getStrWidth(time)) / 2;
    const u8g2_uint_t x_1 = 1 * width / 6;
    const u8g2_uint_t x_2 = 3 * width / 6;
    const u8g2_uint_t x_3 = 5 * width / 6;

    _u8g.firstPage();
    do
    {
        _u8g.setFont(font_big);
        _u8g.drawStr(time_x, time_y, time);
        _u8g.setFont(font_medium);
        _draw_item(x_1, alarm_y, debug_str, false);
        _draw_item(x_2, alarm_y, alarm, false);
        _draw_item(x_3, alarm_y, alarm_percent, false);
    } while (_u8g.nextPage());
}

void Oled::_draw_alarm_select(const State &state)
{
    char alarm[6];
    _make_time_cstr(alarm, state.get_alarms().get_custom_alarm());
    char alarms[SAVED_ALARMS][6];
    for (uint8_t i = 0; i < SAVED_ALARMS; ++i)
    {
        _make_time_cstr(alarms[i], state.get_alarms().get_alarm(i));
    }
    const u8g2_uint_t width = _u8g.getDisplayWidth();
    const u8g2_uint_t x_1 = 1 * width / 6;
    const u8g2_uint_t x_2 = 3 * width / 6;
    const u8g2_uint_t x_3 = 5 * width / 6;
    const u8g2_uint_t height = _u8g.getDisplayHeight();
    const u8g2_uint_t y_1 = 1 * height / 6;
    const u8g2_uint_t y_2 = 3 * height / 6;
    const u8g2_uint_t y_3 = 5 * height / 6;

    const uint8_t id = state.get_alarms().get_alarm_index();
    _u8g.setContrast(state.get_display_brightness());
    _u8g.firstPage();
    do
    {
        _u8g.setFont(font_small);
        _draw_item(x_1, y_1, "OFF", id == NO_ALARM);
        _draw_item(x_3, y_1, "NEW", id == NEW_ALARM);
        _draw_item(x_1, y_2, alarms[0], id == 0);
        _draw_item(x_2, y_2, alarms[1], id == 1);
        _draw_item(x_3, y_2, alarms[2], id == 2);
        _draw_item(x_1, y_3, alarms[3], id == 3);
        _draw_item(x_2, y_3, alarms[4], id == 4);
        _draw_item(x_3, y_3, alarms[5], id == 5);
    } while (_u8g.nextPage());
}

void Oled::_draw_item(int x, int y, const char * str, bool contrast)
{
    const u8g2_uint_t height = small_font_height;
    const u8g2_uint_t width = _u8g.getStrWidth(str);
    x -= width / 2;
    y += height / 2;
    if (contrast)
    {
        _u8g.drawBox(x-2, y-height-2, width+4, height+4);
        _u8g.setDrawColor(0);
    }
    _u8g.drawStr(x, y, str);
    if (contrast)
    {
        _u8g.setDrawColor(1);
    }
}

void Oled::_draw_alarm_set(const State &state)
{
    char time[6];
    _make_time_cstr(time, state.get_current_time().hm());

    char hours[] = "__";
    sprintf(hours, "%d", state.get_alarms().get_custom_alarm().get_hour());
    char minutes[] = "__";
    sprintf(minutes, "%02d", state.get_alarms().get_custom_alarm().get_minute());

    _u8g.setFont(font_big);
    _u8g.setContrast(state.get_display_brightness());
    const u8g2_uint_t time_x = _u8g.getDisplayWidth() / 2;
    const u8g2_uint_t time_y_ = _u8g.getDisplayHeight() / 6;
    const u8g2_uint_t collon_x = time_x - _u8g.getStrWidth(":");
    const u8g2_uint_t minutes_x = _u8g.getDisplayWidth() / 2 + 3;
    const u8g2_uint_t hours_width = _u8g.getStrWidth(hours);
    const u8g2_uint_t minutes_width = _u8g.getStrWidth(minutes);
    const u8g2_uint_t hours_x = collon_x - hours_width - 2;
    const u8g2_uint_t alarm_y_ = 60;

    _u8g.firstPage();
    do
    {
        _u8g.setFont(font_big);
        _u8g.drawStr(collon_x, alarm_y_, ":");
        if (state.get_step() == ALARM_SET_HOUR)
        {
            _u8g.drawBox(hours_x-2, alarm_y_-big_font_height-2, hours_width+4, big_font_height+4);
            _u8g.setDrawColor(0);
        }
        _u8g.drawStr(hours_x, alarm_y_, hours);
        if (state.get_step() == ALARM_SET_HOUR)
        {
            _u8g.setDrawColor(1);
        }
        else
        {
            _u8g.drawBox(minutes_x-2, alarm_y_-big_font_height-2, minutes_width+4, big_font_height+4);
            _u8g.setDrawColor(0);
        }
        _u8g.drawStr(minutes_x, alarm_y_, minutes);
        if (state.get_step() != ALARM_SET_HOUR)
        {
            _u8g.setDrawColor(1);
        }
        _u8g.setFont(font_medium);
        _draw_item(time_x, time_y_, time, false);
    } while (_u8g.nextPage());
}

void Oled::_make_time_cstr(char * data, const Time &time)
{
    snprintf_P(data, 6, PSTR("%d:%02d"), time.get_hour(), time.get_minute());
}
