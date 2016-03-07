#pragma once

#include "core/game_scene.h"

#include <cassert>

class Battery: QObject {
  Q_OBJECT

public slots:
  void gameTickEvent() {
    addEnergy(regeneration);
  }

public:
  Battery(int regeneration, int maxEnergy):
  regeneration{regeneration}, maxEnergy{maxEnergy}, energy{maxEnergy} {
    if (regeneration) {
      GameScene::connect(this);
    }
  }

  bool hasAtLeast(int amount) {
    return energy >= amount;
  }

  void drain(int amountToConsume) {
    assert(energy >= amountToConsume);
    energy -= amountToConsume;
  }

  void addEnergy(int amount) {
    energy += amount;

    if (energy > maxEnergy) {
      energy = maxEnergy;
    }
  }

private:
  int regeneration;
  int maxEnergy;
  int energy;
};
