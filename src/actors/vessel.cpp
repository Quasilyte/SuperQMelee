#include "vessel.h"

#include "nav_bridge.h"

Vessel::Vessel(Control *ctl, QString imagePath):
GameItem{imagePath}, ctl{ctl}, nav{new NavBridge{this}} {
  setZValue(1);

  if (ctl) {
    GameScene::connect(this);
  }
}
