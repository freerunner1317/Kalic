#include <Charge.h>
#include <Arduino.h>
#include <WS2812.h>
#include <FastLED.h>
#include <types.h>
#include <constants.h>
#include <CommonFunc.h>
#include <MyMatrix.h>
#include <MillisTimer.h>
#include <timers.h>


extern MillisTimer Timer_MeasureBattery;
extern MillisTimer Timer_BatteryCharged;
extern MillisTimer Timer_Charging;
extern MillisTimer Timer_CriticalBattery;

float batteryVol = 0;
float batteryVolPrstg = 0;

int LEVEL = 0;
int levelInc = LEVEL;
int brInc = 50;

void MangerCharge() {
  FastLED.clear(); 
  Timer_MeasureBattery.Call(100, []() {
    batteryVol = (float)(4.84 / 1024) * analogRead(A0);
    batteryVolPrstg = (batteryVol - 3) / 1.2;
  });

  if (batteryVol > 4.1) {
    Timer_BatteryCharged.Call(10, []() { 
      int LEVEL = 0;
      int levelInc = LEVEL;
      for (int i = 0; i < WIDTH; i++) {
        for (int j = HEIGHT - 6; j < HEIGHT; j++) {
          drawPixelXY(i, j, CRGB::Green);
        }
      }
      FastLED.setBrightness(brInc--);
      if (brInc == 0) brInc = 50;
      FastLED.show();
    }); 

  } else if (batteryVol > 3.3){
    
    Timer_Charging.Call(300, []() {     
      
      Serial.println(batteryVolPrstg);
      LEVEL = HEIGHT * batteryVolPrstg;
      for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < LEVEL; j++) {
          drawPixelXY(i, j, CRGB::Green);
        }
      }

      for (int i = 0; i < WIDTH; i++) {
        for (int j = LEVEL; j < HEIGHT; j++) {

          if (levelInc == j)
            drawPixelXY(i, j, CRGB::Green);
        }
      }
      if (levelInc < HEIGHT)
        levelInc++;
      else
        levelInc = LEVEL;
      FastLED.setBrightness(2);  
      FastLED.show();   
    });
  } else {
      Timer_CriticalBattery.Call(10, []() {
      int LEVEL = 0;
      int levelInc = LEVEL;
      for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < 4; j++) {
          drawPixelXY(i, j, CRGB::Red);
        }
      }
      FastLED.setBrightness(brInc--);
      if (brInc == 0) brInc = 50;
      FastLED.show();  
    });
  }
}