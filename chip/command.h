#pragma once

#include <QtGlobal>

class Command {
public:  
  void activate(qint32 mask) noexcept {
    bits |= mask;
  }

  void deactivate(qint32 mask) noexcept {
    bits &= ~mask;
  }

  qint32 getBits() const {
    return bits;
  }
  
private:
  qint32 bits;
};
