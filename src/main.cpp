#include <Arduino.h>
#include <BMP280.h>
#include <WS2812.h>
#include <OTA.h>
#include <EffectsManager.h>
#include <types.h>
#include <WiFiManager.h>
#include <timers.h>
#include <constants.h>
#include <Charge.h>

extern WiFiManager wm;
int detectChargerPin = 16;

void setup() {
    Serial.begin(115200);
    Init_WS2812();
    Init_BMP280();    
    Init_OTA();
    InitMillisCalls();
    CalibrationStillPressure();   
}
 
void loop() {
    //AllBlack();
    if (digitalRead(detectChargerPin) == true){
        MangerCharge();
    }else{
        CurrentStatus();
        ChangeMode();
    }
    wm.process();
}

