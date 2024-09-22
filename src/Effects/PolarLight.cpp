#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <types.h>
#include <constants.h>
#include <EffectsManager.h>
#include <MyMatrix.h>

// вместо набора палитр в оригинальном эффекте сделан генератор палитр
#define AURORA_COLOR_RANGE 10 // (+/-10 единиц оттенка) диапазон, в котором плавает цвет сияния относительно выбранного оттенка 
#define AURORA_COLOR_PERIOD 2 // (2 раза в минуту) частота, с которой происходит колебание выбранного оттенка в разрешённом диапазоне

extern float emitterX, emitterY;                          // какие-то динамичные координаты
extern uint16_t ff_x, ff_y, ff_z;                         // большие счётчики
extern float speedfactor;                                 // регулятор скорости в эффектах реального времени
extern uint8_t hue, hue2;                                 // постепенный сдвиг оттенка или какой-нибудь другой цикличный счётчик
extern uint8_t deltaHue, deltaHue2;                       // ещё пара таких же, когда нужно много
unsigned long polarTimer;


//extern uint8_t mapedPressureFPS;
extern uint16_t bmpNormalPressure;

extern CRGBPalette16 myPal;

extern CRGB leds[NUM_LEDS];

void polarRoutine(uint16_t bmpDataRow) {
    //emitterX = fmap((float)HEIGHT, 8, 32, 28, 12); такое работало с горем пополам только для матриц до 32 пикселей в высоту
    //emitterX = 512. / HEIGHT - 0.0001; // это максимально возможное значение
    emitterX = 400. / HEIGHT; // а это - максимум без яркой засветки крайних рядов матрицы (сверху и снизу)

    ff_y = map(WIDTH, 8, 64, 310, 63);
    //ff_z = map(modes[currentMode].Scale, 1, 100, 30, ff_y);
    ff_z = ff_y;

    speedfactor = map(bmpDataRow, MIN_PRESSURE, bmpNormalPressure, 16, 60); // _speed = map(speed, 1, 255, 128, 16);
  
    fillMyPal16_2((uint8_t)((55 - 1U) * 2.55) + AURORA_COLOR_RANGE - beatsin8(AURORA_COLOR_PERIOD, 0U, AURORA_COLOR_RANGE+AURORA_COLOR_RANGE), 55 & 0x01);


    for (byte x = 0; x < WIDTH; x++) {
        for (byte y = 0; y < HEIGHT; y++) {
        polarTimer++;
        //uint16_t i = x*y;
        leds[XY(x, y)]= 
            ColorFromPalette(myPal,
                qsub8(
                inoise8(polarTimer % 2 + x * ff_z,
                    y * 16 + polarTimer % 16,
                    polarTimer / speedfactor
                ),
                fabs((float)HEIGHT/2 - (float)y) * emitterX
                )
            );     
        }
    }
}