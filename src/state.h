#pragma once

#include <stdint.h>

#include "utils/time.h"
#include "utils/optional.h"
#include "utils/range.h"
#include "alarm_list.h"
#include "alarm_runner.h"

class State
{
public:
    enum Step
    {
        NORMAL,
        ALARM_SELECT,
        ALARM_SET_HOUR,
        ALARM_SET_MINUTE,
        ALARM_PLAYING,
        TIME_SET_HOUR,
        TIME_SET_MINUTE,
        NO_DISPLAY
    };

private:
    unsigned long _now_ms;
    bool _need_oled_update;

    uint8_t _display_brightness;
    TimeS _time;
    Optional<Time> _alarm;
    Step _step;
    Time _edit_time;

    AlarmList &_alarm_list;
    Optional<AlarmRunner> _alarm_runner;

    Range<int16_t, 0, 255> _sun_value;
    Optional<int16_t> _debug_value;

public:
    State(AlarmList &_alarm_list);

    inline void setup()
    {
        _edit_time = _alarm_list.get_alarm(0);
    }

    bool is_updated() const;
    void clear_updated();
    inline void set_updated() { _need_oled_update = true; }

    inline Optional<int16_t> get_debug_value() const { return _debug_value; }
    inline void set_debug_value(int16_t value)
    {
        if( value != *_debug_value )
        {
            _debug_value = value;
            _need_oled_update = true;
        }
    }


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
        if( value != _display_brightness )
        {
            _display_brightness = value;
            _need_oled_update = true;
        }
    }

    inline int16_t get_sun_value() const { return *_sun_value; }
    inline void set_sun_value(int16_t value) { _sun_value = value; }

    inline const TimeS &get_current_time() const { return _time; }
    inline void set_current_time(const TimeS &value)
    {
        _need_oled_update |= _time.get_second() != value.get_second();
        _time = value;
    }

    inline const Time &get_edit_time() const { return _edit_time; }
    inline void set_edit_time(const Time &value)
    {
        if(_edit_time != value )
        {
            _edit_time = value;
            _need_oled_update = true;
        }
    }

    inline uint8_t get_alarm_index() const { return _alarm_list.get_alarm_index(); }
    inline void set_alarm_index(uint8_t value)
    {
        _alarm_list.set_alarm_index(value);
        _need_oled_update = true;
    }

    inline bool is_alarm_on() const { return _alarm.has_value(); }
    inline const Time &get_alarm() const { return _alarm.value(); }
    inline void set_alarm(const Time &value)
    {
        _alarm = value;
        _alarm_list.save_alarm(value);
        _need_oled_update = true;
        _edit_time = value;
    }
    inline void switch_off_alarm()
    {
        _alarm.reset();
        _alarm_runner.reset();
        _alarm_list.save_no_alarm();
        _need_oled_update = true;
    }

    inline const AlarmList &get_alarms() const { return _alarm_list; }
    inline AlarmList &alarms() { return _alarm_list; }

    inline bool is_alarm_playing() const { return _alarm_runner.has_value(); }
    inline uint8_t get_alarm_value() const { return _alarm_runner->get_alarm_value(_time); }

    inline void start_playing_alarm() { _alarm_runner = AlarmRunner(_time); }
    inline void snooze_alarm() { _alarm_runner->snooze_alarm(_time); }
};
