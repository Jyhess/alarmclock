#include "alarm_runner.h"
#include <Arduino.h>

namespace
{
#ifdef DEBUG_ALARM
    const int16_t snooze_duration_s = 10;
#else
    const int16_t snooze_duration_s = 10 * 60;
#endif
}

void AlarmRunner::snooze_alarm(const TimeS &current_time)
{
    if( is_snooze_alarm(current_time) )
        return;
    // Fix percent at half of previous value for snooze duration
    _snooze_value = get_alarm_value(current_time) / 2;
    _snooze_time = current_time + TimeS().add_hms(0, 0, snooze_duration_s);
    // Change start time to restart increasing from half value
    //auto seconds_elapsed = (current_time - _alarm_start_time).to_seconds();
    _alarm_start_time.add_hms(0, 0, snooze_duration_s);
#ifdef DEBUG_ALARM
    Serial.printf("_snooze_time=%d", _snooze_time.get_hour());
    Serial.printf(":%d", _snooze_time.get_minute());
    Serial.printf(":%d", _snooze_time.get_second());
    Serial.printf(" _alarm_start_time=%d", _alarm_start_time.get_hour());
    Serial.printf(":%d", _alarm_start_time.get_minute());
    Serial.printf(":%d", _alarm_start_time.get_second());
    Serial.printf(" _snooze_value=%d", _snooze_value);
    Serial.println("");
#endif
}

uint8_t AlarmRunner::get_alarm_value(const TimeS &current_time) const
{
    if (is_snooze_alarm(current_time))
        return _snooze_value;
    auto seconds_elapsed = (current_time - _alarm_start_time).to_seconds();
#ifdef DEBUG_ALARM
    Serial.printf("current_time=%d", current_time.get_hour());
    Serial.printf(":%d", current_time.get_minute());
    Serial.printf(":%d", current_time.get_second());
    Serial.printf(" _alarm_start_time=%d", _alarm_start_time.get_hour());
    Serial.printf(":%d", _alarm_start_time.get_minute());
    Serial.printf(":%d", _alarm_start_time.get_second());
    Serial.printf(" seconds_elapsed=%d", seconds_elapsed);
#endif
    uint8_t value = 255;
    if (seconds_elapsed < alarm_switch_duration_s)
        value = seconds_elapsed * 255 / alarm_switch_duration_s;
#ifdef DEBUG_ALARM
    Serial.printf(" value=%d", value);
    Serial.println("");
#endif
    return value;
}
