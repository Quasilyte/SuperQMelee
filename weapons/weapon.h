#pragma once

#include "core/graphics_item.h"
#include "core/game_scene.h"
#include <qdebug.h>
#include "equipment/battery.h"
#include "utils/polar.h"

class Weapon: QObject {
  Q_OBJECT

public slots:
  void gameTickEvent() {
    if (cooldown != 0) {
      cooldown -= 1;
    }
  }

public:
  Weapon(GraphicsItem *gfx, Battery *energySource):
  gfx{gfx},
  energySource{energySource} {
    GameScene::connect(this);
  }

  virtual void fire() = 0;

protected:
  qreal fireX(int offset) const noexcept {
    return gfx->x() + Polar::x(offset, gfx->rotation());
  }

  qreal fireY(int offset) const noexcept {
    return gfx->y() + Polar::y(offset, gfx->rotation());
  }

  GraphicsItem *gfx;
  Battery *energySource;
  int cooldown = 0;
};


