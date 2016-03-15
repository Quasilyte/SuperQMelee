#pragma once

#pragma once

#include "weapon.h"
#include "actors/basic_bullet.h"

class BladeLauncher: public Weapon {
  class Bullet: public BasicBullet {
  public:
    Bullet(qreal direction, qreal x, qreal y):
    BasicBullet{"sprites/blade.png", direction, x, y, 16.0} {
      health = 6;
      damage = 4;
      lifespan = 20;
    }
  };

public:
  using Weapon::Weapon;

  void fire() {
    if (0 == cooldown && energySource->hasAtLeast(ENERGY_CONSUMPTION)) {
      energySource->drain(ENERGY_CONSUMPTION);
      cooldown += FIRE_DELAY;

      auto *bullet = new Bullet{gfx->rotation(), fireX(70), fireY(70)};
      GameScene::connect(bullet);
    }
  }

  static const int ENERGY_CONSUMPTION = 20;
  static const int FIRE_DELAY = 8;
};
