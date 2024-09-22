#include <Arduino.h>
#include <CommonFunc.h>
#include <WS2812.h>
#include <types.h>
#include <constants.h>
#include <EffectsManager.h>
#include <MyMatrix.h>
#include <Effects/ColorRain.h>


extern uint16_t ff_x, ff_y, ff_z;                      // большие счётчики
extern uint8_t noise3d[NUM_LAYERSMAX][WIDTH][HEIGHT];     // двухслойная маска или хранилище свойств в размер всей матрицы
extern CRGB leds[NUM_LEDS];
extern uint8_t line[WIDTH];                               // свойство пикселей в размер строки матрицы
extern uint8_t hue, hue2;

void ColoredRain() // внимание! этот эффект заточен на работу бегунка Масштаб в диапазоне от 0 до 255. пока что единственный.
{
  // я хз, как прикрутить а 1 регулятор и длину хвостов и цвет капель
  // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  //rain(60, 200, map8(intensity,5,100), 195, CRGB::Green, false, false, false); // было CRGB::Green

    rain(60, 200, map8(42,5,100), myScale8(45), CRGB::Green, true, true, true);
}



void rain(byte backgroundDepth, byte maxBrightness, byte spawnFreq, byte tailLength, CRGB rainColor, bool splashes, bool clouds, bool storm)
{
  ff_x = random16();
  ff_y = random16();
  ff_z = random16();

  CRGB lightningColor = CRGB(72,72,80);
  CRGBPalette16 rain_p( CRGB::Black, rainColor );
#ifdef SMARTMATRIX
  CRGBPalette16 rainClouds_p( CRGB::Black, CRGB(75,84,84), CRGB(49,75,75), CRGB::Black );
#else
  CRGBPalette16 rainClouds_p( CRGB::Black, CRGB(15,24,24), CRGB(9,15,15), CRGB::Black );
#endif

  //fadeToBlackBy( leds, NUM_LEDS, 255-tailLength);
  dimAll(tailLength);

  // Loop for each column individually
  for (uint8_t x = 0; x < WIDTH; x++) {
    // Step 1.  Move each dot down one cell
    for (uint8_t i = 0; i < HEIGHT; i++) {
      if (noise3d[0][x][i] >= backgroundDepth) {  // Don't move empty cells
        if (i > 0) noise3d[0][x][wrapY(i-1)] = noise3d[0][x][i];
        noise3d[0][x][i] = 0;
      }
    }

    // Step 2.  Randomly spawn new dots at top
    if (random8() < spawnFreq) {
      noise3d[0][x][HEIGHT-1] = random(backgroundDepth, maxBrightness);
    }

    // Step 3. Map from tempMatrix cells to LED colors
    for (uint8_t y = 0; y < HEIGHT; y++) {
      if (noise3d[0][x][y] >= backgroundDepth) {  // Don't write out empty cells
        leds[XY(x,y)] = ColorFromPalette(rain_p, noise3d[0][x][y]);
      }
    }

    // Step 4. Add splash if called for
    if (splashes) {
      // FIXME, this is broken
      byte j = line[x];
      byte v = noise3d[0][x][0];

      if (j >= backgroundDepth) {
        leds[XY(wrapX(x-2),0)] = ColorFromPalette(rain_p, j/3);
        leds[XY(wrapX(x+2),0)] = ColorFromPalette(rain_p, j/3);
        line[x] = 0;   // Reset splash
      }

      if (v >= backgroundDepth) {
        leds[XY(wrapX(x-1),1)] = ColorFromPalette(rain_p, v/2);
        leds[XY(wrapX(x+1),1)] = ColorFromPalette(rain_p, v/2);
        line[x] = v; // Prep splash for next frame
      }
    }

    // Step 5. Add lightning if called for
    if (storm) {
      //uint8_t lightning[WIDTH][HEIGHT];
      // ESP32 does not like static arrays  https://github.com/espressif/arduino-esp32/issues/2567
      uint8_t *lightning = (uint8_t *) malloc(WIDTH * HEIGHT);
      while (lightning == NULL) { Serial.println("lightning malloc failed"); }


      if (random16() < 72) {    // Odds of a lightning bolt
        lightning[scale8(random8(), WIDTH-1) + (HEIGHT-1) * WIDTH] = 255;  // Random starting location
        for(uint8_t ly = HEIGHT-1; ly > 1; ly--) {
          for (uint8_t lx = 1; lx < WIDTH-1; lx++) {
            if (lightning[lx + ly * WIDTH] == 255) {
              lightning[lx + ly * WIDTH] = 0;
              uint8_t dir = random8(4);
              switch (dir) {
                case 0:
                  leds[XY(lx+1,ly-1)] = lightningColor;
                  lightning[(lx+1) + (ly-1) * WIDTH] = 255; // move down and right
                break;
                case 1:
                  leds[XY(lx,ly-1)] = CRGB(128,128,128); // я без понятия, почему у верхней молнии один оттенок, а у остальных - другой
                  lightning[lx + (ly-1) * WIDTH] = 255;    // move down
                break;
                case 2:
                  leds[XY(lx-1,ly-1)] = CRGB(128,128,128);
                  lightning[(lx-1) + (ly-1) * WIDTH] = 255; // move down and left
                break;
                case 3:
                  leds[XY(lx-1,ly-1)] = CRGB(128,128,128);
                  lightning[(lx-1) + (ly-1) * WIDTH] = 255; // fork down and left
                  leds[XY(lx-1,ly-1)] = CRGB(128,128,128);
                  lightning[(lx+1) + (ly-1) * WIDTH] = 255; // fork down and right
                break;
              }
            }
          }
        }
      }
      free(lightning);
    }

    // Step 6. Add clouds if called for
    if (clouds) {
      uint16_t noiseScale = 250;  // A value of 1 will be so zoomed in, you'll mostly see solid colors. A value of 4011 will be very zoomed out and shimmery
      //const uint16_t cloudHeight = (HEIGHT*0.2)+1;
      const uint8_t cloudHeight = HEIGHT * 0.4 + 1; // это уже 40% c лишеним, но на высоких матрицах будет чуть меньше

      // This is the array that we keep our computed noise values in
      //static uint8_t noise[WIDTH][cloudHeight];
      static uint8_t *noise = (uint8_t *) malloc(WIDTH * cloudHeight);
      
      while (noise == NULL) { Serial.println("noise malloc failed"); }
      int xoffset = noiseScale * x + hue;

      for(uint8_t z = 0; z < cloudHeight; z++) {
        int yoffset = noiseScale * z - hue;
        uint8_t dataSmoothing = 192;
        uint8_t noiseData = qsub8(inoise8(ff_x + xoffset,ff_y + yoffset,ff_z),16);
        noiseData = qadd8(noiseData,scale8(noiseData,39));
        noise[x * cloudHeight + z] = scale8( noise[x * cloudHeight + z], dataSmoothing) + scale8( noiseData, 256 - dataSmoothing);
        nblend(leds[XY(x,HEIGHT-z-1)], ColorFromPalette(rainClouds_p, noise[x * cloudHeight + z]), (cloudHeight-z)*(250/cloudHeight));
      }
      ff_z ++;
    }
  }
}