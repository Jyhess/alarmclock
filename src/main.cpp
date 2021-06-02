#include <Arduino.h>
#include "rtc.h"
#include "oled.h"
#include "data.h"

// Module connection pins (Digital Pins)
#define LED 5
#define CAPACITY_1 10
#define CAPACITY_2 11

Rtc g_rtc;
Oled g_oled;
Data g_data;
long last_run_ms = 0;

void setup()
{
  Serial.begin(57600);
  Serial.println("Running setup...");
  g_rtc.setup();
  g_oled.setup();
  Serial.println("Setup done");
}

void loop()
{
  long now_ms = millis();

  // Update time only once per seconds
  if (now_ms - last_run_ms > 1000 || now_ms - last_run_ms < 0)
  {
    RtcDateTime now = g_rtc.GetDateTime();
    g_data.set_current_time(Time(now.Hour(), now.Minute()));
    last_run_ms = now_ms;
  }
  g_oled.loop(g_data);
  g_data.clear_updated();
}
