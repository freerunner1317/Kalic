#pragma once
#include <Arduino.h>
#include <FastLED.h>


void ColoredRain();
void rain(byte backgroundDepth, byte maxBrightness, byte spawnFreq, byte tailLength, CRGB rainColor, bool splashes, bool clouds, bool storm);