#include <Arduino.h>
#include "outputs/outputs.h"
#include "inputs/inputs.h"
#include "state.h"
#include "runner.h"

// Module connection pins (Digital Pins)
#define PIN_LIGHT_SENSOR PIN_A0
#define PIN_BUZZER 6
#define PIN_SUN 5

#define PIN_OLED_CLOCK 9
#define PIN_OLED_DATA 10
#define PIN_OLED_RESET 11
#define PIN_OLED_DC 12
#define PIN_OLED_CS 13

#define PIN_BUTTON_RED 4
#define PIN_BUTTON_YELLOW 2
#define PIN_BUTTON_GREEN 3

Outputs g_outputs(
    PIN_BUZZER, PIN_SUN
#ifndef OLED_I2C
    ,
    PIN_OLED_CLOCK, PIN_OLED_DATA, PIN_OLED_CS, PIN_OLED_DC, PIN_OLED_RESET
#endif
);
Inputs g_inputs(PIN_BUTTON_RED, PIN_BUTTON_YELLOW, PIN_BUTTON_GREEN, PIN_LIGHT_SENSOR);
AlarmList g_alarm_list(g_inputs.rtc());
State g_state(g_alarm_list);
Runner g_runner(g_state);

void setup()
{
  Serial.begin(57600);
  //Serial.println("Running setup...");
  g_outputs.setup();
  g_inputs.setup();
  g_alarm_list.setup();
  g_state.setup();
  g_runner.setup(g_inputs);
  //Serial.println("Setup done");
}

void loop()
{
  g_inputs.loop();
  g_runner.loop(g_inputs);
  g_outputs.loop(g_state);
}
