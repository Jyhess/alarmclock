#include <Arduino.h>
#include "rtc.h"
#include "oled.h"
#include "outputs.h"
#include "inputs.h"
#include "state.h"
#include "buzzer.h"

// Module connection pins (Digital Pins)
#define PIN_LIGHT_SENSOR A0
#define PIN_BUZZER 2
#define PIN_BUTTON_RED 11
#define PIN_BUTTON_YELLOW 12
#define PIN_BUTTON_GREEN 13

Rtc g_rtc;
Oled g_oled;
Outputs g_outputs;
Inputs g_inputs(PIN_BUTTON_RED, PIN_BUTTON_YELLOW, PIN_BUTTON_GREEN);
State g_state;
Buzzer g_buzzer(PIN_BUZZER);
long last_run_ms = 0;

void setup()
{
  Serial.begin(57600);
  Serial.println("Running setup...");
  g_rtc.setup();
  g_oled.setup();
  g_state.setup();
  g_buzzer.setup();
  //g_inputs.setup();
  Serial.println("Setup done");
}

void loop()
{
  long now_ms = millis();
  g_inputs.loop(now_ms);
  g_state.loop(g_inputs, g_outputs);

  // Update time only once per seconds
  if (now_ms - last_run_ms > 1000 || now_ms - last_run_ms < 0)
  {
    RtcDateTime now = g_rtc.GetDateTime();
    g_outputs.set_current_time(Time(now.Hour(), now.Minute()));
    last_run_ms = now_ms;
  }
  g_oled.loop(g_outputs);
  g_outputs.clear_updated();

  if (g_outputs.get_step() == Step::NORMAL)
  {
    if (g_inputs.green_has_been_pressed())
    {
      g_buzzer.play();
    }
    else if (g_inputs.yellow_has_been_pressed())
    {
      g_buzzer.stop();
    }
    g_buzzer.loop(now_ms);
  }
}
