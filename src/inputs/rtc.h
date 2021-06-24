#pragma once

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include "utils/time_s.h"

class Rtc
{
private:
    typedef RtcDS3231<TwoWire> RtcDevice;
    RtcDevice _rtc;

public:
    Rtc();

    void setup();
    RtcDateTime GetDateTime();
    TimeS get_time();

    void save_alarm(const Time &time, uint8_t alarm_index);
    void read_alarm(Time &time, uint8_t &alarm_index);
#ifdef DEBUG_RTC
    void printDateTime(const RtcDateTime &dt) const;
#endif
};
