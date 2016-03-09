#pragma once

#include <QTcpSocket>

#include "net/messages/header.h"
#include <cassert>

class Socket: public QTcpSocket {
public:
  using QTcpSocket::write;

  qint64 write(const XMessage& msg) {
    return QTcpSocket::write(msg.getData(), msg.getDataSize());
  }
};
