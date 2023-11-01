#pragma once

#define LED_length 13
#define LED_width  5

#define NUM_LEDS LED_length * LED_width
#define DATA_PIN D7


void Init_WS2812();
void AllBlack();