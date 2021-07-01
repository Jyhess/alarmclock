#include "alarm_list.h"

AlarmList::AlarmList(Rtc & rtc) : _rtc(rtc), _alarm_index(NO_ALARM)
{
}

void AlarmList::setup()
{
    _rtc.read_alarm(_alarms[0], _alarms[1], _alarms[2], _alarm_index);
    _custom_alarm = _alarms[0];
}

void AlarmList::save_alarm(const Time & alarm)
{
    _custom_alarm = alarm;
    _alarm_index = 0;
    // We use _custom_alarm to prevent a reference of _alarms item
    // which may be modified by reorganisation
    _new_alarm(_custom_alarm);
    _rtc.save_alarm(_alarms[0], _alarms[1], _alarms[2], _alarm_index);
}

void AlarmList::save_no_alarm()
{
    _alarm_index = NO_ALARM;
    _rtc.save_alarm(_alarms[0], _alarms[1], _alarms[2], _alarm_index);
}

void AlarmList::_move_to_front(const Time & alarm, uint8_t index)
{
    for(uint8_t i = index; i > 0; --i)
    {
        _alarms[i] = _alarms[i-1];
    }
    _alarms[0] = alarm;
}

void AlarmList::_new_alarm(const Time & alarm)
{
    for(uint8_t index = 0; index < SAVED_ALARMS; ++index)
    {
        if(alarm == _alarms[index])
        {
            _move_to_front(alarm, index);
            return;
        }
    }
    _move_to_front(alarm, SAVED_ALARMS-1);
}
