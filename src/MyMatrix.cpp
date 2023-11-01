#include <MyMatrix.h>
#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <FastLED.h>


extern CRGB leds;

uint16_t getPixelNumberXY(uint8_t x, uint8_t y);
void drawPixelXY(uint8_t x, uint8_t y, CRGB color);
CRGB swapChannels(CRGB color);
uint16_t XY( uint8_t x, uint8_t y);


void drawPixelXY(uint8_t x, uint8_t y, CRGB color)
{
    leds[getPixelNumberXY(x, y)] = swapChannels(color);
}

uint16_t getPixelNumberXY(uint8_t x, uint8_t y)
{
    return static_cast<uint16_t>(XY(y, x));
}

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}

CRGB swapChannels(CRGB color)
{
    // if (mySettings->matrixSettings.order.length() != 3) {
    //     return color;
    // }
    CRGB result = color;
    // if (mySettings->matrixSettings.order.charAt(0) != 'r') {
    //     if (mySettings->matrixSettings.order.charAt(0) == 'g') {
    //         result.r = color.g;
    //     } else {
    //         result.r = color.b;
    //     }
    // }
    // if (mySettings->matrixSettings.order.charAt(1) != 'g') {
    //     if (mySettings->matrixSettings.order.charAt(1) == 'r') {
    //         result.g = color.r;
    //     } else {
    //         result.g = color.b;
    //     }
    // }
    // if (mySettings->matrixSettings.order.charAt(2) != 'b') {
    //     if (mySettings->matrixSettings.order.charAt(2) == 'r') {
    //         result.b = color.r;
    //     } else {
    //         result.b = color.g;
    //     }
    // }
    return result;
}