#include <Effects/ConfitiEffect.h>
#include <Arduino.h>
#include <WS2812.h>
#include <FastLED.h>

extern CRGB leds[NUM_LEDS];


void ConfitiEffect(int delay){
    for (size_t i = 0; i < NUM_LEDS; i++)    // restrained - 8700) / 20
    {
        if (leds[i] == CRGB(0, 0, 0)){
            if (random(0, 1200) < delay) {
                leds[i] = ColorFromPalette(RainbowColors_p, random(0, 255));
            }
        }
    }
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
        if (random(0, 100) < 5) {
            if (leds[i].r >= 10 || leds[i].g >= 10 || leds[i].b >= 10) {
                leds[i].fadeToBlackBy(10);
            } else {
                leds[i] = CRGB(0, 0, 0);
            }
        }    
    }
}

