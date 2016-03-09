#pragma once

#include <QTcpSocket>

#include <cassert>

template<class PLAYER_T> class Connection {
public:
  Connection(QTcpSocket *socket):
  socket{socket}, player{new PLAYER_T{}} {
    assert(socket != nullptr);
    socket->ungetChar();
  }

  QTcpSocket* getSocket() const noexcept {
    return socket;
  }

  PLAYER_T* getPlayer() const noexcept {
    return player;
  }

  ~Connection() {
    delete player;
  }

protected:
  QTcpSocket *socket;
  PLAYER_T *player;
};
