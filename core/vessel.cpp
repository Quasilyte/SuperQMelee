#include "actors/vessel.h"

#include "nav_bridge.h"

NavBridge::NavBridge(Vessel *vessel, Control *ctl):
vessel{vessel}, ctl{ctl} {}

Vessel::Vessel(Control *ctl, QString imagePath):
GameItem{imagePath}, nav{new NavBridge{this, ctl}} {
  setZValue(1);
  nav->enable();
}

void Vessel::onDestroy() {
  nav->disable();
}

void NavBridge::gameTickEvent() {
  ctl->operate(this);
}
