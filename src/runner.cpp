#include "runner.h"
#include "utils/ms_diff.h"

namespace
{
    static unsigned long off_threshold = 30000;
    const char red_pressed[] = "Red pressed";
}

Runner::Runner() : _step(NORMAL), _last_change(0) {}

void Runner::loop(const Inputs &inputs, State &state)
{
    if (_last_change == 0)
        _last_change = inputs.now_ms();
    state.set_now_ms(inputs.now_ms());
    state.set_current_time(inputs.get_time());
    _trigger_alarm_if_needed(state);
    switch (_step)
    {
    case Step::NORMAL:
        _process_normal(inputs, state);
        break;
    case Step::ALARM_SELECT:
        _process_alarm_select(inputs, state);
        break;
    case Step::ALARM_SET_HOUR:
        _process_alarm_set_hour(inputs, state);
        break;
    case Step::ALARM_SET_MINUTE:
        _process_alarm_set_minute(inputs, state);
        break;
    case Step::ALARM_PLAYING:
        _process_alarm_playing(inputs, state);
        break;
    case Step::NO_DISPLAY:
        _process_off(inputs, state);
        break;
    }
}

void Runner::_process_normal(const Inputs &inputs, State &state)
{
    if (inputs.red_has_been_pressed())
    {
        _change_step(state, Step::ALARM_SELECT, red_pressed);
    }
    else if (inputs.yellow_has_been_pressed())
    {
        _last_change = inputs.now_ms();
    }
    else if (inputs.green_has_been_pressed())
    {
        state.set_alarm_playing(true);
        _change_step(state, Step::ALARM_PLAYING, "Green pressed");
    }
    else if (!state.is_alarm_playing() && ms_diff(_last_change, inputs.now_ms()) > off_threshold)
    {
        unsigned long diff = ms_diff(_last_change, inputs.now_ms());
        String reason = String("No action since ") + String(diff) + String(" ms");
        _change_step(state, Step::NO_DISPLAY, reason.c_str());
    }
}

void Runner::_process_alarm_select(const Inputs &inputs, State &state)
{
    int alarm_index = state.get_alarm_index();
    if (inputs.red_has_been_pressed())
    {
        if (alarm_index == NO_ALARM)
        {
            state.switch_off_alarm();
            _change_step(state, Step::NORMAL, red_pressed);
        }
        else if (alarm_index == CUSTOM_ALARM)
        {
            _change_step(state, Step::ALARM_SET_HOUR, red_pressed);
        }
        else
        {
            state.set_alarm(state.get_predefine_alarms()[alarm_index]);
            _change_step(state, Step::NORMAL, red_pressed);
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

        state.set_alarm_index(alarm_index);
    }
}

void Runner::_process_alarm_set_hour(const Inputs &inputs, State &state)
{
    if (inputs.red_has_been_pressed())
    {
        _change_step(state, Step::ALARM_SET_MINUTE, red_pressed);
    }
    else if (inputs.yellow_has_been_pressed())
    {
        Time alarm = state.get_custom_alarm();
        state.set_custom_alarm(alarm.add_hour(-1));
    }
    else if (inputs.green_has_been_pressed())
    {
        Time alarm = state.get_custom_alarm();
        state.set_custom_alarm(alarm.add_hour(1));
    }
}

void Runner::_process_alarm_set_minute(const Inputs &inputs, State &state)
{
    if (inputs.red_has_been_pressed())
    {
        state.set_alarm(state.get_custom_alarm());
        _change_step(state, Step::NORMAL, red_pressed);
        _last_change = inputs.now_ms();
    }
    else if (inputs.yellow_has_been_pressed())
    {
        Time alarm = state.get_custom_alarm();
        state.set_custom_alarm(alarm.add_minute(-1));
    }
    else if (inputs.green_has_been_pressed())
    {
        Time alarm = state.get_custom_alarm();
        state.set_custom_alarm(alarm.add_minute(1));
    }
}

void Runner::_process_alarm_playing(const Inputs &inputs, State &state)
{
    if (inputs.red_has_been_pressed() || inputs.yellow_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        state.set_alarm_playing(false);
        _change_step(state, Step::NORMAL, "Red or yellow pressed");
    }
}
void Runner::_process_off(const Inputs &inputs, State &state)
{
    if (inputs.red_has_been_pressed() || inputs.yellow_has_been_pressed() || inputs.green_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _change_step(state, Step::NORMAL, "Button pressed");
    }
}

void Runner::_change_step(State &state, Step step, const char *reason)
{
    Serial.printf("State changed %d", _step);
    Serial.printf(" > %d ", step);
    if (reason)
        Serial.print(reason);
    Serial.print("\n");
    _step = step;
    state.set_step(step);
}

void Runner::_trigger_alarm_if_needed(State &state)
{
    // Allarm is off
    if (!state.is_alarm_on() || state.is_alarm_playing())
        return;
    // Alarm is for the future
    if (state.get_alarm() != state.get_current_time())
        return;
    // Alarm has already been triggered this minute
    if (state.alarm_already_started_this_minute())
        return;
    state.set_alarm_playing(true);
    _change_step(state, Step::ALARM_PLAYING, "Alarm triggered");
}
