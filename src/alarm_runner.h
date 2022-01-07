#pragma once

#include "js_time_s.h"
#include "js_range.h"

class AlarmRunner
{
public:
#ifdef DEBUG_ALARM
    static const int16_t alarm_switch_duration_s = 120;
#else
    static const int16_t alarm_switch_duration_s = 30 * 60;
#endif

private:
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
