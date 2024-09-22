#pragma once
#include <Arduino.h>

class MillisTimer {
public:

  void Call(int t, void f()) {
    if (millis() - timer > t) {
      timer = millis();
      f();
    }
  }
  void begin() {
    timer = millis();
  }
private:
  unsigned long timer;
};
