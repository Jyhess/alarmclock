#pragma once
#include <RtcDS3231.h>
#include <TM1637Display.h>

class Display
{
    private:
        TM1637Display _tm_1637;
        uint8_t _brightness;

    public:
        Display(uint8_t pinClk, uint8_t pinDIO);
        void setup();
        void display_time(const RtcDateTime &dt);
};
