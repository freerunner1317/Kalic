#include <main.h>
#include <Arduino.h>
#include <EffectsManager.h>
#include <FastLED.h>
#include <BMP280.h>
#include <WS2812.h>
#include <RGB_PropEffect.h>
#include "Effects/ColorCometEffect.h"

Status CurStatus = STILL;
Status PrevStatus = STILL;

CalibrationStatus CalibrationStatusBMP = WRONGDATA;

float bmpDataAcceleration = 0;
float PrevValueBmp;
float NormalPressure;
float bmpValue;

uint8_t LedMode = 3;

int CalibrationINC = 0;

unsigned long timing;


void CurrentStatus(){
    LedMode = 0;

    bmpValue = BMP280_Filtered();
    LoopCalibration(bmpValue);

    if (millis() - timing > 75){ 
        bmpDataAcceleration = (bmpValue - PrevValueBmp) * 10;        

        if (bmpDataAcceleration < -8 && (bmpValue < NormalPressure) && (PrevStatus != VENTILATION)){
            CurStatus = INHALE;  
        }else if(bmpDataAcceleration > 20 && (bmpValue > NormalPressure) && (PrevStatus != INHALE)){
            CurStatus = VENTILATION;
        } else{
            CurStatus = STILL;
        } 

        PrevStatus = CurStatus;

        Serial.println(CurStatus); 

        timing = millis(); 
        PrevValueBmp  = bmpValue;        
    }
}

void ChangeMode(){
    if (CurStatus == INHALE){
        switch (LedMode)
        {
            case 0:
                break;
            case 1:
                break;
            case 2: 
                break; 
            case 3:
                break;  
            default:
            break;
        }
    } else if(CurStatus == VENTILATION){
        for (size_t i = 0; i < 40; i++)
        {
            RGBpropeller();
        }
        AllBlack();        
        if (LedMode == 4)
            LedMode = 0;
        else
            LedMode++;            

    }else if(CurStatus == STILL){
        switch (LedMode)
        {
        case 0:        
            break;
        case 1:
             break;
        case 2:       
            break;  
        case 3:

            break;   
        default:
        break;
        }
    }
}


void LoopCalibration(float bmpData){
    if (abs(PrevValueBmp - bmpData) <= 3){
        CalibrationINC++;
    }
    if (CalibrationINC == 150){
        NormalPressure = bmpData;
        CalibrationINC = 0;
        //Serial.println(normalPressure);
    }
    if (abs(NormalPressure - bmpData) <= 3)
    {
        CalibrationStatusBMP = READY;
    }
    //Serial.println(bmpData - NormalPressure);
}












