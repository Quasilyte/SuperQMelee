#pragma once

#include "human_control.h"
#include "nav_bridge.h"

class DirectControl: public HumanControl {
public:
  void operate(NavBridge *nav) {
    auto bits = cmd.getBits();

    if (bits & ROTATE_LEFT) { nav->left(); }
    if (bits & ROTATE_RIGHT) { nav->right(); }
    if (bits & FIRE1) { nav->usePrimary(); }
    if (bits & THRUST) { nav->forward(); }
  }
};
