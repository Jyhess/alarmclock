#pragma once

#include <stdint.h>

#include "utils/time.h"
#include "utils/optional.h"
#include "utils/array.h"
#include "inputs/rtc.h"
#include "alarm_runner.h"

#define PREDEFINED_ALARMS 6
#define NO_ALARM PREDEFINED_ALARMS
#define CUSTOM_ALARM PREDEFINED_ALARMS + 1

class AlarmList
{
    Rtc &_rtc;
    Time _custom_alarm;
    typedef Array<Time, PREDEFINED_ALARMS> Alarms;
    Alarms _alarms;
    // Off = 6
    // Custom = 7
    // Predefined = [0,PREDEFINED_ALARMS[
    uint8_t _alarm_index;

public:
    AlarmList(Rtc & rtc);
    void setup();

    inline const Time &get_custom_alarm() const { return _custom_alarm; }
    void set_custom_alarm(const Time &value);

    inline const Time &get_alarm(uint8_t alarm_index) const { return _alarms[alarm_index]; }

    inline uint8_t get_alarm_index() const { return _alarm_index; }
    void set_alarm_index(uint8_t value);
};
