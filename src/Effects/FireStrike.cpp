#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <types.h>
#include <constants.h>
#include <EffectsManager.h>
#include <MyMatrix.h>

#define devison_matrix 6

void fireStrike(uint8_t fireHeight) {
  FastLED.clear();

  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < fireHeight; j++) {
      if (random(0, 100) < 5) drawPixelXY(i, j, CRGB::White);
    }
  }

  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < fireHeight; j++) {
      for (int k = 0; k < WIDTH / devison_matrix; k++)
        if (i == k * devison_matrix) drawPixelXY(i, j, CRGB::Red);
    }
  }
}