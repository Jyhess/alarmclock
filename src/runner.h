#pragma once

#include "state.h"
#include "inputs/inputs.h"

//#define DEBUG_STEP

class Runner
{
public:
    Runner(State & state);
    void setup(Inputs &inputs);
    void loop(const Inputs &inputs);

private:
    State & _state;
    unsigned long _last_change;

    void _process_normal(const Inputs &inputs);
    void _process_alarm_select(const Inputs &inputs);
    void _process_alarm_on_off(const Inputs &inputs);
    void _process_alarm_set_hour(const Inputs &inputs);
    void _process_alarm_set_minute(const Inputs &inputs);
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
