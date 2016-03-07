#pragma once

#include "core/graphics_item.h"
#include "core/game_scene.h"
#include <qdebug.h>
#include "equipment/battery.h"
#include "actors/bullet.h"
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
  GraphicsItem *gfx;
  Battery *energySource;
  int cooldown = 0;
};

class FusionBlaster: public Weapon {
public:
  using Weapon::Weapon;

  void fire() {
    if (0 == cooldown && energySource->hasAtLeast(ENERGY_CONSUMPTION)) {
      energySource->drain(ENERGY_CONSUMPTION);
      cooldown += FIRE_DELAY;

      auto dir = gfx->rotation();
      auto fireX = Polar::x(65, dir);
      auto fireY = Polar::y(65, dir);

      Bullet *bullet = new Bullet{dir, fireX + gfx->x(), fireY + gfx->y()};
      GameScene::connect(bullet);
    }
  }

  static const int ENERGY_CONSUMPTION = 20;
  static const int FIRE_DELAY = 8;
};
