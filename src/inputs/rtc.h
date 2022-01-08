#pragma once

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include "js_time_s.h"

class Rtc
{
private:
    typedef RtcDS3231<TwoWire> RtcDevice;
    RtcDevice _rtc;
    TwoWire& _wire;

  public:
    Rtc();

    void setup();
    RtcDateTime GetDateTime();
    TimeS get_time();
    void save_time(const Time & value);
    
    void save_alarm(const Time &alarm1, const Time &alarm2, const Time &alarm3, const Time &alarm4, const Time &alarm5, const Time &alarm6, uint8_t alarm_index);
    void read_alarm(Time &alarm1, Time &alarm2, Time &alarm3, Time &alarm4, Time &alarm5, Time &alarm6, uint8_t &alarm_index);
#ifdef DEBUG_RTC
    void printDateTime(const RtcDateTime &dt) const;
#endif
};
