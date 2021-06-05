#include "inputs/rtc.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

Rtc::Rtc() : _rtc(Wire)
{
}

void Rtc::setup()
{

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    _rtc.Begin();

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

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    _rtc.Enable32kHzPin(false);
    _rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

RtcDateTime Rtc::GetDateTime()
{
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
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("RTC lost confidence in the DateTime!");
        }
    }

    return _rtc.GetDateTime();
}

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
