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
                   static_cast<uint64_t>(alarm_index?1:0) * DAY * DAY * DAY * DAY * DAY
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
