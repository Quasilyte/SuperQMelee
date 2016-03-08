#pragma once

#include "actors/vessel.h"
#include "core/game_scene.h"
#include "weapons/fusion_blaster.h"

class Dreadnought: public Vessel {
public:
  Dreadnought(Control *ctl):
  Vessel{ctl, "sprites/dreadnought.gif"} {
    battery = new Battery{1, 70};
    engine = new Engine{this, 0.5, 4.0};
    rotationSpeed = 3.5;
    primary = new FusionBlaster{this, battery};
    health = 30;
  }

  static const int PRICE = 30;
};
