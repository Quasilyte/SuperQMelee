#pragma once

#include "core/game_item.h"
#include "chip/command.h"
#include "weapons/weapon.h"
#include "equipment/battery.h"
#include "equipment/engine.h"
#include "utils/polar.h"
#include "chip/control.h"

#include <QString>
#include <qdebug.h>

class DirectControl;

class Vessel: public GameItem {
public:
  Vessel(Control *ctl, QString imagePath);

  void left() {
    rotateLeft(rotationSpeed);
  }

  void right() {
    rotateRight(rotationSpeed);
  }

  void onDestroy();

  NavBridge* getNavBridge() const { return nav; }

protected:
  Engine *engine;
  Battery *battery;
  Weapon *primary;
  qreal rotationSpeed;

private:
  NavBridge *nav;
  friend NavBridge;
};
