#pragma once

#include "core/game_scene.h"
#include "actors/vessel.h"

class Control;

class NavBridge: public QObject {
  Q_OBJECT

public slots:
  void gameTickEvent();

public:
  NavBridge(Vessel *vessel, Control *ctl);

  void usePrimary() {
    vessel->primary->fire();
  }

  void forward() {
    vessel->engine->accelerate();
  }

  void left() {
    vessel->left();
  }

  void right() {
    vessel->right();
  }

  void enable() {
    GameScene::connect(this);
  }

  void disable() {
    GameScene::disconnect(this);
  }

private:
  Vessel *vessel;
  Control *ctl;
};
