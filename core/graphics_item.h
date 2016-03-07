#pragma once

#include "utils/polar.h"

#include <QGraphicsPixmapItem>
#include <QRectF>
#include <QPainter>
#include <QPixmap>
#include <QString>

#include <cassert>

class GraphicsItem: public QGraphicsItem {
public:
  GraphicsItem(QString imagePath):
  sprite{QPixmap{imagePath}} {
    initBoundingFromSprite();
  }

  GraphicsItem(QString imagePath, qreal degrees):
  sprite{QPixmap{imagePath}} {
    setRotation(degrees);
    initBoundingFromSprite();
  }

  void moveForward(qreal distance) {
    moveBy(Polar::x(distance, rotation()), Polar::y(distance, rotation()));
  }

  void rotateLeft(qreal degrees) {
    setRotation(rotation() - degrees);
  }

  void rotateRight(qreal degrees) {
    setRotation(rotation() + degrees);
  }

protected:
  QRectF boundingRect() const {
    return bounding;
  }

  void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->drawPixmap(bounding.left(), bounding.top(), sprite);
  }

private:
  QPixmap sprite;
  QRectF bounding;

  void initBoundingFromSprite() {
    qreal width = static_cast<qreal>(sprite.width());
    qreal height = static_cast<qreal>(sprite.height());
    assert(width != 0);
    assert(height != 0);

    bounding = QRectF{-width/2, -height/2, width, height};
  }
};
