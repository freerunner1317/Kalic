#include <Effects/RGB_Prop.h>
#include <WS2812.h>
#include <FastLED.h>
#include <constants.h>
#include <MyMatrix.h>
#include <EffectsManager.h>
#include <types.h>

int thisdelay = 15;          //-FX LOOPS DELAY VAR
int thisstep = 10;           //-FX LOOPS DELAY VAR
int thishue = 0;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR
int idex = 0;                //-LED INDEX (0 to LED_COUNT-1

uint8_t hue_2 = 0;
extern uint8_t hue, hue2;

extern CRGB leds[NUM_LEDS];
extern ModeType modes[MODE_AMOUNT];

static const uint8_t maxDim = max(WIDTH, HEIGHT);



void RGBpropeller() { 
  rainbowHorVertRoutine(false);

  hue_2 += 10;
  if (hue_2 >= 255)
   hue_2 = 0;

  for (uint8_t i = 0U; i < WIDTH; i++)
    for (uint8_t j = 0U; j < HEIGHT; j++) {
      float twirlFactor = 3;  // на сколько оборотов будет закручена матрица, [0..3]
      CRGB thisColor = CHSV((uint8_t)(hue_2 + ((float)WIDTH / (float)HEIGHT * i + j * twirlFactor) * ((float)255 / (float)maxDim)), 255U, 255U);
      drawPixelXY(i, j, thisColor);
    }
}

void rainbowHorVertRoutine(bool isVertical) {
  hue += 10;
  if (hue >= 255)
   hue = 0;
  for (uint8_t i = 0U; i < (isVertical ? WIDTH : HEIGHT); i++) {
    //CRGB thisColor;
    //hsv2rgb_spectrum(CHSV(hue + i * (modes[currentMode].Scale % 67U) * 2U, 255U, 255U), thisColor); // так ещё хуже стало на низкой яркости
    CHSV thisColor = CHSV((uint8_t)(hue + i * (40 % 67U) * 2U), 255U, 255U);

    for (uint8_t j = 0U; j < (isVertical ? HEIGHT : WIDTH); j++)
      drawPixelXY((isVertical ? i : j), (isVertical ? j : i), thisColor);
  }
}

// void RGBpropeller() {                           //-m27-RGB PROPELLER
//   idex++;
//   int ghue = (thishue + 80) % 255;
//   int bhue = (thishue + 160) % 255;
//   int N3  = int(NUM_LEDS / 3);
//   int N6  = int(NUM_LEDS / 6);
//   int N12 = int(NUM_LEDS / 12);
//   for (int i = 0; i < N3; i++ ) {
//     int j0 = (idex + i + NUM_LEDS - N12) % NUM_LEDS;
//     int j1 = (j0 + N3) % NUM_LEDS;
//     int j2 = (j1 + N3) % NUM_LEDS;
//     leds[j0] = CHSV(thishue, thissat, 255);
//     leds[j1] = CHSV(ghue, thissat, 255);
//     leds[j2] = CHSV(bhue, thissat, 255);
//   }
//   //LEDS.show();
//   //delay(thisdelay);
// }