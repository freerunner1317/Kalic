#pragma once
#include <FastLED.h>

uint16_t getPixelNumberXY(uint8_t x, uint8_t y);
CRGB swapChannels(CRGB color);
uint16_t XY(uint8_t x, uint8_t y);
void drawPixelXY(int8_t x, int8_t y, CRGB color);

void DrawLineF(float x1, float y1, float x2, float y2, CRGB color);
void drawPixelXYF(float x, float y, CRGB color);
uint32_t getPixColorXY(uint8_t x, uint8_t y);
uint32_t getPixColor(uint16_t thisPixel);