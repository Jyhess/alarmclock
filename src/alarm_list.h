#pragma once

#include <stdint.h>

#include "utils/time.h"
#include "inputs/rtc.h"

#define SAVED_ALARMS 6
#define NO_ALARM SAVED_ALARMS
#define NEW_ALARM SAVED_ALARMS + 1

class AlarmList
{
    Rtc &_rtc;
    Time _alarms[SAVED_ALARMS];
    // Off = 6
    // New = 7
    // Predefined = [0,SAVED_ALARMS[
    uint8_t _alarm_index;

public:
    AlarmList(Rtc & rtc);
    void setup();

    void save_alarm(const Time & value);
    void save_no_alarm();
    inline const Time &get_alarm(uint8_t alarm_index) const { return _alarms[alarm_index]; }

    inline uint8_t get_alarm_index() const { return _alarm_index; }
    inline void set_alarm_index(uint8_t value) { _alarm_index = value; }

    void save_time(const Time & value);
private:
    void _move_to_front(const Time & alarm, uint8_t index);
    void _new_alarm(const Time & alarm);
};
