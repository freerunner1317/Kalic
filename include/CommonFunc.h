#pragma once
#include <Arduino.h>
#include <FastLED.h>

uint16_t getPix(int x, int y);
uint32_t getPixColor(int x, int y);
void setPix(int x, int y, CRGB color);
#define FOR_i(x,y)  for (int i = (x); i < (y); i++)
#define FOR_j(x,y)  for (int j = (x); j < (y); j++)


