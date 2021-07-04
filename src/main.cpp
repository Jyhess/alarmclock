#include <Arduino.h>
#include "outputs/outputs.h"
#include "inputs/inputs.h"
#include "state.h"
#include "runner.h"

// Module connection pins (Digital Pins)
#define PIN_LIGHT_SENSOR PIN_A0
#define PIN_BUZZER 5
#define PIN_SUN_LOW 9
#define PIN_SUN_MID 10
#define PIN_SUN_HIGH 11

#define PIN_OLED_CLOCK 7
#define PIN_OLED_DATA 6
#define PIN_OLED_RESET 8
#define PIN_OLED_DC 12
#define PIN_OLED_CS 13

#define PIN_BUTTON_RED 4
#define PIN_BUTTON_YELLOW 2
#define PIN_BUTTON_GREEN 3
#define PIN_VIBRATION PIN_A1

Outputs g_outputs(
    PIN_BUZZER, PIN_SUN_LOW, PIN_SUN_MID, PIN_SUN_HIGH
#ifndef OLED_I2C
    ,
    PIN_OLED_CLOCK, PIN_OLED_DATA, PIN_OLED_CS, PIN_OLED_DC, PIN_OLED_RESET
#endif
);
Inputs g_inputs(PIN_BUTTON_RED, PIN_BUTTON_YELLOW, PIN_BUTTON_GREEN, PIN_LIGHT_SENSOR, PIN_VIBRATION);
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
  g_runner.loop(g_inputs, g_outputs);
  g_outputs.loop(g_state);
}
