#pragma once

#include "step.h"
#include "state.h"
#include "inputs/inputs.h"

class Runner
{
public:
    Runner();
    void loop(const Inputs &inputs, State &state);

private:
    Step _step;
    long _last_change;

    void _process_normal(const Inputs &inputs, State &state);
    void _process_alarm_select(const Inputs &inputs, State &state);
    void _process_alarm_on_off(const Inputs &inputs, State &state);
    void _process_alarm_set_hour(const Inputs &inputs, State &state);
    void _process_alarm_set_minute(const Inputs &inputs, State &state);
    void _process_alarm_playing(const Inputs &inputs, State &state);
    void _process_off(const Inputs &inputs, State &state);

    void _change_step(State &state, Step step, const char * reason = nullptr);
    void _trigger_alarm_if_needed(State &state);
};
