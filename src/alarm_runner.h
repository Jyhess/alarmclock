#pragma once

#include "utils/time_s.h"
#include "utils/range.h"

class AlarmRunner
{
    TimeS _alarm_start_time;
    Range<uint8_t, 0, 255> _alarm_value;
    TimeS _snooze_time;
    uint8_t _snooze_value;

public:
    constexpr AlarmRunner(): _snooze_value(0) {}
    AlarmRunner(const TimeS &start_time) : _alarm_start_time(start_time), _snooze_value(0) {}

    void snooze_alarm(const TimeS &current_time);
    inline const TimeS &get_snooze_time() const { return _snooze_time; }
    inline bool is_snooze_alarm(const TimeS &current_time) const { return _snooze_time > current_time; }

    uint8_t get_alarm_value(const TimeS &current_time) const;
};
