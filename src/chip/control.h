#pragma once

class NavBridge;

class Control {
public:
  virtual void operate(NavBridge *nav) = 0;
};
