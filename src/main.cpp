#include <Arduino.h>
#include "timed_capacitive_sensor.h"
#include "display.h"
#include "rtc.h"

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3
#define LED 5
#define CAPACITY_1 10
#define CAPACITY_2 11

Display g_display(CLK, DIO);
Rtc g_rtc;
TimedCapacitiveSensor g_tcs(CAPACITY_1, CAPACITY_2);

void display(const RtcDateTime &dt)
{
}

void setup()
{
  Serial.begin(57600);
  Serial.println("Running setup...");
  pinMode(LED, OUTPUT);
  g_display.setup();
  g_rtc.setup();
  g_tcs.setup();
  Serial.println("Setup done");
}

class Runner
{
private:
  long _last_run_ms;

public:
  Runner() : _last_run_ms(0)
  {
  }
  void run()
  {
    long now_ms = millis();

    g_tcs.loop(now_ms);
    int led_power = g_tcs.activation_duration(now_ms) * 255 / 5000;
    analogWrite(LED, led_power);
    //Serial.printf("%d %d => %i\n", g_tcs.total(), g_tcs.activation_duration(now_ms), led_power);

    // Update time only once per seconds
    if (now_ms - _last_run_ms > 1000)
    {
      RtcDateTime now = g_rtc.GetDateTime();
      if (now.Minute() != g_display.last_time().Minute())
      {
        g_display.display_time(now);
      }
    }
    _last_run_ms = now_ms;
  }
};

Runner g_runner;

void loop()
{
  g_runner.run();
  delay(100);
}
