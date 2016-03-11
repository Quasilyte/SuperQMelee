#pragma once

#include <QTcpSocket>

#include <cassert>

namespace msg {
  class Base;
  enum class Type;
}

class Socket: public QTcpSocket {
public:
  using QTcpSocket::write;

  qint64 write(const msg::Base& msg);
};
