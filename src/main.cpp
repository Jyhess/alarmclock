#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000

TM1637Display tm_1637(CLK, DIO);

uint8_t g_brightness = 0;

void display(uint8_t time)
{
  // uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  // data[0] = tm_1637.encodeDigit(0);
  // data[1] = tm_1637.encodeDigit(1);
  // data[2] = tm_1637.encodeDigit(2);
  // data[3] = tm_1637.encodeDigit(3);
  // tm_1637.setSegments(data);
  //tm_1637.clear();
  tm_1637.setBrightness(g_brightness, true);
	tm_1637.showNumberDecEx(time, 64, false);
}

void setup()
{
}

void loop()
{
  uint8_t time = 0;
  while(true)
  {
    display(time++);
    delay(500);
  }
}


