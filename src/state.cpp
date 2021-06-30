#include "state.h"

State::State(AlarmList &alarm_list) : _now_ms(0), _need_oled_update(false), _display_brightness(0), _step(NORMAL), _alarm_list(alarm_list)
{
}

bool State::is_updated() const
{
    return _need_oled_update;
}

void State::clear_updated()
{
    _need_oled_update = false;
}
