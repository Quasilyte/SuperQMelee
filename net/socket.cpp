#include "socket.h"

#include "messages/header.h"

qint64 Socket::write(const XMessage &msg) {
  return QTcpSocket::write(msg.getData(), msg.getDataSize());
}
