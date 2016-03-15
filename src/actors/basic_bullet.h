#pragma once

#include "core/game_scene.h"
#include "core/game_item.h"

#include <QMovie>
#include <QLabel>
#include <QTimer>

class BasicBullet: public QObject, public GameItem {
  Q_OBJECT

public slots:
  void gameTickEvent() {
    if (lifespan) {
      lifespan -= 1;
      moveBy(shift.x(), shift.y());

      foreach (auto item, collidingItems()) {
        GameItem *gameItem = static_cast<GameItem*>(item);

        if (gameItem->isSolid()) {
          inflictDamage(gameItem, damage);
          GameScene::destroy(this);
        }
      }
    } else {
      GameScene::removeItem(this);
      delete this;
    }
  }

public:
  BasicBullet(QString imagePath, qreal direction, qreal x, qreal y, qreal speed):
  GameItem{imagePath, direction, x, y}, shift{Polar::point(speed, direction)} {}

  void onDestroy() {
    explode();
    delete this;
  }

  virtual void explode() {}

protected:
  QPointF shift;
  int damage;
  int lifespan;
};
