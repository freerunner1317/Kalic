#pragma once

enum Status { INHALE = 0, VENTILATION = 1, STILL = 2 };
enum CalibrationStatus { WRONGDATA = 0, READY = 1};


void LoopCalibration(float bmpData);
void ChangeMode();
void CurrentStatus();








