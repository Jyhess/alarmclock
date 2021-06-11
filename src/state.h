#pragma once

#include <stdint.h>

#include "step.h"
#include "utils/time.h"
#include "utils/updatable.h"
#include "utils/optional.h"
#include "utils/array.h"
#include "utils/range.h"

#define NO_ALARM -2
#define CUSTOM_ALARM -1
#define PREDEFINED_ALARMS 6

class State
{
private:
    unsigned long _now_ms;
    bool _need_oled_update;

    uint8_t _display_brightness;
    Time _time;
    Optional<Time> _alarm;
    Step _step;

    Time _custom_alarm;
    typedef Array<Time, PREDEFINED_ALARMS> Alarms;
    Alarms _alarms;
    // Off = -2
    // Custom = -1
    // Predefined = [0,PREDEFINED_ALARMS[
    int8_t _alarm_index;

    Optional<unsigned long> _alarm_start_time;
    Optional<Range<uint8_t, 0, 100>> _alarm_percent;
    Optional<Time> _last_alarm;

    Range<uint8_t, 0, 100> _sun_percent;

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

    inline const Time &get_current_time() const { return _time; }
    inline void set_current_time(const Time &value)
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
    inline void switch_off_alarm() { _alarm = Optional<Time>(); }

    inline const Time &get_custom_alarm() const { return _custom_alarm; }
    inline void set_custom_alarm(const Time &value)
    {
        _custom_alarm = value;
        _need_oled_update = true;
    }

    const Alarms &get_predefine_alarms() const { return _alarms; }
    inline int8_t get_alarm_index() const { return _alarm_index; }
    inline void set_alarm_index(int8_t value)
    {
        _alarm_index = value;
        _need_oled_update = true;
    }

    inline bool is_alarm_playing() const { return _alarm_start_time.has_value(); }
    inline uint8_t get_alarm_percent() const { return _alarm_percent->value(); }
    inline void set_alarm_percent(uint8_t percent) { _alarm_percent = percent; }

    inline unsigned long get_alarm_start_time() const { return *_alarm_start_time; }
    inline void start_playing_alarm()
    {
        _alarm_start_time = _now_ms;
        _last_alarm = _time;
        _need_oled_update = true;
        set_alarm_percent(0);
        // TODO: Where to reset this _last_alarm
    }
    inline void stop_alarm()
    {
        _alarm_start_time.reset();
        _alarm_percent.reset();
        _need_oled_update = true;
    }
    inline void snooze_alarm()
    { // TODO
    }

    inline bool alarm_already_started_this_minute() const
    {
        return _last_alarm.has_value() && _last_alarm.value() == get_current_time();
    }
};
