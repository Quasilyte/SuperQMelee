#pragma once

#include "weapon.h"
#include "actors/basic_bullet.h"
#include "actors/special_effect.h"

class FusionBlaster: public Weapon {
  class Bullet: public BasicBullet {
  public:
    Bullet(qreal direction, qreal x, qreal y):
    BasicBullet{"sprites/dreadnought_bullet1.gif", direction, x, y, 20.0} {
      health = 6;
      damage = 6;
      lifespan = 20;
    }

  private:
    void explode() {
      SpecialEffect::spawn(x(), y());
      /*
      QMovie *movie = new QMovie("sprites/explo.gif");
      QLabel *processLabel = new QLabel();
      processLabel->setMovie(movie);
      movie->start();
      processLabel->setGeometry(x() - 20, y() - 20, 80, 80);
      GameScene::addWidget(processLabel);

      QTimer::singleShot(1000, processLabel, SLOT(deleteLater()));
      */
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
