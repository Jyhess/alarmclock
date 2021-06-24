#pragma once

#include <stdint.h>

#include "step.h"
#include "utils/time.h"
#include "utils/optional.h"
#include "utils/array.h"
#include "utils/range.h"
#include "alarm_runner.h"

#define PREDEFINED_ALARMS 6
#define NO_ALARM PREDEFINED_ALARMS
#define CUSTOM_ALARM PREDEFINED_ALARMS + 1

class State
{
private:
    unsigned long _now_ms;
    bool _need_oled_update;

    uint8_t _display_brightness;
    TimeS _time;
    Optional<Time> _alarm;
    Step _step;

    Time _custom_alarm;
    typedef Array<Time, PREDEFINED_ALARMS> Alarms;
    Alarms _alarms;
    // Off = 6
    // Custom = 7
    // Predefined = [0,PREDEFINED_ALARMS[
    uint8_t _alarm_index;

    Optional<AlarmRunner> _alarm_runner;

    Range<int8_t, 0, 100> _sun_percent;

public:
    State();

    bool is_updated() const;
    void clear_updated();

    inline unsigned long now_ms() const { return _now_ms; }
    inline void set_now_ms(long value) { _now_ms = value; }

    inline const Step &get_step() const { return _step; }
    inline void set_step(const Step &value)
    {
        _step = value;
        _need_oled_update = true;
    }

    inline uint8_t get_display_brightness() const { return _display_brightness; }
    inline void set_display_brightness(uint8_t value)
    {
        _display_brightness = value;
        _need_oled_update = true;
    }

    inline uint8_t get_sun_percent() const { return *_sun_percent; }
    inline void set_sun_percent(uint8_t value) { _sun_percent = value; }

    inline const TimeS &get_current_time() const { return _time; }
    inline void set_current_time(const TimeS &value)
    {
        _time = value;
        _need_oled_update = true;
    }

    inline bool is_alarm_on() const { return _alarm.has_value(); }
    inline const Time &get_alarm() const { return _alarm.value(); }
    inline void set_alarm(const Time &value)
    {
        _alarm = value;
        _need_oled_update = true;
    }
    inline void switch_off_alarm()
    {
        _alarm.reset();
        _alarm_runner.reset();
    }

    inline const Time &get_custom_alarm() const { return _custom_alarm; }
    inline void set_custom_alarm(const Time &value)
    {
        _custom_alarm = value;
        _need_oled_update = true;
    }

    const Alarms &get_predefine_alarms() const { return _alarms; }
    inline const Time &get_predefine_alarm(uint8_t alarm_index) const { return _alarms[alarm_index]; }
    inline uint8_t get_alarm_index() const { return _alarm_index; }
    inline void set_alarm_index(uint8_t value)
    {
        _alarm_index = value;
        _need_oled_update = true;
    }

    inline bool is_alarm_playing() const { return _alarm_runner.has_value(); }
    inline uint8_t get_alarm_percent() const { return _alarm_runner->get_alarm_percent(_time); }

    inline void start_playing_alarm() { _alarm_runner = AlarmRunner(_time); }
    inline void snooze_alarm() { _alarm_runner->snooze_alarm(_time); }
};
