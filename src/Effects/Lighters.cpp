#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <types.h>
#include <constants.h>
#include <EffectsManager.h>
#include <MyMatrix.h>
#include <MillisTimer.h>
#include <timers.h>


extern float trackingObjectPosX[trackingOBJECT_MAX_COUNT];
extern float trackingObjectPosY[trackingOBJECT_MAX_COUNT];
extern float trackingObjectSpeedX[trackingOBJECT_MAX_COUNT];
extern float trackingObjectSpeedY[trackingOBJECT_MAX_COUNT];
extern uint8_t trackingObjectHue[trackingOBJECT_MAX_COUNT];

extern uint8_t step;                                      // какой-нибудь счётчик кадров или последовательностей операций


extern MillisTimer Timer_Lighters;

uint8_t Scale;
uint8_t lightersDelay;

extern uint16_t bmpNormalPressure;

extern bool loadingFlag;

void LightersRoutine(uint16_t bmpValueRow)
{

    Scale = map(bmpValueRow, MIN_PRESSURE, bmpNormalPressure, 75, 12);
    lightersDelay = map(bmpValueRow, MIN_PRESSURE, bmpNormalPressure, 5, 60);

    Timer_Lighters.Call(lightersDelay, []() {
        //randomSeed(millis());
        //if (modes[currentMode].Scale > trackingOBJECT_MAX_COUNT) modes[currentMode].Scale = trackingOBJECT_MAX_COUNT;
    if (loadingFlag)
    {    
        loadingFlag = false;    
        for (uint8_t i = 0U; i < trackingOBJECT_MAX_COUNT; i++)
        {
            trackingObjectPosX[i] = random(0, WIDTH * 10);
            trackingObjectPosY[i] = random(0, HEIGHT * 10);
            trackingObjectSpeedX[i] = random(-10, 10);
            trackingObjectSpeedY[i] = random(-10, 10);
            //lightersColor[i] = CHSV(random(0U, 255U), 255U, 255U);
            trackingObjectHue[i] = random8();
        }
    }
    FastLED.clear();
    if (++step > 20U) step = 0U;
    for (uint8_t i = 0U; i < Scale; i++)
    {
        if (step == 0U)                                  // меняем скорость каждые 255 отрисовок
        {
            trackingObjectSpeedX[i] += random(-3, 4);
            trackingObjectSpeedY[i] += random(-3, 4);
            trackingObjectSpeedX[i] = constrain(trackingObjectSpeedX[i], -20, 20);
            trackingObjectSpeedY[i] = constrain(trackingObjectSpeedY[i], -20, 20);
        }

        trackingObjectPosX[i] += trackingObjectSpeedX[i];
        trackingObjectPosY[i] += trackingObjectSpeedY[i];

        if (trackingObjectPosX[i] < 0) trackingObjectPosX[i] = (WIDTH - 1) * 10;
        if (trackingObjectPosX[i] >= (int32_t)(WIDTH * 10)) trackingObjectPosX[i] = 0;

        if (trackingObjectPosY[i] < 0)
        {
            trackingObjectPosY[i] = 0;
            trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
        }
        if (trackingObjectPosY[i] >= (int32_t)(HEIGHT - 1) * 10)
        {
            trackingObjectPosY[i] = (HEIGHT - 1U) * 10;
            trackingObjectSpeedY[i] = -trackingObjectSpeedY[i];
        }
        //drawPixelXY(trackingObjectPosX[i] / 10, trackingObjectPosY[i] / 10, lightersColor[i]);
        drawPixelXY(trackingObjectPosX[i] / 10, trackingObjectPosY[i] / 10, CHSV(trackingObjectHue[i], 255U, 255U));
    }
  });
}

// --------------------------- светлячки со шлейфом ---------------------
#define BALLS_AMOUNT          (1U)                          // количество "шариков"
#define CLEAR_PATH            (1U)                          // очищать путь
#define BALL_TRACK            (1U)                          // (0 / 1) - вкл/выкл следы шариков
#define TRACK_STEP            (20U)                         // длина хвоста шарика (чем больше цифра, тем хвост короче)
int16_t coord[BALLS_AMOUNT][2U];
int8_t vector[BALLS_AMOUNT][2U];
CRGB ballColors[BALLS_AMOUNT];

void BallsRoutine(uint16_t bmpValueRow)
{
  if (loadingFlag)
  {
    loadingFlag = false;
    for (uint8_t j = 0U; j < BALLS_AMOUNT; j++)
    {
      int8_t sign;
      // забиваем случайными данными
      coord[j][0U] = WIDTH / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][0U] = random(4, 15) * sign;
      coord[j][1U] = HEIGHT / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][1U] = random(4, 15) * sign;
      //ballColors[j] = CHSV(random(0, 9) * 28, 255U, 255U);
      // цвет зависит от масштаба
      ballColors[j] = CHSV((22 * (j + 1)) % 256U, 255U, 255U);
    }
  }

  if (!BALL_TRACK)                                          // режим без следов шариков
  {
    FastLED.clear();
  }
  else                                                      // режим со следами
  {
    //fader(TRACK_STEP);
    dimAll(256U - TRACK_STEP);
  }

  // движение шариков
  for (uint8_t j = 0U; j < BALLS_AMOUNT; j++)
  {
    // движение шариков
    for (uint8_t i = 0U; i < 2U; i++)
    {
      coord[j][i] += vector[j][i];
      if (coord[j][i] < 0)
      {
        coord[j][i] = 0;
        vector[j][i] = -vector[j][i];
      }
    }

    if (coord[j][0U] > (int16_t)((WIDTH - 1) * 10))
    {
      coord[j][0U] = (WIDTH - 1) * 10;
      vector[j][0U] = -vector[j][0U];
    }
    if (coord[j][1U] > (int16_t)((HEIGHT - 1) * 10))
    {
      coord[j][1U] = (HEIGHT - 1) * 10;
      vector[j][1U] = -vector[j][1U];
    }
    //leds[XY(coord[j][0U] / 10, coord[j][1U] / 10)] =  ballColors[j];
    drawPixelXYF(coord[j][0U] / 10.0F, coord[j][1U] / 10.0F, ballColors[j]);
  }
}