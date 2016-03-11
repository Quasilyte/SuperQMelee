#pragma once

#include "core/graphics_item.h"
#include "core/game_scene.h"

#include <qdebug.h>
#include <math.h>

class Engine: public QObject {
  Q_OBJECT

public slots:
  void gameTickEvent() {
    if (speed > 0) {
      speed -= 0.1;
    }

    if (abs(dx) > 0.05 || abs(dy) > 0.05) {
      gfx->moveBy(dx, dy);
    }
  }

  void accelerate() {
    speed += acceleration;
    if (speed > maxSpeed) {
      speed = maxSpeed;
    }

    dx = Polar::x(speed, gfx->rotation());
    dy = Polar::y(speed, gfx->rotation());
  }

public:
  Engine(GraphicsItem *gfx, qreal acceleration, qreal maxSpeed):
  gfx{gfx}, acceleration{acceleration}, maxSpeed{maxSpeed} {
    GameScene::connect(this);
  }

private:
  GraphicsItem *gfx;
  qreal acceleration;
  qreal maxSpeed;
  qreal speed = 0.0;

  qreal dx;
  qreal dy;
};
