#include "runner.h"
#include "utils/ms_diff.h"

#ifdef DEBUG_STEP
#define CHANGE_STEP(step, label) _change_step(step, label);
#else
#define CHANGE_STEP(step, label) _change_step(step);
#endif

namespace
{
    const unsigned int off_threshold = 30000;
#ifdef DEBUG_STEP
    const char red_pressed[] = "Red pressed";
#endif
    const unsigned int long_press = 1000;
}

Runner::Runner(State & state) : _state(state), _step(NORMAL), _last_change(0) {}

void Runner::setup(Inputs &inputs)
{
    auto alarm_index = _state.get_alarms().get_alarm_index();
    if (alarm_index == NO_ALARM)
    {
        _state.switch_off_alarm();
    }
    else if (alarm_index == CUSTOM_ALARM)
    {
        _state.set_alarm(_state.get_alarms().get_custom_alarm());
    }
    else if (alarm_index < PREDEFINED_ALARMS)
    {
        _state.set_alarm(_state.get_alarms().get_alarm(alarm_index));
    }
    else
    {
        // Weired saved data
        _state.switch_off_alarm();
    }
}

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
        CHANGE_STEP(Step::ALARM_SELECT, red_pressed);
    }
    else if (inputs.yellow_has_been_pressed() && inputs.green_has_been_pressed())
    {
        _state.start_playing_alarm();
        CHANGE_STEP(Step::ALARM_PLAYING, "Alarm manual");
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
    else if (inputs.is_dark())
    {
        if (!_state.is_alarm_playing() && _state.get_sun_percent() == 0 && ms_diff(_last_change, inputs.now_ms()) > off_threshold)
        {
            CHANGE_STEP(Step::NO_DISPLAY, "No actions in dark");
        }
        else
        {
            _state.set_display_brightness(0);
        }
    }
    else
    {
        // No scrren off in light
        _last_change = inputs.now_ms();
        _state.set_display_brightness(255);
    }
}

void Runner::_process_alarm_select(const Inputs &inputs)
{
    int8_t alarm_index = _state.get_alarms().get_alarm_index();
    if (inputs.red_has_been_pressed())
    {
        if (alarm_index == NO_ALARM)
        {
            _state.switch_off_alarm();
            _state.alarms().set_alarm_index(alarm_index);
            CHANGE_STEP(Step::NORMAL, red_pressed);
        }
        else if (alarm_index == CUSTOM_ALARM)
        {
            CHANGE_STEP(Step::ALARM_SET_HOUR, red_pressed);
        }
        else
        {
            _state.set_alarm(_state.get_alarms().get_alarm(alarm_index));
            _state.alarms().set_alarm_index(alarm_index);
            CHANGE_STEP(Step::NORMAL, red_pressed);
        }
    }
    else
    {
        if (inputs.yellow_has_been_pressed())
        {
            if (alarm_index == 0)
                alarm_index = CUSTOM_ALARM;
            else
                alarm_index--;
        }
        else if (inputs.green_has_been_pressed())
        {
            if (alarm_index == CUSTOM_ALARM)
                alarm_index = 0;
            else
                alarm_index++;
        }
        else
            return;
        _state.alarms().set_alarm_index(alarm_index);
    }
}

void Runner::_process_alarm_set_hour(const Inputs &inputs)
{
    AlarmList & alarms = _state.alarms();
    if (inputs.red_has_been_pressed())
    {
        CHANGE_STEP(Step::ALARM_SET_MINUTE, red_pressed);
    }
    else if (inputs.yellow_has_been_pressed())
    {
        Time alarm = alarms.get_custom_alarm();
        alarms.set_custom_alarm(alarm.add_hour(-1));
    }
    else if (inputs.green_has_been_pressed())
    {
        Time alarm = alarms.get_custom_alarm();
        alarms.set_custom_alarm(alarm.add_hour(1));
    }
    else if (inputs.yellow_long_pressed(long_press))
    {
        Time alarm = alarms.get_custom_alarm();
        alarms.set_custom_alarm(alarm.add_hour(-1));
    }
    else if (inputs.green_long_pressed(long_press))
    {
        Time alarm = alarms.get_custom_alarm();
        alarms.set_custom_alarm(alarm.add_hour(1));
    }
}

void Runner::_process_alarm_set_minute(const Inputs &inputs)
{
    AlarmList & alarms = _state.alarms();
    if (inputs.red_has_been_pressed())
    {
        _state.set_alarm(alarms.get_custom_alarm());
        alarms.set_custom_alarm(alarms.get_custom_alarm());
        CHANGE_STEP(Step::NORMAL, red_pressed);
        _last_change = inputs.now_ms();
    }
    else if (inputs.yellow_has_been_pressed())
    {
        Time alarm = alarms.get_custom_alarm();
        alarms.set_custom_alarm(alarm.add_minute(-1));
    }
    else if (inputs.green_has_been_pressed())
    {
        Time alarm = alarms.get_custom_alarm();
        alarms.set_custom_alarm(alarm.add_minute(1));
    }
    else if (inputs.yellow_long_pressed(long_press))
    {
        Time alarm = alarms.get_custom_alarm();
        alarms.set_custom_alarm(alarm.add_minute(-1));
    }
    else if (inputs.green_long_pressed(long_press))
    {
        Time alarm = alarms.get_custom_alarm();
        alarms.set_custom_alarm(alarm.add_minute(1));
    }
}

void Runner::_process_alarm_playing(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _state.switch_off_alarm();
        CHANGE_STEP(Step::NORMAL, red_pressed);
    }
    else if (inputs.green_has_been_pressed() || inputs.yellow_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _state.snooze_alarm();
    }
}

void Runner::_process_off(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed() || inputs.yellow_has_been_pressed() || inputs.green_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        CHANGE_STEP(Step::NORMAL, "Button pressed");
    }
    else if (!inputs.is_dark())
    {
        _last_change = inputs.now_ms();
        CHANGE_STEP(Step::NORMAL, "Light detected");
    }
}

void Runner::_change_step(
    Step step
#ifdef DEBUG_STEP
    ,
    const char *reason
#endif
)
{
#ifdef DEBUG_STEP
    Serial.printf("State changed %d", _step);
    Serial.printf(" > %d ", step);
    if (reason)
        Serial.print(reason);
    Serial.print("\n");
#endif
    _step = step;
    _state.set_step(step);
}

void Runner::_trigger_alarm_if_needed()
{
    // Allarm is off
    if (!_state.is_alarm_on() || _state.is_alarm_playing())
        return;
    // Alarm is for the future
    if (_state.get_current_time() != _state.get_alarm())
        return;
    _state.start_playing_alarm();
    CHANGE_STEP(Step::ALARM_PLAYING, "Alarm triggered");
}
