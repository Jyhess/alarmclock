#include "alarm_list.h"

AlarmList::AlarmList(Rtc & rtc) : _rtc(rtc), _alarm_index(NO_ALARM)
{
    _alarms[0] = Time(6, 30);
    _alarms[1] = Time(7, 0);
    _alarms[2] = Time(7, 30);
    _alarms[3] = Time(8, 0);
    _alarms[4] = Time(8, 30);
    _alarms[5] = Time(9, 0);
}

void AlarmList::setup()
{
    _rtc.read_alarm(_custom_alarm, _alarm_index);
}

void AlarmList::set_custom_alarm(const Time &value)
{
    _custom_alarm = value;
    _rtc.save_alarm(_custom_alarm, _alarm_index);
}

void AlarmList::set_alarm_index(uint8_t value)
{
    _alarm_index = value;
    _rtc.save_alarm(_custom_alarm, _alarm_index);
}
