#include <Arduino.h>
#include "outputs/outputs.h"
#include "inputs/inputs.h"
#include "state.h"
#include "runner.h"

// Module connection pins (Digital Pins)
#define PIN_LIGHT_SENSOR PIN_A0
#define PIN_BUZZER 2
#define PIN_OLED_CLOCK 3
#define PIN_OLED_DATA 4
#define PIN_OLED_CS 5
#define PIN_OLED_DC 6
#define PIN_OLED_RESET 7
#define PIN_SUN 10
#define PIN_BUTTON_RED 11
#define PIN_BUTTON_YELLOW 12
#define PIN_BUTTON_GREEN 13

Outputs g_outputs(
    PIN_BUZZER, PIN_SUN
#ifndef OLED_I2C
    ,
    PIN_OLED_CLOCK, PIN_OLED_DATA, PIN_OLED_CS, PIN_OLED_DC, PIN_OLED_RESET
#endif
);
Inputs g_inputs(PIN_BUTTON_RED, PIN_BUTTON_YELLOW, PIN_BUTTON_GREEN, PIN_LIGHT_SENSOR);
Runner g_runner;

void setup()
{
  Serial.begin(57600);
  //Serial.println("Running setup...");
  g_outputs.setup();
  g_inputs.setup();
  //Serial.println("Setup done");
}

void loop()
{
  g_inputs.loop();
  g_runner.loop(g_inputs);
  g_outputs.loop(g_runner.state());
}
