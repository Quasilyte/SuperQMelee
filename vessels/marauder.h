#pragma once

#pragma once

#include "actors/vessel.h"
#include "core/game_scene.h"
#include "weapons/blade_launcher.h"

class Marauder: public Vessel {
public:
  Marauder(Control *ctl):
  Vessel{ctl, "sprites/marauder.png"} {
    battery = new Battery{1, 70};
    engine = new Engine{this, 0.5, 4.0};
    rotationSpeed = 3.5;
    primary = new BladeLauncher{this, battery};
    health = 30;
  }

  static const int PRICE = 30;
};
