#pragma once

#include "player.h"
#include "route.h"

#include <cassert>
#include <QString>
#include <QtNetwork/QTcpSocket>
#include <qdebug.h>
#include <memory>

#include "net/message.h"
#include "ui/messenger.h"

#include "utils.h"

class Client: public QObject {
  Q_OBJECT

signals:
  void joined(QVector<Player>);
  void gotText(QString);
  void gotNewPlayer(Player);

private slots:
  void connected() {
    // Messenger::info("connected");
  }

  void disconnected() {
    // Messenger::info("disconnected");
  }

  void recvAny() {
    Message in{socket};

    switch (in.getType()) {
    case Message::NEW_PLAYER:
      emit gotNewPlayer(Player::Codec::fromSocket(socket));
      break;
    case Message::TEXT:
      emit gotText(QString{socket->readAll()});
      break;

    default:
      qDebug() << "Client: unknown message type";
    }
  }

  void recvAuthConfirm() {
    Message in{socket};

    if (in.getType() == in.AUTH_CONFIRM && !in.isBroken(socket)) {
      disconnect(socket, SIGNAL(readyRead()), this, SLOT(recvAuthConfirm()));
      connect(socket, SIGNAL(readyRead()), this, SLOT(recvPlayerList()));

      Message out{in.PLAYER_LIST_REQUEST, id};
      socket->write(out.getData(), out.getTotalSize());
    }
  }

  void recvPlayerList() {
    Message in{socket};
    QVector<Player> players;

    if (in.is(Message::PLAYER_LIST)) {
      while (socket->bytesAvailable()) {
        qDebug() << socket->bytesAvailable() << "bytes left";
        players.push_back(Player::Codec::fromSocket(socket));
      }

      disconnect(socket, SIGNAL(readyRead()), this, SLOT(recvPlayerList()));
      connect(socket, SIGNAL(readyRead()), this, SLOT(recvAny()));

      emit joined(players);
    } else {
      qDebug() << "not a player list!";
    }
  }

  void sendAuth() {
    Message in{socket};

    if (in.getType() == in.AUTH_DATA_REQUEST && !in.isBroken(socket)) {
      disconnect(socket, SIGNAL(readyRead()), this, SLOT(sendAuth()));
      connect(socket, SIGNAL(readyRead()), this, SLOT(recvAuthConfirm()));

      Message out{Message::AUTH_DATA, in.getId(), player->getName().length()};
      out.append(player->getName());
      socket->write(out.getData(), out.getTotalSize());

      auth(in.getId());
    }
  }

public:
  Client(QTcpSocket *socket):
  socket{socket}, player{new Player{}} {
    assert(socket != nullptr);
  }

  Client(QTcpSocket *socket, Player *player):
  socket{socket}, player{player} {
    assert(socket != nullptr);
  }

  ~Client() {
    delete player;
  }

  void enable() {
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    // connect(socket, SIGNAL(readyRead()), this, SLOT(readBytes()));
  }

  bool hasAuth() const {
    return id != DEFAULT_ID;
  }

  QTcpSocket* getSocket() const noexcept {
    return socket;
  }

  Player* getPlayer() const noexcept {
    return player;
  }

  void sendText(QString text) {
    assert(hasAuth());

    Message out{Message::TEXT, id, text.length()};
    out.append(text);
    socket->write(out.getData(), out.getTotalSize());
  }

  void joinServer(Route route) {
     socket->connectToHost(route.address, route.port);

     if (!socket->waitForConnected(JOIN_TIMEOUT)) {
       Messenger::warn("Connection timeout");
     }

     connect(this->socket, SIGNAL(readyRead()), this, SLOT(sendAuth()));
  }

  void leaveServer() {
    if (socket->isOpen()) {
      socket->disconnectFromHost();
    }
  }

  void auth(Message::Id id) noexcept {
    assert(!hasAuth());
    this->id = id;
  }

  Message::Id getId() const noexcept {
    assert(hasAuth());
    return id;
  }

private:
  static const unsigned JOIN_TIMEOUT = 1000;
  static const Message::Id DEFAULT_ID = -1;

  Message::Id id = DEFAULT_ID;
  QTcpSocket *socket;
  Player *player;
};
