#include <Arduino.h>
#include <main.h>
#include <BMP280.h>
#include <WS2812.h>
#include <OTA.h>
#include <EffectsManager.h>

void setup() {
    Init_BMP280();
    Init_WS2812();
    Init_OTA();
}
 
void loop() {
    CurrentStatus();
    ChangeMode();
}

