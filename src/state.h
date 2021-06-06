#pragma once

#include "step.h"
#include "utils/time.h"
#include "utils/updatable.h"
#include "utils/optional.h"
#include "utils/array.h"

#define NO_ALARM -2
#define CUSTOM_ALARM -1
#define PREDEFINED_ALARMS 6

class State
{
private:
    long _now_ms;

    Updatable<int> _display_brightness;
    Updatable<Time> _time;
    Updatable<Optional<Time>> _alarm;
    Updatable<Step> _step;

    Updatable<Time> _custom_alarm;
    typedef Array<Time, PREDEFINED_ALARMS> Alarms;
    Alarms _alarms;
    // Off = -2
    // Custom = -1
    // Predefined = [0,PREDEFINED_ALARMS[
    Updatable<int> _alarm_index;

    Updatable<bool> _alarm_playing;
    Optional<Time> _last_alarm;

    Updatable<int> _sun_percent;

public:
    State();

    bool is_updated() const;
    void clear_updated();

    inline long now_ms() const { return _now_ms; }
    inline void set_now_ms(long value) { _now_ms = value; }

    inline const Step &get_step() const { return *_step; }
    inline void set_step(const Step &value) { _step = value; }

    inline int get_display_brightness() const { return *_display_brightness; }
    inline void set_display_brightness(int value) { _display_brightness = value; }

    inline int get_sun_percent() const { return *_sun_percent; }
    inline void set_sun_percent(int value) { _sun_percent = value; }

    inline const Time &get_current_time() const { return *_time; }
    inline void set_current_time(const Time &value) { _time = value; }

    inline bool is_alarm_on() const { return _alarm.value().has_value(); }
    inline const Time &get_alarm() const { return _alarm.value().value(); }
    inline void set_alarm(const Time &value) { _alarm = value; }
    inline void switch_off_alarm() { _alarm = Optional<Time>(); }

    inline const Time &get_custom_alarm() const { return *_custom_alarm; }
    inline void set_custom_alarm(const Time &value) { _custom_alarm = value; }

    const Alarms &get_predefine_alarms() const { return _alarms; }
    inline int get_alarm_index() const { return *_alarm_index; }
    inline void set_alarm_index(int value) { _alarm_index = value; }

    inline bool is_alarm_playing() const { return *_alarm_playing; }
    inline void set_alarm_playing(bool value)
    {
        _alarm_playing = value;
        if (value)
            _last_alarm = get_current_time();
        // TODO: Where to reset this _last_alarm
    }

    inline bool alarm_already_started_this_minute() const
    {
        return _last_alarm.has_value() && _last_alarm.value() == get_current_time();
    }
};
