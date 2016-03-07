#pragma once

#include "graphics_item.h"
#include "game_scene.h"

#include <QString>
#include <qdebug.h>

class GameItem: public GraphicsItem {
public:
  GameItem(QString imagePath, qreal degrees, qreal x, qreal y):
  GraphicsItem{imagePath, degrees} {
    GameScene::addItem(this, x, y);
  }

  GameItem(QString imagePath):
  GraphicsItem{imagePath} {
    GameScene::addItem(this);
  }

  void receiveDamage(int damage) {
    health -= damage;

    if (health <= 0) {
      GameScene::removeItem(this);
      onDestroy();
    }
  }

  virtual void onDestroy() {}

protected:
  int health;
};
