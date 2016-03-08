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
      foreach (auto item, collidingItems()) {
        if (!item->isWidget()) {
          GameItem *gameItem = static_cast<GameItem*>(item);
          inflictDamage(gameItem, damage);
          explode();

          GameScene::destroy(this);
          return;
        }
      }

      lifespan -= 1;
      moveForward(speed);
    } else {
      GameScene::removeItem(this);
      delete this;
    }
  }

public:
  BasicBullet(QString imagePath, qreal direction, qreal x, qreal y):
  GameItem{imagePath, direction, x, y} {}

  void onDestroy() {
    explode();
    delete this;
  }

  virtual void explode() {}

protected:
  qreal speed;
  int damage;
  int lifespan;
};
