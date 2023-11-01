#include <Arduino.h>
#include <WS2812.h>
#include <FastLED.h>

extern CRGB leds[NUM_LEDS];

uint16_t getPix(int x, int y) {
  int matrixW;
  int matrix = 4;
  int deviceType = 2;

  if (matrix == 2 || matrix == 4 || matrix == 6 || matrix == 8)  matrixW = LED_length;
  else matrixW = LED_width;
  int thisX, thisY;
  switch (matrix) {
    case 1: thisX = x;                    thisY = y;                    break;
    case 2: thisX = y;                    thisY = x;                    break;
    case 3: thisX = x;                    thisY = (LED_length - y - 1); break;
    case 4: thisX = (LED_length - y - 1); thisY = x;                    break;
    case 5: thisX = (LED_width - x - 1);  thisY = (LED_length - y - 1); break;
    case 6: thisX = (LED_length - y - 1); thisY = (LED_width - x - 1);  break;
    case 7: thisX = (LED_width - x - 1);  thisY = y;                    break;
    case 8: thisX = y;                    thisY = (LED_width - x - 1);  break;
  }

  if ( !(thisY & 1) || (deviceType - 2) ) return (thisY * matrixW + thisX);   // чётная строка
  else return (thisY * matrixW + matrixW - thisX - 1);                            // нечётная строка
}

uint32_t getPixColor(int x, int y) {
  int thisPix = getPix(x, y);
  if (thisPix < 0 || thisPix >= NUM_LEDS) return 0;
  return (((uint32_t)leds[thisPix].r << 16) | ((long)leds[thisPix].g << 8 ) | (long)leds[thisPix].b);
}

void setPix(int x, int y, CRGB color) {
  if (y >= 0 && y < LED_length && x >= 0 && x < LED_width) leds[getPix(x, y)] = color;
}