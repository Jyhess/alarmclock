#pragma once

#include <stdint.h>
#include "js_time.h"

class TimeS
{
private:
    Time _hm;
    uint8_t _second;

public:
    inline constexpr TimeS() : _second(0) {}
    inline TimeS(const Time &time, uint8_t second = 0) : _hm(time), _second(second) {}
    explicit inline TimeS(uint8_t hour, uint8_t minute = 0, uint8_t second = 0) : _hm(hour, minute), _second(second) {}

    inline uint8_t get_hour() const { return _hm.get_hour(); }
    inline void set_hour(uint8_t hour) { _hm.set_hour(hour); }
    inline uint8_t get_minute() const { return _hm.get_minute(); }
    inline void set_minute(uint8_t minute) { _hm.set_minute(minute); }
    inline uint8_t get_second() const { return _second; }
    inline void set_second(uint8_t second) { _second = second; }

    const Time &hm() const { return _hm; }

    inline unsigned long to_seconds() const { return _hm.to_minutes() * 60 + _second; }

    inline bool operator==(const TimeS &other) const { return _hm == other._hm && _second == other._second; }
    inline bool operator!=(const TimeS &other) const { return !operator==(other); }
    inline bool operator<(const TimeS &other) const { return _hm < other._hm || (_hm == other._hm && _second < other._second); }
    inline bool operator<=(const TimeS &other) const { return operator<(other) || operator==(other); }
    inline bool operator>(const TimeS &other) const { return !operator<=(other); }
    inline bool operator>=(const TimeS &other) const { return !operator<(other); }

    TimeS &add_hms(int16_t hours, int16_t minutes, int16_t seconds);
};

inline TimeS operator+(TimeS a, const TimeS &b) { return a.add_hms(b.get_hour(), b.get_minute(), b.get_second()); }
inline TimeS operator-(TimeS a, const TimeS &b) { return a.add_hms(-b.get_hour(), -b.get_minute(), -b.get_second()); }
