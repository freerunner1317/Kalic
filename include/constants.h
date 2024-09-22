#pragma once
#define MODE_AMOUNT           (5U)

#define WIDTH                 (18U)                         // ширина матрицы
#define HEIGHT                (28U)                         // высота матрицы

#define NUM_LEDS HEIGHT * WIDTH
#define DATA_PIN D6
#define MAX_POWER             (400U)

//нормальная
#define MATRIX_TYPE           (0U) // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE      (0U) // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION       (1U) // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз

//спиральная
// #define MATRIX_TYPE           (1U) // тип матрицы: 0 - зигзаг, 1 - параллельная
// #define CONNECTION_ANGLE      (1U) // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
// #define STRIP_DIRECTION       (0U) // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз

#define NUM_LAYERSMAX         (2U)

#define MIN_PRESSURE    9700
#define MAX_PRESSURE    9950
//#define MIDDLE_PRESSURE ((MIN_PRESSURE + MAX_PRESSURE) / 2)
#define MAX_MODES       8  

#define trackingOBJECT_MAX_COUNT (100U)  // максимальное количество отслеживаемых объектов (очень влияет на расход памяти)

const uint8_t CENTER_X_MINOR = (WIDTH / 2) - ((WIDTH - 1) & 0x01);    // центр матрицы по ИКСУ, сдвинутый в меньшую сторону, если ширина чётная
const uint8_t CENTER_Y_MINOR = (HEIGHT / 2) - ((HEIGHT - 1) & 0x01);  // центр матрицы по ИГРЕКУ, сдвинутый в меньшую сторону, если высота чётная
const uint8_t CENTER_X_MAJOR = WIDTH / 2 + (WIDTH % 2);               // центр матрицы по ИКСУ, сдвинутый в большую сторону, если ширина чётная
const uint8_t CENTER_Y_MAJOR = HEIGHT / 2 + (HEIGHT % 2);             // центр матрицы по ИГРЕКУ, сдвинутый в большую сторону, если высота чётная