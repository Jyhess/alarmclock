#pragma once

#include <stdint.h>

class Time
{
private:
    uint8_t _hour;
    uint8_t _minute;

public:
    inline constexpr Time() : _hour(0), _minute(0) {}
    inline Time(uint8_t hour, uint8_t minute) : _hour(hour), _minute(minute) {}

    inline uint8_t get_hour() const { return _hour; }
    inline void set_hour(uint8_t hour) { _hour = hour; }
    inline uint8_t get_minute() const { return _minute; }
    inline void set_minute(uint8_t minute) { _minute = minute; }

    inline bool operator==(const Time &other) const { return _minute == other._minute && _hour == other._hour; }
    inline bool operator!=(const Time &other) const { return !operator==(other); }

    inline Time &add_hour(int8_t value)
    {
        while(value < 0)
            value += 24;
        _hour = (_hour + value) % 24;
        return *this;
    }
    inline Time &add_minute(int8_t value)
    {
        while (value < 0)
            value += 60;
        _minute = (_minute + value) % 60;
        return *this;
    }
};
