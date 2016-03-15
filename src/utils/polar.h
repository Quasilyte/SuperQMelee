#pragma once

#include <qglobal.h>
#include <QPointF>

namespace trig {
  constexpr const qreal PI = static_cast<qreal>(3.141592653589793);

  inline qreal angle(const QPointF& pt) {
    return std::atan2(pt.y(), pt.x()) * 180 / PI;
  }

  inline qreal hypo(const QPointF& pt) {
    return std::hypot(pt.x(), pt.y());
  }
}

class Polar {
public:
  static QPointF point(qreal distance, qreal direction) noexcept {
    return QPointF{
      x(distance, direction),
      y(distance, direction)
    };
  }

  static qreal x(qreal distance, qreal direction) noexcept {
    return distance * std::cos(direction * trig::PI / 180);
  }

  static qreal y(qreal distance, qreal direction) noexcept {
    return distance * std::sin(direction * trig::PI / 180);
  }
};
