#pragma once

#include "control.h"

class RandomControl: public Control {
public:
  void operate(NavBridge *nav) {
    nav->left();
    nav->usePrimary();
  }
};
