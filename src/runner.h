#pragma once

#include "step.h"
#include "state.h"
#include "inputs/inputs.h"
#include "state.h"

class Runner
{
public:
    Runner();
    void loop(const Inputs &inputs);

    inline const State &state() const { return _state; }

private:
    State _state;
    Step _step;
    unsigned long _last_change;

    void _process_normal(const Inputs &inputs);
    void _process_alarm_select(const Inputs &inputs);
    void _process_alarm_on_off(const Inputs &inputs);
    void _process_alarm_set_hour(const Inputs &inputs);
    void _process_alarm_set_minute(const Inputs &inputs);
    void _process_alarm_playing(const Inputs &inputs);
    void _process_off(const Inputs &inputs);

    void _change_step(Step step, const char *reason = nullptr);
    void _trigger_alarm_if_needed();
};
