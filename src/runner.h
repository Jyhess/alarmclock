#pragma once

#include "state.h"
#include "inputs/inputs.h"
#include "outputs/outputs.h"

//#define DEBUG_STEP

class Runner
{
public:
    explicit Runner(State & state);
    void setup(Inputs &inputs);
    void loop(const Inputs &inputs, const Outputs & outputs);

private:
    State & _state;
    unsigned long _last_change;

    void _process_normal(const Inputs &inputs, const Outputs & outputs);
    void _process_alarm_select(const Inputs &inputs);
    void _process_alarm_on_off(const Inputs &inputs);
    void _process_set_hour(const Inputs &inputs);
    void _process_set_minute(const Inputs &inputs);
    void _process_alarm_playing(const Inputs &inputs);
    void _process_off(const Inputs &inputs);

    void _change_step(
        State::Step step
#ifdef DEBUG_STEP
        ,
        const char *reason = nullptr
#endif
    );
    void _trigger_alarm_if_needed();
};
