#pragma once

#include <QTcpSocket>

#include <cassert>

class XMessage;

class Socket: public QTcpSocket {
public:
  using QTcpSocket::write;

  qint64 write(const XMessage& msg);
};
