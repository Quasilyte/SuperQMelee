#pragma once

#include "control.h"
#include "command.h"
#include "nav_bridge.h"

class HumanControl: public Control {
public:
  void setCommand(qint32 bits) {
    cmd.setBits(bits);
  }

  void handleKeyRelease(int keyCode) {
    switch (keyCode) {
    case Qt::Key_BracketLeft: cmd.deactivate(FIRE1); break;
    case Qt::Key_BraceRight: cmd.deactivate(FIRE2); break;
    case Qt::Key_W: cmd.deactivate(THRUST); break;
    case Qt::Key_A: cmd.deactivate(ROTATE_LEFT); break;
    case Qt::Key_D: cmd.deactivate(ROTATE_RIGHT); break;
    }
  }

  void handleKeyPress(int keyCode) {
    switch (keyCode) {
    case Qt::Key_BracketLeft: cmd.activate(FIRE1); break;
    case Qt::Key_BraceRight: cmd.activate(FIRE2); break;
    case Qt::Key_W: cmd.activate(THRUST); break;
    case Qt::Key_A: cmd.activate(ROTATE_LEFT); break;
    case Qt::Key_D: cmd.activate(ROTATE_RIGHT); break;
    }
  }

  Command getCommand() {
    return cmd;
  }

  void operate(NavBridge *nav) {
    auto bits = cmd.getBits();

    if (bits & ROTATE_LEFT) { nav->left(); }
    if (bits & ROTATE_RIGHT) { nav->right(); }
    if (bits & FIRE1) { nav->usePrimary(); }
    if (bits & THRUST) { nav->forward(); }
  }

protected:
  static const qint32 IDLE = 0;
  static const qint32 THRUST = 1 << 0;
  static const qint32 ROTATE_LEFT = 1 << 1;
  static const qint32 ROTATE_RIGHT = 1 << 2;
  static const qint32 FIRE1 = 1 << 3;
  static const qint32 FIRE2 = 1 << 4;

  Command cmd;
};
