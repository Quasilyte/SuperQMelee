#pragma once

#include "player.h"
#include "route.h"

#include <cassert>
#include <QString>
#include <QtNetwork/QTcpSocket>
#include <qdebug.h>
#include <memory>

#include "ui/messenger.h"

class Client: public QObject {
  Q_OBJECT

signals:
  void broadcast(QString message);

private slots:
  void connected() {
    // Messenger::info("connected");
  }

  void disconnected() {
    // Messenger::info("disconnected");
  }

  void readBytes() {
    auto bytes = socket->readAll();
    emit broadcast(QString{bytes});
    /*
    auto bytes = socket->readAll();
    emit broadcast(QString{bytes});
    */
  }

public:
  Client(QTcpSocket *socket):
  socket{socket},
  player{new Player{}} {
  }

  void bindEvents() {
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readBytes()));
  }

  ~Client() {
    delete player;
  }

  int getId() const noexcept {
    return id;
  }

  bool hasAuth() const {
    return id != DEFAULT_ID;
  }

  void auth(int id, QString name) noexcept {
    assert(!hasAuth());

    this->id = id;
    player->setName(name);
  }

  QTcpSocket* getSocket() const noexcept {
    return socket;
  }

  Player* getPlayer() const noexcept {
    return player;
  }

  void sendMessage(QString message) {
    socket->write(message.toUtf8());
  }

  void joinServer(Route route) {
     socket->connectToHost(route.address, route.port);

     if (!socket->waitForConnected(JOIN_TIMEOUT)) {
       // #FIXME: handle error
       Messenger::warn("Connection timeout");
     }
  }

  void leaveServer() {
    if (socket->isOpen()) {
      socket->disconnectFromHost();
    }
  }

private:
  static const unsigned JOIN_TIMEOUT = 1000;
  static const int DEFAULT_ID = -1;

  int id = DEFAULT_ID;
  // std::unique_ptr<Player> player;
  QTcpSocket *socket;
  Player *player;
};
