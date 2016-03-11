#pragma once

#include <QTcpSocket>

#include <cassert>

class Message;

class Socket: public QTcpSocket {
public:
  using QTcpSocket::write;

  qint64 write(const Message& msg);
};
