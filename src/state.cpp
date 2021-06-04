#include "state.h"
#include "utils/ms_diff.h"

namespace
{
    static unsigned long off_threshold = 30;
}

State::State() : _step(NORMAL), _last_change(0) {}

void State::setup()
{
}

void State::loop(const Inputs &inputs, Outputs &outputs)
{
    if (_last_change == 0)
        _last_change = inputs.now_ms();
    switch (_step)
    {
    case Step::NORMAL:
        _process_normal(inputs, outputs);
        break;
    case Step::ALARM_SELECT:
        _process_alarm_select(inputs, outputs);
        break;
    case Step::ALARM_SET_HOUR:
        _process_alarm_set_hour(inputs, outputs);
        break;
    case Step::ALARM_SET_MINUTE:
        _process_alarm_set_minute(inputs, outputs);
        break;
    case Step::NO_DISPLAY:
        _process_off(inputs, outputs);
        break;
    }
}

void State::_process_normal(const Inputs &inputs, Outputs &outputs)
{
    if (inputs.red_has_been_pressed())
    {
        _change_step(outputs, Step::ALARM_SELECT);
    }
    else if (inputs.yellow_has_been_pressed() || inputs.green_has_been_pressed())
    {
        _last_change = inputs.now_ms();
    }
    else if (ms_diff(_last_change, inputs.now_ms()) > off_threshold)
    {
        _change_step(outputs, Step::NO_DISPLAY);
    }
}

void State::_process_alarm_select(const Inputs &inputs, Outputs &outputs)
{
    int alarm_index = outputs.get_alarm_index();
    if (inputs.red_has_been_pressed())
    {
        if (alarm_index == NO_ALARM)
        {
            outputs.switch_off_alarm();
            _change_step(outputs, Step::NORMAL);
        }
        else if (alarm_index == CUSTOM_ALARM)
        {
            _change_step(outputs, Step::ALARM_SET_HOUR);
        }
        else
        {
            outputs.set_alarm(outputs.get_predefine_alarms()[alarm_index]);
            _change_step(outputs, Step::NORMAL);
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

        outputs.set_alarm_index(alarm_index);
    }
}

void State::_process_alarm_set_hour(const Inputs &inputs, Outputs &outputs)
{
    if (inputs.red_has_been_pressed())
    {
        _change_step(outputs, Step::ALARM_SET_MINUTE);
    }
    else if (inputs.yellow_has_been_pressed())
    {
        Time alarm = outputs.get_custom_alarm();
        outputs.set_custom_alarm(alarm.add_hour(-1));
    }
    else if (inputs.green_has_been_pressed())
    {
        Time alarm = outputs.get_custom_alarm();
        outputs.set_custom_alarm(alarm.add_hour(1));
    }
}

void State::_process_alarm_set_minute(const Inputs &inputs, Outputs &outputs)
{
    if (inputs.red_has_been_pressed())
    {
        outputs.set_alarm(outputs.get_custom_alarm());
        _change_step(outputs, Step::NORMAL);
        _last_change = inputs.now_ms();
    }
    else if (inputs.yellow_has_been_pressed())
    {
        Time alarm = outputs.get_custom_alarm();
        outputs.set_custom_alarm(alarm.add_minute(-1));
    }
    else if (inputs.green_has_been_pressed())
    {
        Time alarm = outputs.get_custom_alarm();
        outputs.set_custom_alarm(alarm.add_minute(1));
    }
}

void State::_process_off(const Inputs &inputs, Outputs &outputs)
{
    if (inputs.red_has_been_pressed() || inputs.yellow_has_been_pressed() || inputs.green_has_been_pressed())
    {
        _last_change = inputs.now_ms();
        _change_step(outputs, Step::NORMAL);
    }
}

void State::_change_step(Outputs &outputs, Step step)
{
    _step = step;
    outputs.set_step(step);
}
