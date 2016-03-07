#pragma once

#include <qglobal.h>

class Polar {
public:
  static qreal x(qreal distance, qreal direction) noexcept {
    return distance * std::cos(direction * PI / 180);
  }

  static qreal y(qreal distance, qreal direction) noexcept {
    return distance * std::sin(direction * PI / 180);
  }

private:
  static constexpr const qreal PI = static_cast<qreal>(3.141592653589793);
};
