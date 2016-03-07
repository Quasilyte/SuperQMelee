#pragma once

#include "core/graphics_item.h"
#include "core/game_scene.h"

class Engine: public QObject {
  Q_OBJECT

public slots:
  void gameTickEvent() {
    if (speed > 0) {
      speed -= 0.1;
      gfx->moveForward(speed);
    }
  }

  void accelerate() {
    speed += acceleration;
    if (speed > maxSpeed) {
      speed = maxSpeed;
    }
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
};
