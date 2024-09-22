#pragma once

enum Status { INHALE = 0, VENTILATION = 1, STILL = 2 };
enum CalibrationStatus { WRONGDATA = 0, READY = 1};


void CalibrationStillPressure();
void ChangeMode();
void CurrentStatus();








