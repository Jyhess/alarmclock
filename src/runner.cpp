#include "runner.h"
#include "utils/ms_diff.h"

namespace
{
    static unsigned long off_threshold = 30000;
    const char red_pressed[] = "Red pressed";
    unsigned long long_press = 1000;
}

Runner::Runner() : _step(NORMAL), _last_change(0) {}

void Runner::loop(const Inputs &inputs)
{
    _state.clear_updated();
    if (_last_change == 0)
        _last_change = inputs.now_ms();
    _state.set_now_ms(inputs.now_ms());
    _state.set_current_time(inputs.get_time());
    _trigger_alarm_if_needed();
    switch (_step)
    {
    case Step::NORMAL:
        _process_normal(inputs);
        break;
    case Step::ALARM_SELECT:
        _process_alarm_select(inputs);
        break;
    case Step::ALARM_SET_HOUR:
        _process_alarm_set_hour(inputs);
        break;
    case Step::ALARM_SET_MINUTE:
        _process_alarm_set_minute(inputs);
        break;
    case Step::ALARM_PLAYING:
        _process_alarm_playing(inputs);
        break;
    case Step::NO_DISPLAY:
        _process_off(inputs);
        break;
    }
}

void Runner::_process_normal(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed())
    {
        _change_step(Step::ALARM_SELECT, red_pressed);
    }
    else if (inputs.yellow_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _state.set_sun_percent(0);
    }
    else if (inputs.green_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _state.set_sun_percent(_state.get_sun_percent() + 10);
    }
    else if (inputs.yellow_long_pressed(100))
    {
        _last_change = inputs.now_ms();
        _state.set_sun_percent(_state.get_sun_percent() - 1);
    }
    else if (inputs.green_long_pressed(100))
    {
        _last_change = inputs.now_ms();
        _state.set_sun_percent(_state.get_sun_percent() + 1);
    }
    else if (!_state.is_alarm_playing() && _state.get_sun_percent() == 0 && ms_diff(_last_change, inputs.now_ms()) > off_threshold)
    {
        unsigned long diff = ms_diff(_last_change, inputs.now_ms());
        String reason = String("No action since ") + String(diff) + String(" ms");
        _change_step(Step::NO_DISPLAY, reason.c_str());
    }
}

void Runner::_process_alarm_select(const Inputs &inputs)
{
    int alarm_index = _state.get_alarm_index();
    if (inputs.red_has_been_pressed())
    {
        if (alarm_index == NO_ALARM)
        {
            _state.switch_off_alarm();
            _change_step(Step::NORMAL, red_pressed);
        }
        else if (alarm_index == CUSTOM_ALARM)
        {
            _change_step(Step::ALARM_SET_HOUR, red_pressed);
        }
        else
        {
            _state.set_alarm(_state.get_predefine_alarms()[alarm_index]);
            _change_step(Step::NORMAL, red_pressed);
        }
    }
    else
    {
        if (inputs.yellow_has_been_pressed())
            alarm_index--;
        else if (inputs.green_has_been_pressed())
            alarm_index++;
        else
            return;

        if (alarm_index >= PREDEFINED_ALARMS)
            alarm_index = NO_ALARM;
        else if (alarm_index < NO_ALARM)
            alarm_index = PREDEFINED_ALARMS - 1;

        _state.set_alarm_index(alarm_index);
    }
}

void Runner::_process_alarm_set_hour(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed())
    {
        _change_step(Step::ALARM_SET_MINUTE, red_pressed);
    }
    else if (inputs.yellow_has_been_pressed())
    {
        Time alarm = _state.get_custom_alarm();
        _state.set_custom_alarm(alarm.add_hour(-1));
    }
    else if (inputs.green_has_been_pressed())
    {
        Time alarm = _state.get_custom_alarm();
        _state.set_custom_alarm(alarm.add_hour(1));
    }
    else if (inputs.yellow_long_pressed(long_press))
    {
        Time alarm = _state.get_custom_alarm();
        _state.set_custom_alarm(alarm.add_hour(-1));
    }
    else if (inputs.green_long_pressed(long_press))
    {
        Time alarm = _state.get_custom_alarm();
        _state.set_custom_alarm(alarm.add_hour(1));
    }
}

void Runner::_process_alarm_set_minute(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed())
    {
        _state.set_alarm(_state.get_custom_alarm());
        _change_step(Step::NORMAL, red_pressed);
        _last_change = inputs.now_ms();
    }
    else if (inputs.yellow_has_been_pressed())
    {
        Time alarm = _state.get_custom_alarm();
        _state.set_custom_alarm(alarm.add_minute(-1));
    }
    else if (inputs.green_has_been_pressed())
    {
        Time alarm = _state.get_custom_alarm();
        _state.set_custom_alarm(alarm.add_minute(1));
    }
    else if (inputs.yellow_long_pressed(long_press))
    {
        Time alarm = _state.get_custom_alarm();
        _state.set_custom_alarm(alarm.add_minute(-1));
    }
    else if (inputs.green_long_pressed(long_press))
    {
        Time alarm = _state.get_custom_alarm();
        _state.set_custom_alarm(alarm.add_minute(1));
    }
}

void Runner::_process_alarm_playing(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed() || inputs.yellow_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _state.set_alarm_playing(false);
        _change_step(Step::NORMAL, "Red or yellow pressed");
    }
}

void Runner::_process_off(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed() || inputs.yellow_has_been_pressed() || inputs.green_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _change_step(Step::NORMAL, "Button pressed");
    }
}

void Runner::_change_step(Step step, const char *reason)
{
    Serial.printf("State changed %d", _step);
    Serial.printf(" > %d ", step);
    if (reason)
        Serial.print(reason);
    Serial.print("\n");
    _step = step;
    _state.set_step(step);
}

void Runner::_trigger_alarm_if_needed()
{
    // Allarm is off
    if (!_state.is_alarm_on() || _state.is_alarm_playing())
        return;
    // Alarm is for the future
    if (_state.get_alarm() != _state.get_current_time())
        return;
    // Alarm has already been triggered this minute
    if (_state.alarm_already_started_this_minute())
        return;
    _state.set_alarm_playing(true);
    _change_step(Step::ALARM_PLAYING, "Alarm triggered");
}
