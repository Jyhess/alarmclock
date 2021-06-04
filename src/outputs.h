#pragma once

#include "time.h"
#include "step.h"
#include "utils/updatable.h"
#include "utils/optional.h"
#include "utils/array.h"

#define NO_ALARM -2
#define CUSTOM_ALARM -1
#define PREDEFINED_ALARMS 6

class Outputs
{
private:
    Updatable<int> _display_brightness;
    Updatable<Time> _time;
    Updatable<Optional<Time>> _alarm;
    Updatable<Step> _step;

    typedef Array<Time, PREDEFINED_ALARMS> Alarms;
    Alarms _alarms;
    // Off = -2
    // Custom = -1
    // Predefined = [0,PREDEFINED_ALARMS[
    Updatable<int> _alarm_index;

public:
    inline Outputs() : _display_brightness(0), _alarm_index(NO_ALARM)
    {
        _alarms[0] = Time(6, 30);
        _alarms[1] = Time(7, 0);
        _alarms[2] = Time(7, 30);
        _alarms[3] = Time(8, 0);
        _alarms[4] = Time(8, 30);
        _alarms[5] = Time(9, 0);
    }

    inline bool is_updated() const
    {
        return _display_brightness.is_updated() || _time.is_updated() || _alarm.is_updated() || _step.is_updated() || _alarm_index.is_updated();
    }
    void clear_updated()
    {
        _display_brightness.clear_updated();
        _time.clear_updated();
        _alarm.clear_updated();
        _step.clear_updated();
        _alarm_index.clear_updated();
    }

    inline const Step &get_step() const { return *_step; }
    inline void set_step(const Step &value) { _step = value; }

    inline int get_display_brightness() const { return *_display_brightness; }
    inline void set_display_brightness(int value) { _display_brightness = value; }

    inline const Time &get_current_time() const { return *_time; }
    inline void set_current_time(const Time &value) { _time = value; }

    inline bool is_alarm_on() const { return _alarm.value().has_value(); }
    inline const Time &get_alarm() const { return _alarm.value().value(); }
    inline void set_alarm(const Time &value) { _alarm = value; }
    inline void switch_off_alarm() { _alarm = Optional<Time>(); }

    const Alarms &get_predefine_alarms() const { return _alarms; }
    inline int get_alarm_index() const { return *_alarm_index; }
    inline void set_alarm_index(int value) { _alarm_index = value; }
};
