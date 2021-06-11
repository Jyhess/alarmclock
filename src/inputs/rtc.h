#pragma once

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>

class Rtc
{
private:
    typedef RtcDS3231<TwoWire> RtcDevice;
    RtcDevice _rtc;

public:
    Rtc();

    void setup();
    RtcDateTime GetDateTime();

#ifdef DEBUG_RTC
    void printDateTime(const RtcDateTime &dt) const;
#endif
};
