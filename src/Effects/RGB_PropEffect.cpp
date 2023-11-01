#include <RGB_PropEffect.h>
#include <WS2812.h>
#include <FastLED.h>

int thisdelay = 15;          //-FX LOOPS DELAY VAR
int thisstep = 10;           //-FX LOOPS DELAY VAR
int thishue = 0;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR
int idex = 0;                //-LED INDEX (0 to LED_COUNT-1

extern CRGB leds[NUM_LEDS];

void RGBpropeller() {                           //-m27-RGB PROPELLER
  idex++;
  int ghue = (thishue + 80) % 255;
  int bhue = (thishue + 160) % 255;
  int N3  = int(NUM_LEDS / 3);
  int N6  = int(NUM_LEDS / 6);
  int N12 = int(NUM_LEDS / 12);
  for (int i = 0; i < N3; i++ ) {
    int j0 = (idex + i + NUM_LEDS - N12) % NUM_LEDS;
    int j1 = (j0 + N3) % NUM_LEDS;
    int j2 = (j1 + N3) % NUM_LEDS;
    leds[j0] = CHSV(thishue, thissat, 255);
    leds[j1] = CHSV(ghue, thissat, 255);
    leds[j2] = CHSV(bhue, thissat, 255);
  }
  LEDS.show();
  delay(thisdelay);
}