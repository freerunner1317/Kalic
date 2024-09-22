#pragma once

#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include "GyverFilters.h"
#include <MillisTimer.h>

float BMP280_Filtered();

void Init_BMP280();
void bmpCalibrationRange();