#include <Arduino.h>
#include <display.h>
#include <rtc.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3


// The amount of time (in milliseconds) between tests
#define TEST_DELAY 2000

Display g_display(CLK, DIO);
Rtc g_rtc;

void display(const RtcDateTime &dt)
{
}

void setup()
{
  Serial.begin(57600);
  g_display.setup();
  g_rtc.setup();
}

void loop()
{
  while (true)
  {
    RtcDateTime now = g_rtc.GetDateTime();
    g_display.display_time(now);
    delay(1000);
  }
}
