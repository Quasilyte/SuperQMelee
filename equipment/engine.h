#pragma once

#include "core/graphics_item.h"
#include "core/game_scene.h"

#include <qdebug.h>
#include <math.h>
#include <algorithm>

class Engine: public QObject {
  Q_OBJECT

public slots:
  void gameTickEvent() {
    qreal dxNormalized = normalize(passiveShift.x());
    qreal dyNormalized = normalize(passiveShift.y());

    if (dxNormalized || dyNormalized) {
      gfx->moveBy(dxNormalized, dyNormalized);
    }
  }

  void accelerate() {
    auto activeShift = Polar::point(acceleration, gfx->rotation());
    auto calculatedShift = activeShift + passiveShift;

    qreal calculatedSpeed = std::min(maxSpeed, trig::hypo(calculatedShift));

    passiveShift = Polar::point(calculatedSpeed, trig::angle(calculatedShift));
  }

public:
  Engine(GraphicsItem *gfx, qreal acceleration, qreal maxSpeed):
  gfx{gfx}, acceleration{acceleration}, maxSpeed{maxSpeed} {
    GameScene::connect(this);
  }

private:
  static const qreal MIN_OFFSET;
  static const qreal MOVE_THRESHOLD;

  GraphicsItem *gfx;
  qreal acceleration;
  qreal maxSpeed;

  QPointF passiveShift;
  qreal dir;

  qreal normalize(qreal offset) {
    if (offset >= MOVE_THRESHOLD) {
      return std::max(MIN_OFFSET, offset);
    } else if (offset <= -MOVE_THRESHOLD) {
      return std::min(-MIN_OFFSET, offset);
    } else {
      return 0.0;
    }
  }
};
