#pragma once

#include "graphics_item.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>

#include <cassert>

inline unsigned long long operator"" _fps(unsigned long long fps) {
  return 1000 / fps;
}

/*!
 * \brief Responsible for managing game event pool
 */
class GameScene {
public:
  static void addWidget(QWidget *widget) {
    scene->addWidget(widget);
  }

  static void addItem(QGraphicsItem *item) {
    scene->addItem(item);
  }

  static void addItem(QGraphicsItem *item, qreal x, qreal y) {
    addItem(item);
    item->setPos(x, y);
  }

  static void removeItem(QGraphicsItem *item) {
    scene->removeItem(item);
  }

  static void connect(QObject *item) {
    QObject::connect(timer, SIGNAL(timeout()), item, SLOT(gameTickEvent()));
  }

  static void disconnect(QObject *item) {
    QObject::disconnect(timer, SIGNAL(timeout()), item, SLOT(gameTickEvent()));
  }

  static void stop() {
    assert(scene != nullptr);
    assert(timer != nullptr);

    delete scene, delete timer;
    scene = nullptr, timer = nullptr;
  }

  static void start(QGraphicsScene *scene) {
    assert(GameScene::scene == nullptr);
    assert(timer == nullptr);

    GameScene::scene = scene;

    timer = new QTimer{};
    timer->start(30_fps);
  }

private:
  static QGraphicsScene *scene;
  static QTimer *timer;
};
