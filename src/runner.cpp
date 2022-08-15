#include "runner.h"
#include "js_ms_diff.h"

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
    const unsigned int long_press = 500;
    const unsigned int set_time_long_press = 3000;
}

Runner::Runner(State & state) : _state(state), _last_change(0) {}

void Runner::setup(Inputs &inputs)
{
    auto alarm_index = _state.get_alarms().get_alarm_index();
    if (alarm_index == NO_ALARM)
    {
        _state.switch_off_alarm();
    }
    else if (alarm_index == NEW_ALARM)
    {
        _state.set_alarm(_state.get_edit_time());
    }
    else if (alarm_index < SAVED_ALARMS)
    {
        _state.set_alarm(_state.get_alarms().get_alarm(alarm_index));
    }
    else
    {
        // Weired saved data
        _state.switch_off_alarm();
    }
}

void Runner::loop(const Inputs &inputs, const Outputs & outputs)
{
    _state.clear_updated();
    if (_last_change == 0)
        _last_change = inputs.now_ms();
    _state.set_now_ms(inputs.now_ms());
    if( inputs.get_time() != _state.get_current_time() )
    {
        _state.set_current_time(inputs.get_time());
        _trigger_alarm_if_needed();
    }
    _state.set_debug_value(inputs.get_luminosity());
    switch (_state.get_step())
    {
    case State::Step::NORMAL:
        _process_normal(inputs, outputs);
        break;
    case State::Step::ALARM_SELECT:
        _process_alarm_select(inputs);
        break;
    case State::Step::ALARM_SET_HOUR:
    case State::Step::TIME_SET_HOUR:
        _process_set_hour(inputs);
        break;
    case State::Step::ALARM_SET_MINUTE:
    case State::Step::TIME_SET_MINUTE:
        _process_set_minute(inputs);
        break;
    case State::Step::ALARM_PLAYING:
        _process_alarm_playing(inputs);
        break;
    case State::Step::NO_DISPLAY:
        _process_off(inputs);
        break;
    }
}

void Runner::_process_normal(const Inputs &inputs, const Outputs & outputs)
{
    if (inputs.red_has_been_pressed(set_time_long_press))
    {
        CHANGE_STEP(State::Step::ALARM_SELECT, red_pressed);
    }
    else if (inputs.red_long_pressed(set_time_long_press))
    {
        _state.set_edit_time(_state.get_current_time().hm());
        CHANGE_STEP(State::Step::TIME_SET_HOUR, red_pressed);
    }
    else if (inputs.yellow_has_been_pressed() && inputs.green_has_been_pressed())
    {
        _state.start_playing_alarm();
        CHANGE_STEP(State::Step::ALARM_PLAYING, "Alarm manual");
    }
    else if (inputs.yellow_has_been_pressed(200))
    {
        _last_change = inputs.now_ms();
        _state.set_sun_value(0);
    }
    else if (inputs.green_has_been_pressed(200))
    {
        _last_change = inputs.now_ms();
        // When we increase luminosity with single click, we want only 4 different values
        // only red light
        // slight yellow light
        // max yellow light
        // max white light
        const uint8_t sun_steps[] = {10, 50, 180, 255};
        uint8_t sun_value = outputs.sun().get_current_value();
        for (uint8_t i = 0; i < 4; ++i)
        {
            if (sun_value < sun_steps[i])
            {
                sun_value = sun_steps[i];
                break;
            }
        }
        _state.set_sun_value(sun_value);
    }
    else if (inputs.yellow_long_pressed(200))
    {
        _last_change = inputs.now_ms();
        _state.set_sun_value(outputs.sun().get_current_value() - 1);
    }
    else if (inputs.green_long_pressed(200))
    {
        _last_change = inputs.now_ms();
        _state.set_sun_value(outputs.sun().get_current_value() + 1);
    }
    else if(inputs.has_moved())
    {
        _last_change = inputs.now_ms();
    }
    else if (inputs.is_dark())
    {
        if (!_state.is_alarm_playing() && _state.get_sun_value() == 0 && ms_diff(_last_change, inputs.now_ms()) > off_threshold)
        {
            CHANGE_STEP(State::Step::NO_DISPLAY, "No actions in dark");
            //_state.set_updated();
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
    int8_t alarm_index = _state.get_alarm_index();
    if (inputs.red_has_been_pressed())
    {
        if (alarm_index == NO_ALARM)
        {
            _state.switch_off_alarm();
            CHANGE_STEP(State::Step::NORMAL, red_pressed);
        }
        else if (alarm_index == NEW_ALARM)
        {
            _state.set_edit_time(_state.alarms().get_alarm(0));
            CHANGE_STEP(State::Step::ALARM_SET_HOUR, red_pressed);
        }
        else
        {
            _state.set_alarm(_state.get_alarms().get_alarm(alarm_index));
            CHANGE_STEP(State::Step::NORMAL, red_pressed);
        }
    }
    else
    {
        if (inputs.yellow_has_been_pressed())
        {
            if (alarm_index == 0)
                alarm_index = NO_ALARM;
            else
                alarm_index--;
        }
        else if (inputs.green_has_been_pressed())
        {
            if (alarm_index == NO_ALARM)
                alarm_index = 0;
            else
                alarm_index++;
        }
        else
            return;
        _state.set_alarm_index(alarm_index);
    }
}

void Runner::_process_set_hour(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed())
    {
        if(_state.get_step() == State::Step::ALARM_SET_HOUR)
        {
            CHANGE_STEP(State::Step::ALARM_SET_MINUTE, red_pressed);
        }
        else
        {
            CHANGE_STEP(State::Step::TIME_SET_MINUTE, red_pressed);
        }
    }
    else if (inputs.yellow_has_been_pressed() || inputs.yellow_long_pressed(long_press))
    {
        Time alarm = _state.get_edit_time();
        _state.set_edit_time(alarm.add_hour(-1));
    }
    else if (inputs.green_has_been_pressed() || inputs.green_long_pressed(long_press))
    {
        Time alarm = _state.get_edit_time();
        _state.set_edit_time(alarm.add_hour(1));
    }
}

void Runner::_process_set_minute(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed())
    {
        if(_state.get_step() == State::Step::ALARM_SET_MINUTE)
        {
            _state.set_alarm(_state.get_edit_time());
        }
        else
        {
            _state.alarms().save_time(_state.get_edit_time());
            _state.set_current_time(_state.get_edit_time());
        }
        CHANGE_STEP(State::Step::NORMAL, red_pressed);
        _last_change = inputs.now_ms();
    }
    else if (inputs.yellow_has_been_pressed() || inputs.yellow_long_pressed(long_press))
    {
        Time alarm = _state.get_edit_time();
        _state.set_edit_time(alarm.add_minute(-1));
    }
    else if (inputs.green_has_been_pressed() || inputs.green_long_pressed(long_press))
    {
        Time alarm = _state.get_edit_time();
        _state.set_edit_time(alarm.add_minute(1));
    }
}

void Runner::_process_alarm_playing(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _state.switch_off_alarm();
        CHANGE_STEP(State::Step::NORMAL, red_pressed);
    }
    else if (inputs.green_has_been_pressed() || inputs.yellow_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _state.snooze_alarm();
    }
}

void Runner::_process_off(const Inputs &inputs)
{
    if (inputs.red_has_been_pressed() || inputs.yellow_has_been_pressed() || inputs.green_has_been_pressed() )
    {
        _last_change = inputs.now_ms();
        CHANGE_STEP(State::Step::NORMAL, "Button pressed");
    }
    else if (!inputs.is_dark())
    {
        _last_change = inputs.now_ms();
        CHANGE_STEP(State::Step::NORMAL, "Light detected");
    }
    else if (inputs.has_moved())
    {
        _last_change = inputs.now_ms();
        CHANGE_STEP(State::Step::NORMAL, "Vibration detected");
    }
}

void Runner::_change_step(
    State::Step step
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
    _state.set_step(step);
}

void Runner::_trigger_alarm_if_needed()
{
    // Allarm is off
    if (!_state.is_alarm_on() || _state.is_alarm_playing())
        return;
    // Alarm is not for now
    // We start alarm to have 100% at specified time
    if (_state.get_current_time() + Time::from_seconds(AlarmRunner::alarm_switch_duration_s) != _state.get_alarm())
        return;
    _state.start_playing_alarm();
    CHANGE_STEP(State::Step::ALARM_PLAYING, "Alarm triggered");
}
