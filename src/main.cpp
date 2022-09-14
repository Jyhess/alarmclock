#include <Arduino.h>
#include "outputs/outputs.h"
#include "inputs/inputs.h"
#include "state.h"
#include "runner.h"

#ifdef TEST_BOARD
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

#define PIN_BUTTON_MIDDLE 4
#define PIN_BUTTON_LEFT 2
#define PIN_BUTTON_RIGHT 3
#define PIN_VIBRATION PIN_A1

#else
#define PIN_LIGHT_SENSOR PIN_A3
#define PIN_BUZZER PIN_PD6
#define PIN_SUN_LOW PIN_PB3
#define PIN_SUN_MID PIN_PB2
#define PIN_SUN_HIGH PIN_PB1

#define PIN_OLED_CLOCK PIN_PD7
#define PIN_OLED_DATA PIN_PB0
#define PIN_OLED_RESET U8X8_PIN_NONE
#define PIN_OLED_DC PIN_PB4
#define PIN_OLED_CS PIN_PB5

#define PIN_BUTTON_SEND PIN_PD5
#define PIN_BUTTON_MIDDLE PIN_PD3
#define PIN_BUTTON_LEFT PIN_PD2
#define PIN_BUTTON_RIGHT PIN_PD4
#define PIN_VIBRATION PIN_A2

#endif

Outputs g_outputs(
    PIN_BUZZER, PIN_SUN_LOW, PIN_SUN_MID, PIN_SUN_HIGH
#ifndef OLED_I2C
    ,
    PIN_OLED_CLOCK, PIN_OLED_DATA, PIN_OLED_CS, PIN_OLED_DC, PIN_OLED_RESET
#endif
);
Inputs g_inputs(PIN_BUTTON_SEND, PIN_BUTTON_MIDDLE, PIN_BUTTON_LEFT, PIN_BUTTON_RIGHT, PIN_LIGHT_SENSOR, PIN_VIBRATION);
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
