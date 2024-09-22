#include "Effects/ColorComet.h"
#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <FastLED.h>
#include <types.h>
#include <constants.h>
#include <EffectsManager.h>
#include <MyMatrix.h>

extern CRGB leds[NUM_LEDS];
CRGB ledsbuff[NUM_LEDS];      // копия массива leds[] целиком

uint32_t noise32_x[NUM_LAYERSMAX];
uint32_t noise32_y[NUM_LAYERSMAX];
uint32_t noise32_z[NUM_LAYERSMAX];
uint32_t scale32_x[NUM_LAYERSMAX];
uint32_t scale32_y[NUM_LAYERSMAX];

extern uint8_t noise3d[NUM_LAYERSMAX][WIDTH][HEIGHT];  // двухслойная маска или хранилище свойств в размер всей матрицы

uint8_t noisesmooth;

int8_t zD;
int8_t zF;

void dimAll(uint8_t value);
void FillNoise(int8_t layer);
void MoveFractionalNoiseY(int8_t amplitude, float shift);
void MoveFractionalNoiseX(int8_t amplitude, float shift);


void ColorCometRoutine() {  // <- ******* для оригинальной прошивки Gunner47 ******* (раскомментить/закоментить)

  dimAll(254U);  // < -- затухание эффекта для последующего кадра
  CRGB _eNs_color = CRGB::Blue;

  //if (modes[0].Scale < 100) _eNs_color = CHSV((modes[currentMode].Scale) * 2.57, 255, 255);  // 2.57 вместо 2.55, потому что при 100 будет белый цвет
  leds[XY(CENTER_X_MINOR, CENTER_Y_MINOR)] += _eNs_color;
  leds[XY(CENTER_X_MINOR + 1, CENTER_Y_MINOR)] += _eNs_color;
  leds[XY(CENTER_X_MINOR, CENTER_Y_MINOR + 1)] += _eNs_color;
  leds[XY(CENTER_X_MINOR + 1, CENTER_Y_MINOR + 1)] += _eNs_color;

  // Noise
  noise32_x[0] += 3000;
  noise32_y[0] += 3000;
  noise32_z[0] += 3000;
  scale32_x[0] = 16000;
  scale32_y[0] = 16000;
  FillNoise(0);
  MoveFractionalNoiseX(WIDTH / 2U + 1U, 3);
  MoveFractionalNoiseY(HEIGHT / 2U + 1U, 3);
}




void FillNoise(int8_t layer) {
  for (uint8_t i = 0; i < WIDTH; i++) {
    int32_t ioffset = scale32_x[layer] * (i - CENTER_X_MINOR);
    for (uint8_t j = 0; j < HEIGHT; j++) {
      int32_t joffset = scale32_y[layer] * (j - CENTER_Y_MINOR);
      int8_t data = inoise16(noise32_x[layer] + ioffset, noise32_y[layer] + joffset, noise32_z[layer]) >> 8;
      int8_t olddata = noise3d[layer][i][j];
      int8_t newdata = scale8(olddata, noisesmooth) + scale8(data, 255 - noisesmooth);
      data = newdata;
      noise3d[layer][i][j] = data;
    }
  }
}

void MoveFractionalNoiseX(int8_t amplitude, float shift) {
  for (uint8_t y = 0; y < HEIGHT; y++) {
    int16_t amount = ((int16_t)noise3d[0][0][y] - 128) * 2 * amplitude + shift * 256;
    int8_t delta = abs(amount) >> 8;
    int8_t fraction = abs(amount) & 255;
    for (uint8_t x = 0; x < WIDTH; x++) {
      if (amount < 0) {
        zD = x - delta;
        zF = zD - 1;
      } else {
        zD = x + delta;
        zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black;
      if ((zD >= 0) && (zD < WIDTH)) PixelA = leds[XY(zD, y)];
      CRGB PixelB = CRGB::Black;
      if ((zF >= 0) && (zF < WIDTH)) PixelB = leds[XY(zF, y)];
      ledsbuff[XY(x, y)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));  // lerp8by8(PixelA, PixelB, fraction );
    }
  }
  memcpy(leds, ledsbuff, sizeof(CRGB) * NUM_LEDS);
}

void MoveFractionalNoiseY(int8_t amplitude, float shift) {
  for (uint8_t x = 0; x < WIDTH; x++) {
    int16_t amount = ((int16_t)noise3d[0][x][0] - 128) * 2 * amplitude + shift * 256;
    int8_t delta = abs(amount) >> 8;
    int8_t fraction = abs(amount) & 255;
    for (uint8_t y = 0; y < HEIGHT; y++) {
      if (amount < 0) {
        zD = y - delta;
        zF = zD - 1;
      } else {
        zD = y + delta;
        zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black;
      if ((zD >= 0) && (zD < HEIGHT)) PixelA = leds[XY(x, zD)];
      CRGB PixelB = CRGB::Black;
      if ((zF >= 0) && (zF < HEIGHT)) PixelB = leds[XY(x, zF)];
      ledsbuff[XY(x, y)] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
    }
  }
  memcpy(leds, ledsbuff, sizeof(CRGB) * NUM_LEDS);
}