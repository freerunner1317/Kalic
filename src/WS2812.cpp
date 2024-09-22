#include <MyMatrix.h>
#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <FastLED.h>
#include <constants.h>

CRGB leds[NUM_LEDS];

void Init_WS2812(){
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical   
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Green;    
    }
    //FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER);
    FastLED.setBrightness(10); 
    FastLED.show();
    delay(1000);
}

void AllBlack(){
    // for (size_t i = 0; i < NUM_LEDS; i++)
    // {
    //     leds[i] = CRGB::Black;    
    // }
    // FastLED.show();
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;    
    }
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER);
    FastLED.show();
}


