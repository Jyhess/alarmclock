#include <Arduino.h>
#include "outputs/outputs.h"
#include "inputs/inputs.h"
#include "state.h"
#include "runner.h"

// Module connection pins (Digital Pins)
#define PIN_LIGHT_SENSOR A0
#define PIN_BUZZER 2
#define PIN_SUN 3
#define PIN_BUTTON_RED 11
#define PIN_BUTTON_YELLOW 12
#define PIN_BUTTON_GREEN 13

Outputs g_outputs(PIN_BUZZER, PIN_SUN);
Inputs g_inputs(PIN_BUTTON_RED, PIN_BUTTON_YELLOW, PIN_BUTTON_GREEN);
Runner g_runner;
State g_state;

void setup()
{
  Serial.begin(57600);
  Serial.println("Running setup...");
  g_outputs.setup();
  g_inputs.setup();
  Serial.println("Setup done");
}

void loop()
{
  g_inputs.loop();
  g_runner.loop(g_inputs, g_state);
  g_outputs.loop(g_state);
  g_state.clear_updated();
}
