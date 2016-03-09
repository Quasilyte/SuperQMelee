#pragma once

#include "socket.h"

inline qint8 readByte(Socket *socket) {
  qint8 byte;
  socket->read(reinterpret_cast<char*>(&byte), 1);

  return byte;
}
