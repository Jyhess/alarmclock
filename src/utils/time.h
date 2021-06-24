#pragma once

#include <stdint.h>

class Time
{
private:
    uint8_t _hour;
    uint8_t _minute;

public:
    inline constexpr Time() : _hour(0), _minute(0) {}
    inline Time(uint8_t hour, uint8_t minute, uint8_t second = 0) : _hour(hour), _minute(minute) {}

    inline uint8_t get_hour() const { return _hour; }
    inline void set_hour(uint8_t hour) { _hour = hour; }
    inline uint8_t get_minute() const { return _minute; }
    inline void set_minute(uint8_t minute) { _minute = minute; }

    inline unsigned long to_minutes() const { return static_cast<unsigned long>(_hour) * 60 + _minute; }

    inline bool operator==(const Time &other) const { return _minute == other._minute && _hour == other._hour; }
    inline bool operator!=(const Time &other) const { return !operator==(other); }
    inline bool operator<(const Time &other) const { return _hour < other._hour || (_hour == other._hour && _minute < other._minute); }
    inline bool operator<=(const Time &other) const { return operator<(other) || operator==(other); }
    inline bool operator>(const Time &other) const { return !operator<=(other); }
    inline bool operator>=(const Time &other) const { return !operator<(other); }

    Time &add_hour(int16_t value);
    Time &add_minute(int16_t value, bool propagate = false);
    Time &add_hm(int16_t hours, int16_t minutes, bool propagate = true);
};

inline Time operator+(Time a, const Time &b) { return a.add_hm(b.get_hour(), b.get_minute(), true); }
inline Time operator-(Time a, const Time &b) { return a.add_hm(-b.get_hour(), -b.get_minute(), true); }
