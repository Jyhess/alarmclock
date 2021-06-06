#include "state.h"

State::State() : _now_ms(0), _display_brightness(0), _alarm_index(NO_ALARM), _alarm_playing(false), _sun_percent(0)
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
    return _display_brightness.is_updated() || _time.is_updated() || _alarm.is_updated() || _step.is_updated() || _alarm_index.is_updated() || _custom_alarm.is_updated() || _alarm_playing.is_updated() || _sun_percent.is_updated();
}

void State::clear_updated()
{
    _display_brightness.clear_updated();
    _time.clear_updated();
    _alarm.clear_updated();
    _step.clear_updated();
    _alarm_index.clear_updated();
    _custom_alarm.clear_updated();
    _alarm_playing.clear_updated();
    _sun_percent.clear_updated();
}
