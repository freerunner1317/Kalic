#include <main.h>
#include <Arduino.h>
#include <EffectsManager.h>
#include <FastLED.h>
#include <BMP280.h>
#include <WS2812.h>
#include <Effects/RGB_Prop.h>
#include "Effects/ColorComet.h"
#include "Effects/Fire2020.h"
#include "Effects/FireStrike.h"
#include "Effects/Confiti.h"
#include <Effects/PolarLight.h>
#include <Effects/Lighters.h>
#include <Effects/Plazma.h>
#include <Effects/LightBalls.h>
#include <Effects/ColorRain.h>
#include <types.h>
#include <constants.h>
#include <CommonFunc.h>
#include <MillisTimer.h>
#include <palettes.h>

const TProgmemRGBPalette16 *palette_arr[] = {
  &PartyColors_p,
  &OceanColors_p,
  &LavaColors_p,
  &HeatColors_p,
  &WaterfallColors_p,
  &CloudColors_p,
  &ForestColors_p,
  &RainbowColors_p,
  &RainbowStripeColors_p
};
const TProgmemRGBPalette16 *firePalettes[] = {
  //    &HeatColors_p, // эта палитра уже есть в основном наборе. если в эффекте подключены оба набора палитр, тогда копия не нужна
  &WoodFireColors_p,
  &NormalFire_p,
  &NormalFire2_p,
  &LithiumFireColors_p,
  &SodiumFireColors_p,
  &CopperFireColors_p,
  &AlcoholFireColors_p,
  &RubidiumFireColors_p,
  &PotassiumFireColors_p
};

const TProgmemRGBPalette16 *curPalette = palette_arr[0];

   

ModeType modes[MODE_AMOUNT];
 
Status CurStatus = STILL;
Status PrevStatus = STILL;

CalibrationStatus CalibrationStatusBMP = WRONGDATA;


float bmpValueFloat;
int16_t bmpValueAcceleration = 0;
uint16_t bmpValuePrev;
uint16_t bmpValue;
uint16_t bmpValueInhaleRange;
uint16_t bmpNormalPressure;

uint8_t currentMode = 6;



unsigned long timing;

extern MillisTimer Timer_EffectPeriodCompare;
extern MillisTimer Timer_InhaleEffects;
extern MillisTimer Timer_VentilationEffects;
extern MillisTimer Timer_StillEffects;
extern MillisTimer Timer_Temp;


uint8_t currentFireHeight = HEIGHT / 2;

uint8_t mapedPressureHeight = 0;
uint8_t mapedPressureFPS = 0;
uint8_t mapedPressureBrightness = 0;
uint8_t mapedPressureScale = 0;

float emitterX, emitterY;                          // какие-то динамичные координаты
float speedfactor;                                 // регулятор скорости в эффектах реального времени

uint16_t ff_x, ff_y, ff_z;                      // большие счётчики


float trackingObjectPosX[trackingOBJECT_MAX_COUNT];
float trackingObjectPosY[trackingOBJECT_MAX_COUNT];
float trackingObjectSpeedX[trackingOBJECT_MAX_COUNT];
float trackingObjectSpeedY[trackingOBJECT_MAX_COUNT];
uint8_t trackingObjectHue[trackingOBJECT_MAX_COUNT];

uint8_t noise3d[NUM_LAYERSMAX][WIDTH][HEIGHT];     // двухслойная маска или хранилище свойств в размер всей матрицы

uint8_t line[WIDTH];                               // свойство пикселей в размер строки матрицы

uint8_t step;                 // какой-нибудь счётчик кадров или последовательностей операций
uint8_t pcnt;                                      // какой-то счётчик какого-то прогресса
uint8_t hue, hue2;

bool loadingFlag = true;

void CurrentStatus(){
    bmpValueFloat = BMP280_Filtered();   
    bmpValue = round(bmpValueFloat); 
    bmpValueInhaleRange =  NormalizeRange(bmpValue, MIN_PRESSURE, bmpNormalPressure); // от нуля до max значения
    

    mapedPressureHeight = map(bmpValueInhaleRange, MIN_PRESSURE, bmpNormalPressure, HEIGHT, HEIGHT / 2);
    mapedPressureFPS = map(bmpValueInhaleRange, MIN_PRESSURE, bmpNormalPressure, 2, 60);
    mapedPressureBrightness = map(bmpValueInhaleRange, MIN_PRESSURE, bmpNormalPressure, 255, 15);
    
    Timer_EffectPeriodCompare.Call(220, []() {
        bmpValueAcceleration = (bmpValue - bmpValuePrev);        
        Serial.println(bmpValueAcceleration);
        if (bmpValueAcceleration < -2 && (bmpValue < bmpNormalPressure) && (PrevStatus != VENTILATION)){
            CurStatus = INHALE;  
        }else if(bmpValueAcceleration > 10 && (bmpValue > bmpNormalPressure) && (PrevStatus != INHALE)){
            CurStatus = VENTILATION;
        } else{
            CurStatus = STILL;
        } 
        PrevStatus = CurStatus;
        bmpValuePrev  = bmpValue;        
    });
}

void ChangeMode(){
    if (CurStatus == INHALE){
        Timer_InhaleEffects.Call(mapedPressureFPS, []() {
            FastLED.setBrightness(mapedPressureBrightness);
            switch (currentMode)
            {
                case 0: 
                    Fire2020_effect(mapedPressureHeight);           break;
                case 1:
                    ColorCometRoutine();                            break;
                case 2:
                    mapedPressureHeight = map(bmpValueInhaleRange, MIN_PRESSURE, bmpNormalPressure, HEIGHT, HEIGHT / 4);
                    fireStrike(mapedPressureHeight); 
                    FastLED.setBrightness(mapedPressureBrightness);
                    break; 
                case 3:
                    polarRoutine(bmpValueInhaleRange);
                    FastLED.setBrightness((mapedPressureBrightness) / 2); 
                    break; 
                case 4:
                    //mapedPressureScale = map(bmpValueInhaleRange, MIN_PRESSURE, bmpNormalPressure, 75, 12);
                    LightersRoutine(bmpValueInhaleRange);           break;   
                case 5:
                    Plazma(bmpValueInhaleRange);                    break; 
                case 6:
                    BallsRoutine(bmpValueInhaleRange);              break; 
                case 7:
                    LightBallsRoutine(bmpValueInhaleRange);         break;    
                case 8:
                    ColoredRain();                                  break;                                              
                default:
                break;
            }
            FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER);
            FastLED.show();            
        });
    } else if(CurStatus == VENTILATION){
        //Timer_VentilationEffects.Call(1, []() {
            FastLED.clear();
            for (size_t i = 0; i < 40; i++)
            {
                RGBpropeller();
                FastLED.setBrightness(10);
                FastLED.show();
            }
            AllBlack();        
            if (currentMode++ < MAX_MODES)
                loadingFlag = true;   
            else
                currentMode = 0;
                        
        //});
    }else if(CurStatus == STILL){        
        Timer_StillEffects.Call(40, []() {
            FastLED.setBrightness(mapedPressureBrightness);
            switch (currentMode)
            {
            case 0:            
                Fire2020_effect(mapedPressureHeight);   break;
            case 1:
                ColorCometRoutine();                    break;
            case 2:
                mapedPressureHeight = map(bmpValueInhaleRange, MIN_PRESSURE, bmpNormalPressure, HEIGHT, HEIGHT / 4);                    
                fireStrike(mapedPressureHeight);        
                break;  
            case 3:
                polarRoutine(bmpValueInhaleRange);
                FastLED.setBrightness((mapedPressureBrightness ) / 2); 
                break;  
            case 4:
                LightersRoutine(bmpValueInhaleRange);   break; 
            case 5:
                Plazma(bmpValueInhaleRange);            break; 
            case 6:
                BallsRoutine(bmpValueInhaleRange);      break; 
            case 7:
                LightBallsRoutine(bmpValueInhaleRange); break; 
            case 8:
                ColoredRain();                          break;                                        
            default:
            break;
            }
            //FastLED.setBrightness(30);
            FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER);
            FastLED.show();
        }); 
    }
}














