#pragma once

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>


class Rtc
{
private:
    RtcDS3231<TwoWire> _rtc;

public:
    Rtc();

    void setup();
    RtcDateTime GetDateTime();

    void printDateTime(const RtcDateTime &dt) const;
};
