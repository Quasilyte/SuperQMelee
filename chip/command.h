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

  qint32 getBits() const noexcept {
    return bits;
  }

  void setBits(qint32 bits) {
    this->bits = bits;
  }
  
private:
  qint32 bits;
};
