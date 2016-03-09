#pragma once

#include <QTcpSocket>

inline qint8 readByte(QTcpSocket *socket) {
  qint8 byte;
  socket->read(reinterpret_cast<char*>(&byte), 1);

  return byte;
}
