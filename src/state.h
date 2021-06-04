#pragma once

#include "step.h"
#include "inputs.h"
#include "outputs.h"

class State
{
public:
    State();
    void setup(long now_ms);
    void loop(const Inputs &inputs, Outputs &outputs);

private:
    Step _step;
    long _last_change;

    void _process_normal(const Inputs &inputs, Outputs &outputs);
    void _process_alarm_select(const Inputs &inputs, Outputs &outputs);
    void _process_alarm_on_off(const Inputs &inputs, Outputs &outputs);
    void _process_alarm_set_hour(const Inputs &inputs, Outputs &outputs);
    void _process_alarm_set_minute(const Inputs &inputs, Outputs &outputs);
    void _process_off(const Inputs &inputs, Outputs &outputs);

    void _change_step(Outputs &outputs, Step step);
};
