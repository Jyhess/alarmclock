#pragma once

class Time
{
private:
    int _hour;
    int _minute;

public:
    inline Time() : _hour(0), _minute(0) {}
    inline Time(int hour, int minute) : _hour(hour), _minute(minute) {}

    inline int get_hour() const { return _hour; }
    inline void set_hour(int hour) { _hour = hour; }
    inline int get_minute() const { return _minute; }
    inline void set_minute(int minute) { _minute = minute; }

    inline bool operator==(const Time &other) const { return _minute == other._minute && _hour == other._hour; }
    inline bool operator!=(const Time &other) const { return !operator==(other); }

    inline Time &add_hour(int value)
    {
        _hour = (_hour + value) % 24;
        if (_hour < 0)
            _hour = 23;
        return *this;
    }
    inline Time &add_minute(int value)
    {
        _minute = (_minute + value) % 60;
        if (_minute < 0)
            _minute = 59;
        return *this;
    }
};
