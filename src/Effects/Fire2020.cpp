#include <Arduino.h>
#include <CommonFunc.h>
#include <palettes.h>
#include <WS2812.h>
#include <types.h>
#include <constants.h>
#include <EffectsManager.h>
#include <MyMatrix.h>

#define SPARKLES_NUM (WIDTH / 8U)  // не более чем  enlargedOBJECT_MAX_COUNT (WIDTH * 2)
//#define trackingOBJECT_MAX_COUNT (100U)  // максимальное количество отслеживаемых объектов (очень влияет на расход памяти)
extern float trackingObjectPosX[trackingOBJECT_MAX_COUNT];
extern float trackingObjectPosY[trackingOBJECT_MAX_COUNT];

extern CRGB leds[NUM_LEDS];
extern ModeType modes[MODE_AMOUNT];
extern uint8_t currentMode;

extern uint16_t ff_x, ff_y, ff_z;                      // большие счётчики

uint8_t deltaValue;
uint8_t deltaHue, deltaHue2;  // ещё пара таких же, когда нужно много
extern uint8_t step;                 // какой-нибудь счётчик кадров или последовательностей операций
uint8_t shiftHue[HEIGHT];                       // свойство пикселей в размер столбца матрицы

extern const TProgmemRGBPalette16 *curPalette;

 void Fire2020_effect(uint8_t fireHeight) {
  if (1) {
    FastLED.clear();
    //loadingFlag = false;
    if (modes[currentMode].Scale > 100U) modes[currentMode].Scale = 100U;  // чтобы не было проблем при прошивке без очистки памяти
    /*if (modes[currentMode].Scale == 100U)
      deltaValue = random8(9U);
    else
      deltaValue = modes[currentMode].Scale * 0.0899; // /100.0F * ((sizeof(firePalettes)/sizeof(TProgmemRGBPalette16 *))-0.01F))*/
    deltaValue = modes[currentMode].Scale * 0.0899;  // /100.0F * ((sizeof(palette_arr) /sizeof(TProgmemRGBPalette16 *))-0.01F));
    // if (deltaValue == 3U || deltaValue == 4U)
    //   curPalette = palette_arr[deltaValue];  // (uint8_t)(modes[currentMode].Scale/100.0F * ((sizeof(palette_arr) /sizeof(TProgmemRGBPalette16 *))-0.01F))];
    // else
    //   curPalette = firePalettes[deltaValue];
      
    curPalette = &NormalFire_p;                                   // (uint8_t)(modes[currentMode].Scale/100.0F * ((sizeof(firePalettes)/sizeof(TProgmemRGBPalette16 *))-0.01F))];
    deltaValue = (((modes[currentMode].Scale - 1U) % 11U + 1U) << 4U) - 2U;  // ширина языков пламени (масштаб шума Перлина)
    deltaHue = map(deltaValue, 8U, 168U, 8U, 200U);                           // высота языков пламени должна уменьшаться не так быстро, как ширина
    step = map(255U - deltaValue, 87U, 247U, 4U, 32U);                       // вероятность смещения искорки по оси ИКС
    for (uint8_t j = 0; j < fireHeight; j++) {
      shiftHue[j] = (fireHeight - 1 - j) * 255 / (fireHeight - 1);  // init colorfade table
    }

    for (uint8_t i = 0; i < SPARKLES_NUM; i++) {
      trackingObjectPosY[i] = random8(fireHeight);
      trackingObjectPosX[i] = random8(WIDTH);
    }
  }
  for (uint8_t i = 0; i < WIDTH; i++) {
    for (uint8_t j = 0; j < fireHeight; j++) {
      //if (modes[currentMode].Brightness & 0x01)
      //      leds[XY(i,HEIGHT-1U-j)] = ColorFromPalette(*curPalette, qsub8(inoise8(i * deltaValue, (j+ff_y+random8(2)) * deltaHue, ff_z), shiftHue[j]), 255U);
      //else // немного сгладим картинку
      nblend(leds[XY(i, fireHeight - 1U - j)], ColorFromPalette(NormalFire_p, qsub8(inoise8(i * deltaValue, (j + ff_y + random8(2)) * deltaHue, ff_z), shiftHue[j]), 255U), 160U);
    }
  }

  //вставляем искорки из отдельного массива
  for (uint8_t i = 0; i < SPARKLES_NUM; i++) {
    //leds[XY(trackingObjectPosX[i],trackingObjectPosY[i])] += ColorFromPalette(*curPalette, random(156, 255));   //trackingObjectHue[i]
    if (trackingObjectPosY[i] > 3U) {
      leds[XY(trackingObjectPosX[i], trackingObjectPosY[i])] = leds[XY(trackingObjectPosX[i], 3U)];
      leds[XY(trackingObjectPosX[i], trackingObjectPosY[i])].fadeToBlackBy(trackingObjectPosY[i] * 2U);
    }
    trackingObjectPosY[i]++;
    if (trackingObjectPosY[i] >= fireHeight) {
      trackingObjectPosY[i] = random8(4U);
      trackingObjectPosX[i] = random8(WIDTH);
    }
    if (!random8(step))
      trackingObjectPosX[i] = (WIDTH + (uint8_t)trackingObjectPosX[i] + 1U - random8(3U)) % WIDTH;
  }
  ff_y++;
  if (ff_y & 0x01)
    ff_z++;
}



// void fire2020(byte scale, int len) {
//   static uint8_t deltaValue;
//   static uint8_t deltaHue;
//   static uint8_t step;
//   static uint8_t shiftHue[50];
//   static float trackingObjectPosX[100];
//   static float trackingObjectPosY[100];
//   static uint16_t ff_x, ff_y, ff_z;

// //#define LED_length 13
// //#define LED_width  5

//   if (1) {
//     //loading = false;
//     //deltaValue = (((scale - 1U) % 11U + 1U) << 4U) - 8U; // ширина языков пламени (масштаб шума Перлина)
//     deltaValue = map(scale, 0, 255, 8, 168);
//     deltaHue = map(deltaValue, 8U, 168U, 8U, 84U); // высота языков пламени должна уменьшаться не так быстро, как ширина
//     step = map(255U - deltaValue, 87U, 247U, 4U, 32U); // вероятность смещения искорки по оси ИКС
//     for (uint8_t j = 0; j < LED_length; j++) {
//       shiftHue[j] = (LED_length - 1 - j) * 255 / (LED_length - 1); // init colorfade table
//     }

//     for (uint8_t i = 0; i < LED_width / 8; i++) {
//       trackingObjectPosY[i] = random8(LED_length);
//       trackingObjectPosX[i] = random8(LED_width);
//     }
//   }
//   for (uint8_t i = 0; i < LED_width; i++) {
//     for (uint8_t j = 0; j < len; j++) {
//       leds[getPix(i, len - 1U - j)] = ColorFromPalette(WoodFireColors_p, qsub8(inoise8(i * deltaValue, (j + ff_y + random8(2)) * deltaHue, ff_z), shiftHue[j]), 255U);
//     }
//   }

//   //вставляем искорки из отдельного массива
//   for (uint8_t i = 0; i < LED_width / 8; i++) {
//     if (trackingObjectPosY[i] > 3U) {
//       leds[getPix(trackingObjectPosX[i], trackingObjectPosY[i])] = leds[getPix(trackingObjectPosX[i], 3U)];
//       leds[getPix(trackingObjectPosX[i], trackingObjectPosY[i])].fadeToBlackBy( trackingObjectPosY[i] * 2U );
//     }
//     trackingObjectPosY[i]++;
//     if (trackingObjectPosY[i] >= len) {
//       trackingObjectPosY[i] = random8(4U);
//       trackingObjectPosX[i] = random8(LED_width);
//     }
//     if (!random8(step))
//       trackingObjectPosX[i] = (LED_width + (uint8_t)trackingObjectPosX[i] + 1U - random8(3U)) % LED_width;
//   }
//   ff_y++;
//   if (ff_y & 0x01) ff_z++;
// }