#include <Arduino.h>
#include <WS2812.h>
#include <FastLED.h>
#include <constants.h>
#include <CommonFunc.h>

#define pgm_read_byte(addr)                pgm_read_byte_inlined(addr)

extern CRGB leds[NUM_LEDS];

CRGBPalette16 myPal;

static const uint8_t AVAILABLE_BOID_COUNT = 20U;



float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max){
        return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}

uint16_t NormalizeRange(uint16_t number, uint16_t min, uint16_t max){
    if (number > max)
        return max;
    else if (number < min)
        return min;
    else    
        return number;    
}

void fillMyPal16_2(uint8_t hue, bool isInvert = false){ 
// я бы, конечно, вместо копии функции генерации палитры "_2"
// лучше бы сделал её параметром указатель на массив с базовой палитрой, 
// но я пониятия не имею, как это делается с грёбаным PROGMEM

  int8_t lastSlotUsed = -1;
  uint8_t istart8, iend8;
  CRGB rgbstart, rgbend;
  
  // начинаем с нуля
  if (isInvert)
    //с неявным преобразованием оттенков цвета получаются, как в фотошопе, но для данного эффекта не красиво выглядят
    //rgbstart = CHSV(256 + hue - pgm_read_byte(&MBAuroraColors_arr[0][1]), pgm_read_byte(&MBAuroraColors_arr[0][2]), pgm_read_byte(&MBAuroraColors_arr[0][3])); // начальная строчка палитры с инверсией
    hsv2rgb_spectrum(CHSV(256 + hue - pgm_read_byte(&MBAuroraColors_arr[0][1]), pgm_read_byte(&MBAuroraColors_arr[0][2]), pgm_read_byte(&MBAuroraColors_arr[0][3])), rgbstart);
  else
    //rgbstart = CHSV(hue + pgm_read_byte(&MBAuroraColors_arr[0][1]), pgm_read_byte(&MBAuroraColors_arr[0][2]), pgm_read_byte(&MBAuroraColors_arr[0][3])); // начальная строчка палитры
    hsv2rgb_spectrum(CHSV(hue + pgm_read_byte(&MBAuroraColors_arr[0][1]), pgm_read_byte(&MBAuroraColors_arr[0][2]), pgm_read_byte(&MBAuroraColors_arr[0][3])), rgbstart);
  int indexstart = 0; // начальный индекс палитры
  for (uint8_t i = 1U; i < 5U; i++) { // в палитре @obliterator всего 5 строчек
    int indexend = pgm_read_byte(&MBAuroraColors_arr[i][0]);
    if (isInvert)
      hsv2rgb_spectrum(CHSV(hue + pgm_read_byte(&MBAuroraColors_arr[i][1]), pgm_read_byte(&MBAuroraColors_arr[i][2]), pgm_read_byte(&MBAuroraColors_arr[i][3])), rgbend);
    else
      hsv2rgb_spectrum(CHSV(256 + hue - pgm_read_byte(&MBAuroraColors_arr[i][1]), pgm_read_byte(&MBAuroraColors_arr[i][2]), pgm_read_byte(&MBAuroraColors_arr[i][3])), rgbend);
    istart8 = indexstart / 16;
    iend8   = indexend   / 16;
    if ((istart8 <= lastSlotUsed) && (lastSlotUsed < 15)) {
       istart8 = lastSlotUsed + 1;
       if (iend8 < istart8)
         iend8 = istart8;
    }
    lastSlotUsed = iend8;
    fill_gradient_RGB( myPal, istart8, rgbstart, iend8, rgbend);
    indexstart = indexend;
    rgbstart = rgbend;
  }
}


void dimAll(uint8_t value) {
  //for (uint16_t i = 0; i < NUM_LEDS; i++) {
  //  leds[i].nscale8(value); //fadeToBlackBy
  //}
  // теперь короткий вариант
  nscale8(leds, NUM_LEDS, value);
  //fadeToBlackBy(LEDarray, NUM_LEDS, 255U - value); // эквивалент
}

void blurScreen(fract8 blur_amount, CRGB *LEDarray = leds)
{
  blur2d(LEDarray, WIDTH, HEIGHT, blur_amount);
}


uint8_t myScale8(uint8_t x) { // даёт масштабировать каждые 8 градаций (от 0 до 7) бегунка Масштаб в значения от 0 до 255 по типа синусоиде
  uint8_t x8 = x % 8U;
  uint8_t x4 = x8 % 4U;
  if (x4 == 0U)
    if (x8 == 0U)       return 0U;
    else                return 255U;
  else if (x8 < 4U)     return (1U   + x4 * 72U); // всего 7шт по 36U + 3U лишних = 255U (чтобы восхождение по синусоиде не было зеркально спуску)
//else
                        return (253U - x4 * 72U); // 253U = 255U - 2U
}


uint8_t wrapX(int8_t x){
  return (x + WIDTH)%WIDTH;
}
uint8_t wrapY(int8_t y){
  return (y + HEIGHT)%HEIGHT;
}

// uint16_t getPix(int x, int y) {
//   int matrixW;
//   int matrix = 4;
//   int deviceType = 2;

//   if (matrix == 2 || matrix == 4 || matrix == 6 || matrix == 8)  matrixW = LED_length;
//   else matrixW = LED_width;
//   int thisX, thisY;
//   switch (matrix) {
//     case 1: thisX = x;                    thisY = y;                    break;
//     case 2: thisX = y;                    thisY = x;                    break;
//     case 3: thisX = x;                    thisY = (LED_length - y - 1); break;
//     case 4: thisX = (LED_length - y - 1); thisY = x;                    break;
//     case 5: thisX = (LED_width - x - 1);  thisY = (LED_length - y - 1); break;
//     case 6: thisX = (LED_length - y - 1); thisY = (LED_width - x - 1);  break;
//     case 7: thisX = (LED_width - x - 1);  thisY = y;                    break;
//     case 8: thisX = y;                    thisY = (LED_width - x - 1);  break;
//   }

//   if ( !(thisY & 1) || (deviceType - 2) ) return (thisY * matrixW + thisX);   // чётная строка
//   else return (thisY * matrixW + matrixW - thisX - 1);                            // нечётная строка
// }

// uint32_t getPixColor(int x, int y) {
//   int thisPix = getPix(x, y);
//   if (thisPix < 0 || thisPix >= NUM_LEDS) return 0;
//   return (((uint32_t)leds[thisPix].r << 16) | ((long)leds[thisPix].g << 8 ) | (long)leds[thisPix].b);
// }

// void setPix(int x, int y, CRGB color) {
//   if (y >= 0 && y < LED_length && x >= 0 && x < LED_width) leds[getPix(x, y)] = color;
// }

