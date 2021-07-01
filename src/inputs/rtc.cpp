#include "inputs/rtc.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

Rtc::Rtc() : _rtc(Wire)
{
}

void Rtc::setup()
{
    _rtc.Begin();
#ifdef DEBUG_RTC
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!_rtc.IsDateTimeValid())
    {
        if (_rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(_rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            _rtc.SetDateTime(compiled);
        }
    }

    if (!_rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        _rtc.SetIsRunning(true);
    }

    RtcDateTime now = _rtc.GetDateTime();
    if (now < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        _rtc.SetDateTime(compiled);
    }
    else if (now > compiled)
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled)
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
#else
    _rtc.SetIsRunning(true);
    const RtcDateTime now = _rtc.GetDateTime();
    const RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    if (now < compiled)
    {
        Serial.println("RTC time set");
        _rtc.SetDateTime(compiled);
    }
#endif

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    _rtc.Enable32kHzPin(false);
    _rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

RtcDateTime Rtc::GetDateTime()
{
#ifdef DEBUG_RTC
    if (!_rtc.IsDateTimeValid())
    {
        Serial.print("RTC error: ");
        if (_rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.println(_rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("battery HS");
        }
    }
#endif
    return _rtc.GetDateTime();
}

TimeS Rtc::get_time()
{
    const RtcDateTime now = GetDateTime();
    return TimeS(now.Hour(), now.Minute(), now.Second());
}

void Rtc::save_alarm(const Time &alarm1, const Time &alarm2, const Time &alarm3, uint8_t alarm_index)
{
    DS3231AlarmOne alarm_one(alarm1.get_hour(), alarm1.get_minute(), alarm2.get_hour(), alarm2.get_minute(), DS3231AlarmOneControl::DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    DS3231AlarmTwo alarm_two(alarm3.get_hour(), alarm3.get_minute(), alarm_index, DS3231AlarmTwoControl::DS3231AlarmTwoControl_HoursMinutesMatch);
    _rtc.SetAlarmOne(alarm_one);
    _rtc.SetAlarmTwo(alarm_two);
}

void Rtc::read_alarm(Time &alarm1, Time &alarm2, Time &alarm3, uint8_t &alarm_index)
{
    auto alarm_one = _rtc.GetAlarmOne();
    alarm1 = Time(alarm_one.DayOf(), alarm_one.Hour());
    alarm2 = Time(alarm_one.Minute(), alarm_one.Second());
    auto alarm_two = _rtc.GetAlarmTwo();
    alarm3 = Time(alarm_two.DayOf(), alarm_two.Hour());
    alarm_index = alarm_two.Minute();
}

#ifdef DEBUG_RTC
void Rtc::printDateTime(const RtcDateTime &dt) const
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(),
               dt.Day(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    Serial.print(datestring);
}
#endif
