#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <types.h>
#include <constants.h>
#include <EffectsManager.h>
#include <MyMatrix.h>
#include <MillisTimer.h>
#include <timers.h>
#include <palettes.h>

extern uint8_t hue, hue2;           // постепенный сдвиг оттенка или какой-нибудь другой цикличный счётчик
extern uint8_t deltaHue, deltaHue2;        // ещё пара таких же, когда нужно много
extern uint16_t bmpNormalPressure;
extern float speedfactor;                                 // регулятор скорости в эффектах реального времени
extern uint8_t pcnt;                                      // какой-то счётчик какого-то прогресса

extern const TProgmemRGBPalette16 *curPalette;

void Plazma(uint16_t bmpDataRow) {   
    curPalette = &LithiumFireColors_p;

    pcnt = 5; // количество линий от 1 до 11 для каждой из 9 палитр
    //speedfactor = fmap(modes[currentMode].Speed, 1, 255, 20., 2.); 
    speedfactor = fmap(bmpDataRow, MIN_PRESSURE, bmpNormalPressure, 5., 30.);

    if (hue2++ & 0x01 && deltaHue++ & 0x01 && deltaHue2++ & 0x01) hue++; // хз. как с 60ю кадрами в секунду скорость замедлять...
    dimAll(205);
    float time_shift = millis() & 0x7FFFFF; // overflow protection proper by SottNick
    time_shift /= speedfactor;
    for (uint8_t c = 0; c < pcnt; c++) {
        float xx = 2. + sin8(time_shift + 6000 * c) / 12.;
        float yy = 2. + cos8(time_shift + 9000 * c) / 12.;
        //DrawLineF(xx, yy, (float)WIDTH - xx - 1, (float)HEIGHT - yy - 1, CHSV(c * (256 / pcnt), 200, 255)); // так было в оригинале
        //if (modes[currentMode].Speed & 0x01)
        //DrawLineF(xx, yy, (float)WIDTH - xx - 1, (float)HEIGHT - yy - 1, ColorFromPalette(*curPalette, hue + c * (255 / pcnt)).nscale8(200)); // кажется, это не работает, хотя и компилируется
        //else
        DrawLineF(xx, yy, (float)WIDTH - xx - 1, (float)HEIGHT - yy - 1, ColorFromPalette(*curPalette, hue + c * (255 / pcnt)));
    }
}