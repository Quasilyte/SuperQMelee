#pragma once

#include "graphics_item.h"
#include "game_scene.h"

#include <QString>
#include <qdebug.h>

class GameItem;

inline void inflictDamage(GameItem *target, int damage);

class GameItem: public GraphicsItem {
public:
  GameItem(const QPixmap& sprite, qreal x, qreal y):
  GraphicsItem{sprite} {
    GameScene::addItem(this, x, y);
  }

  GameItem(QString imagePath, qreal degrees, qreal x, qreal y):
  GraphicsItem{imagePath, degrees} {
    GameScene::addItem(this, x, y);
  }

  GameItem(QString imagePath):
  GraphicsItem{imagePath} {
    GameScene::addItem(this);
  }

  virtual void onDestroy() {}

  bool isSolid() const noexcept { return solid; }

protected:
  int health;
  bool solid = true;

  friend void inflictDamage(GameItem *target, int damage);
};

inline void inflictDamage(GameItem *target, int damage) {
  target->health -= damage;

  if (target->health <= 0) {
    GameScene::destroy(target);
  }
}
