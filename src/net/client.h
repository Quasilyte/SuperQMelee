#pragma once

#include "player.h"

#include <cassert>
#include <QString>
#include <QtNetwork/QTcpSocket>
#include <qdebug.h>
#include <memory>

#include "ui/messenger.h"
#include "net/messages/header.h"
#include "net/socket.h"

#include "utils.h"

class Client: public QObject {
  Q_OBJECT

signals:
  void joined(QVector<Player>);
  void leaved();
  void gotPrivateText(QString);
  void gotPublicText(QString);
  void gotNewPlayer(Player);

private slots:
  void onSocketDisconnect() {
    emit leaved();
  }

  void recvAny() {
    msg::Header meta{socket};

   if (meta.getSize() == socket->bytesAvailable() && meta.getId() == id) {
      switch (meta.getType()) {
      case msg::Type::NEW_PLAYER:
        emit gotNewPlayer(Player::Codec::fromSocket(socket));
        break;
      case msg::Type::PUBLIC_TEXT:
        emit gotPublicText(QString{socket->readAll()});
        break;
      case msg::Type::PRIVATE_TEXT:
        emit gotPrivateText(QString{socket->readAll()});
        break;
      default:
        qDebug() << "Client: unknown message type";
      }
    } else {
      qDebug() << "Client: malformed packet";
    }
  }

  void recvAuthConfirm() {
    if (messageCheckout(msg::Type::AUTH_CONFIRM)) {
      rebind(SLOT(recvAuthConfirm()), SLOT(recvPlayerList()));

      socket->write(msg::PlayerListRequest{id});
    }
  }

  void recvPlayerList() {
    if (messageCheckout(msg::Type::PLAYER_LIST)) {
      QVector<Player> players;

      while (socket->bytesAvailable()) {
        players.push_back(Player::Codec::fromSocket(socket));
      }

      rebind(SLOT(recvPlayerList()), SLOT(recvAny()));

      emit joined(players);
    } else {
      qDebug() << "not a player list!";
    }
  }

  void sendAuth() {
    msg::Header meta{socket};
    bool isWhole = meta.getSize() == socket->bytesAvailable();

    if (isWhole && meta.getType() == msg::Type::AUTH_DATA_REQUEST) {
      rebind(SLOT(sendAuth()), SLOT(recvAuthConfirm()));

      player->setTeam(readByte(socket));
      socket->write(msg::AuthData{meta.getId(), player->getName()});

      auth(meta.getId());
    }
  }

public:
  Client(Socket *socket):
  socket{socket}, player{new Player{}} {
    assert(socket != nullptr);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnect()));
  }

  Client(msg::Id id, Socket *socket, Player *player):
  id{id}, socket{socket}, player{player} {
    assert(socket != nullptr);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnect()));
  }

  ~Client() {
    delete player;
  }

  bool hasAuth() const {
    return id != DEFAULT_ID;
  }

  /*
  void setTeam(int team) {
    assert(hasAuth());

    Message out{Message::CHANGE_TEAM, id, 1};
    out.embed(static_cast<Player::Team>(team));
    socket->write(out.getData(), out.getTotalSize());
  }
  */

  void sendPublicText(QString text) {
    assert(hasAuth());
    socket->write(msg::PublicText{id, text});
  }

  void sendPrivateText(QString text) {
    assert(hasAuth());
    socket->write(msg::PrivateText{id, player->getTeam(), text});
  }

  void joinServer(QString address) {
     socket->connectToHost(address, cfg::PORT);

     if (!socket->waitForConnected(JOIN_TIMEOUT)) {
       Messenger::warn("Client: connection timeout");
     }

     connect(socket, SIGNAL(readyRead()), this, SLOT(sendAuth()));
  }

  void leaveServer() {
    if (socket->isOpen()) {
      socket->disconnectFromHost();
    }
  }

  void auth(msg::Id id) noexcept {
    assert(!hasAuth());
    this->id = id;
  }

  msg::Id getId() const noexcept {
    assert(hasAuth());
    return id;
  }
  Socket* getSocket() const noexcept { return socket; }
  Player* getPlayer() const noexcept { return player; }

private:
  static const unsigned JOIN_TIMEOUT = 1000;
  static const msg::Id DEFAULT_ID = -1;

  msg::Id id = DEFAULT_ID;
  Socket *socket;
  Player *player;

  void rebind(const char *toUnbind, const char *toBind) {
    disconnect(socket, SIGNAL(readyRead()), this, toUnbind);
    connect(socket, SIGNAL(readyRead()), this, toBind);
  }

  bool messageCheckout(msg::Type expectedType) {
    msg::Header meta{socket};

    return meta.getSize() == socket->bytesAvailable()
        && meta.getId() == id
        && meta.getType() == expectedType;
  }
};
