#include "state.h"

State::State() : _now_ms(0), _need_oled_update(false), _display_brightness(0), _alarm_index(NO_ALARM), _sun_percent(0)
{
    _alarms[0] = Time(6, 30);
    _alarms[1] = Time(7, 0);
    _alarms[2] = Time(7, 30);
    _alarms[3] = Time(8, 0);
    _alarms[4] = Time(8, 30);
    _alarms[5] = Time(9, 0);
}

bool State::is_updated() const
{
    return _need_oled_update;
}

void State::clear_updated()
{
    _need_oled_update = false;
}
