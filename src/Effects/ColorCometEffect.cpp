#include "Effects/ColorCometEffect.h"
#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <FastLED.h>

extern CRGB leds;



void tick()
{
   
    CRGB _eNs_color = CRGB::White;
    
    _eNs_color = ColorFromPalette(RainbowColors_p, random(0, 255));

    //drawPixelXY(getCenterX(), getCenterY(), _eNs_color);
    // drawPixelXY(getCenterX() + 1, getCenterY(), _eNs_color);
    // drawPixelXY(getCenterX(), getCenterY() + 1, _eNs_color);
    // drawPixelXY(getCenterX() + 1, getCenterY() + 1, _eNs_color);

    // Noise
    // effectX[0] += 1500;
    // effectY[0] += 1500;
    // effectZ[0] += 1500;
    // effectScaleX[0] = 8000;
    // effectScaleY[0] = 8000;
    // FillNoise(0);
    // MoveFractionalNoiseX(mySettings->matrixSettings.width / 2 - 1);
    // MoveFractionalNoiseY(mySettings->matrixSettings.height / 2 - 1);
}