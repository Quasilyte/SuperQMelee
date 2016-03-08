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

class Vessel: public QObject, public GameItem {
  Q_OBJECT

public slots:
  void gameTickEvent() {
    ctl->operate(nav);
  }

public:
  Vessel(Control *ctl, QString imagePath);

  void left() {
    rotateLeft(rotationSpeed);
  }

  void right() {
    rotateRight(rotationSpeed);
  }

  void onDestroy() {
    if (ctl) {
      GameScene::disconnect(this);
    }
  }

protected:
  Engine *engine;
  Battery *battery;
  Weapon *primary;
  qreal rotationSpeed;

private:
  Control *ctl;
  NavBridge *nav;
  friend NavBridge;
};
