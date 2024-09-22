#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <types.h>
#include <constants.h>
#include <EffectsManager.h>
#include <MyMatrix.h>


#define BORDERTHICKNESS (1U) // глубина бордюра для размытия яркой частицы: 0U - без границы (резкие края); 1U - 1 пиксель (среднее размытие) ; 2U - 2 пикселя (глубокое размытие)
const uint8_t paintWidth = WIDTH - BORDERTHICKNESS * 2;
const uint8_t paintHeight = HEIGHT - BORDERTHICKNESS * 2;

extern CRGB leds[NUM_LEDS];
extern uint16_t bmpNormalPressure;

void LightBallsRoutine(uint16_t bmpValueRow)
{

  // Apply some blurring to whatever's already on the matrix
  // Note that we never actually clear the matrix, we just constantly
  // blur it repeatedly. Since the blurring is 'lossy', there's
  // an automatic trend toward black -- by design.
  //uint8_t blurAmount = dim8_raw(beatsin8(3, 64, 100));
  //blur2d(leds, WIDTH, HEIGHT, blurAmount);
  blurScreen(dim8_raw(beatsin8(3, 64, 100)), leds);

  // Use two out-of-sync sine waves
  uint16_t i = beatsin16( 79, 0, 255); //91
  uint16_t j = beatsin16( 67, 0, 255); //109
  uint16_t k = beatsin16( 53, 0, 255); //73
  uint16_t m = beatsin16( 97, 0, 255); //123

  // The color of each point shifts over time, each at a different speed.
  uint32_t ms = millis() / (82 / 4 + 1);
  leds[XY( highByte(i * paintWidth) + BORDERTHICKNESS, highByte(j * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 29, 200U, 255U);
  leds[XY( highByte(j * paintWidth) + BORDERTHICKNESS, highByte(k * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 41, 200U, 255U);
  leds[XY( highByte(k * paintWidth) + BORDERTHICKNESS, highByte(m * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 37, 200U, 255U);
  leds[XY( highByte(m * paintWidth) + BORDERTHICKNESS, highByte(i * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 53, 200U, 255U);
}