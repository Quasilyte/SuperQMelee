#pragma once

#include "core/game_item.h"

#include "sprite/animated_sprite.h"

#include <QtGlobal>

class SpecialEffect: public QObject, public GameItem {
  Q_OBJECT

public slots:
  void gameTickEvent() {
    setSprite(anim.nextFrame());
    update();
    inflictDamage(this, 1);
  }

public:
  SpecialEffect(AnimatedSprite anim, qreal x, qreal y):
  GameItem{anim.nextFrame(), x, y}, anim{anim} {
    GameScene::connect(this);
    solid = false;
    health = anim.getFrameCount();
  }

  void onDestroy() {
    delete this;
  }

  SpecialEffect(SpecialEffect&&) = default;

  static void spawn(qreal x, qreal y) {
    // new SpecialEffect{new AnimatedSprite{imagePath, 64, 64}, x, y};
    auto sprite = AnimatedSprite{
      AnimatedPixmaps::get(AnimatedPixmaps::FUSION_BLASTER_EXPLOSION)
    };
    new SpecialEffect{sprite, x, y};
  }

private:
  AnimatedSprite anim;
};

/*
QMovie *movie = new QMovie("sprites/explo.gif");
QLabel *processLabel = new QLabel();
processLabel->setMovie(movie);
movie->start();
processLabel->setGeometry(x() - 20, y() - 20, 80, 80);
GameScene::addWidget(processLabel);

QTimer::singleShot(1000, processLabel, SLOT(deleteLater()));
*/
