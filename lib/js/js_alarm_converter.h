#pragma once

#include "js_types.h"
#include "js_time.h"

// This file is created only to be able to test this algo.
// We should test directly rtc class, but it has "arduino.h" dependency
// which does not exists in test mode.

#define DAY (24ull * 60)

uint64_t serialize_alarms(
    const Time &a0,
    const Time &a1,
    const Time &a2,
    const Time &a3,
    const Time &a4,
    const Time &a5,
    uint8_t alarm_index
    )
{
  uint64_t value = static_cast<uint64_t>(a0.to_minutes()) +
                   static_cast<uint64_t>(a1.to_minutes()) * DAY +
                   static_cast<uint64_t>(a2.to_minutes()) * DAY * DAY +
                   static_cast<uint64_t>(a3.to_minutes()) * DAY * DAY * DAY +
                   static_cast<uint64_t>(a4.to_minutes()) * DAY * DAY * DAY * DAY +
                   static_cast<uint64_t>(alarm_index) * DAY * DAY * DAY * DAY * DAY
                   // Can save only 5 alarms with 7 bytes
                   //static_cast<uint64_t>(a5.to_minutes()) * DAY * DAY * DAY * DAY * DAY +
                   //static_cast<uint64_t>(alarm_index?1:0) * DAY * DAY * DAY * DAY * DAY * DAY
                   ;
  return value;
}

void deserialize_alarm(
    uint64_t value,
    Time &a0, Time &a1, Time &a2, Time &a3, Time &a4, Time& a5,
    uint8_t & flag
    )
{
    a0 = Time::from_minutes(value % DAY);
    a1 = Time::from_minutes((value / DAY) % DAY);
    a2 = Time::from_minutes((value / (DAY * DAY)) % DAY);
    a3 = Time::from_minutes((value / (DAY * DAY * DAY)) % DAY);
    a4 = Time::from_minutes((value / (DAY * DAY * DAY * DAY)) % DAY);
    flag = (value / (DAY * DAY * DAY * DAY * DAY));
    // Can save only 5 alarms with 7 bytes
    //a5 = Time::from_minutes((value / (DAY * DAY * DAY * DAY * DAY)) % DAY);
    //flag = (value / (DAY * DAY * DAY * DAY * DAY * DAY));
}

template <class W, uint8_t ADRESS, uint8_t REG_ALARM1, uint8_t REG_ALARM1_SIZE, uint8_t REG_ALARM2, uint8_t REG_ALARM2_SIZE>
class AlarmStore
{
    W& _wire;

public:
    explicit AlarmStore(W& wire) : _wire(wire)
    {
    }

    int save_alarm(
        const Time& alarm1, const Time& alarm2, const Time& alarm3,
        const Time& alarm4, const Time& alarm5, const Time& alarm6,
        uint8_t alarm_index
        )
    {
        uint64_t value = serialize_alarms(alarm1, alarm2, alarm3, alarm4, alarm5, alarm6, alarm_index);
        _wire.beginTransmission(ADRESS);
        _wire.write(REG_ALARM1);
        _wire.write(static_cast<uint8_t>(value));
        _wire.write(static_cast<uint8_t>(value >> 8));
        _wire.write(static_cast<uint8_t>(value >> 16));
        _wire.write(static_cast<uint8_t>(value >> 24));
        uint8_t lastError = _wire.endTransmission();
        if (lastError != 0) {
            return 1;
        }
        _wire.beginTransmission(ADRESS);
        _wire.write(REG_ALARM2);
        _wire.write(static_cast<uint8_t>(value >> 32));
        _wire.write(static_cast<uint8_t>(value >> 40));
        _wire.write(static_cast<uint8_t>(value >> 48));
        lastError = _wire.endTransmission();
        if (lastError != 0) {
            return 2;
        }
        return 0;
    }

    int read_alarm(Time &alarm1, Time &alarm2, Time &alarm3, Time &alarm4, Time &alarm5, Time &alarm6, uint8_t &alarm_index)
    {
        _wire.beginTransmission(ADRESS);
        _wire.write(REG_ALARM1);
        uint8_t lastError = _wire.endTransmission();
        if (lastError != 0)
        {
            return 1;
        }

        uint8_t bytesRead = _wire.requestFrom(ADRESS, REG_ALARM1_SIZE);
        if (REG_ALARM1_SIZE != bytesRead)
        {
            return 3;
        }
        uint64_t value =
            static_cast<uint64_t>(_wire.read()) + (static_cast<uint64_t>(_wire.read()) << 8) +
            (static_cast<uint64_t>(_wire.read()) << 16) + (static_cast<uint64_t>(_wire.read()) << 24);
        _wire.beginTransmission(ADRESS);
        _wire.write(REG_ALARM2);
        lastError = _wire.endTransmission();
        if (lastError != 0)
        {
            return 2;
        }

        bytesRead = _wire.requestFrom(ADRESS, REG_ALARM2_SIZE);
        if (REG_ALARM2_SIZE != bytesRead)
        {
            return 4;
        }
        value += (static_cast<uint64_t>(_wire.read()) << 32) +
                (static_cast<uint64_t>(_wire.read()) << 40) +
                (static_cast<uint64_t>(_wire.read()) << 48);
        deserialize_alarm(value, alarm1, alarm2, alarm3, alarm4, alarm5, alarm6, alarm_index);
        return 0;
    }
};
