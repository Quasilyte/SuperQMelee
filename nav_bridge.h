#pragma once

#include "actors/vessel.h"

class Control;

class NavBridge {
public:
  NavBridge(Vessel *vessel):
  vessel{vessel} {}

  void usePrimary() {
    vessel->primary->fire();
  }

  void forward() {
    vessel->engine->accelerate();
  }

  void left() {
    vessel->left();
  }

  void right() {
    vessel->right();
  }

private:
  Vessel *vessel;
};
