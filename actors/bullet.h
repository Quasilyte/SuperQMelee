#pragma once

#include "core/game_scene.h"
#include "core/game_item.h"

#include <QMovie>
#include <QLabel>
#include <QTimer>

/*
class Bullet: public QObject {
  Q_OBJECT


};
*/

class Bullet: public QObject {
  Q_OBJECT

public slots:
  void gameTickEvent() {
    // explode();
    foreach (auto item, gfx->collidingItems()) {
      if (!item->isWidget()) {
        GameItem *gameItem = static_cast<GameItem*>(item);
        gameItem->receiveDamage(10);
        explode();
        GameScene::removeItem(this->gfx);
        delete this;
      }
    }

    if (hp) {
      hp -= 1;
      gfx->moveForward(16);
    } else {
      GameScene::removeItem(this->gfx);
      delete this;
    }
  }

  GameItem* getGfx() const {
    return gfx;
  }

public:
  Bullet(qreal direction, qreal x, qreal y):
  gfx{new GameItem{"sprites/dreadnought_bullet1.gif", direction, x, y}} {}

private:
  GameItem *gfx;
  int hp = 20;
  qreal speed = 10;

  void explode() {
    QMovie *movie = new QMovie("sprites/explo.gif");
    QLabel *processLabel = new QLabel();
    processLabel->setMovie(movie);
    movie->start();
    processLabel->setGeometry(gfx->x() - 20, gfx->y() - 20, 80, 80);
    GameScene::addWidget(processLabel);

    QTimer::singleShot(1000, processLabel, SLOT(deleteLater()));
  }
};
