#include <FastLED.h>
#include <Wire.h>
#include <SPI.h>
#include <WS2812.h>

CRGB leds[NUM_LEDS];

void Init_WS2812(){
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical   
}

void AllBlack(){
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;    
    }
    FastLED.show();
}