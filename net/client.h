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
#include "net/messages/header.h"
#include "net/socket.h"

#include "utils.h"

class Client: public QObject {
  Q_OBJECT

signals:
  void joined(QVector<Player>);
  void gotPrivateText(QString);
  void gotPublicText(QString);
  void gotNewPlayer(Player);

private slots:
  void connected() {
    // Messenger::info("connected");
  }

  void disconnected() {
    // Messenger::info("disconnected");
  }

  void recvAny() {
    MessageHeader in{socket};

    switch (in.getType()) {
    case NewPlayer::TYPE:
      emit gotNewPlayer(Player::Codec::fromSocket(socket));
      break;
    case PublicText::TYPE:
      emit gotPublicText(QString{socket->readAll()});
      break;
    case PrivateText::TYPE:
      emit gotPrivateText(QString{socket->readAll()});
      break;
    default:
      qDebug() << "Client: unknown message type";
    }
  }

  void recvAuthConfirm() {
    MessageHeader in{socket};

    if (in.getType() == AuthConfirm::TYPE) {
      rebind(SLOT(recvAuthConfirm()), SLOT(recvPlayerList()));
      socket->write(PlayerListRequest{id});
    }
  }

  void recvPlayerList() {
    MessageHeader in{socket};
    QVector<Player> players;

    if (in.getType() == PlayerList::TYPE) {
      while (socket->bytesAvailable()) {
        qDebug() << socket->bytesAvailable() << "bytes left";
        players.push_back(Player::Codec::fromSocket(socket));
      }

      rebind(SLOT(recvPlayerList()), SLOT(recvAny()));

      emit joined(players);
    } else {
      qDebug() << "not a player list!";
    }
  }

  void sendAuth() {
    MessageHeader in{socket};

    if (in.getType() == AuthDataRequest::TYPE) {
      rebind(SLOT(sendAuth()), SLOT(recvAuthConfirm()));

      player->setTeam(readByte(socket));
      socket->write(AuthData{in.getId(), player->getName()});

      auth(in.getId());
    }
  }

public:
  Client(Socket *socket):
  socket{socket}, player{new Player{}} {
    assert(socket != nullptr);
  }

  Client(Socket *socket, Player *player):
  socket{socket}, player{player} {
    assert(socket != nullptr);
  }

  ~Client() {
    delete player;
  }

  bool hasAuth() const {
    return id != DEFAULT_ID;
  }

  Socket* getSocket() const noexcept {
    return socket;
  }

  Player* getPlayer() const noexcept {
    return player;
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
    socket->write(PublicText{id, text});
  }

  void sendPrivateText(QString text) {
    assert(hasAuth());
    socket->write(PrivateText{id, player->getTeam(), text});
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

  void auth(MessageHeader::Id id) noexcept {
    assert(!hasAuth());
    this->id = id;
  }

  MessageHeader::Id getId() const noexcept {
    assert(hasAuth());
    return id;
  }

private:
  static const unsigned JOIN_TIMEOUT = 1000;
  static const MessageHeader::Id DEFAULT_ID = -1;

  MessageHeader::Id id = DEFAULT_ID;
  Socket *socket;
  Player *player;

  void rebind(const char *toUnbind, const char *toBind) {
    disconnect(socket, SIGNAL(readyRead()), this, toUnbind);
    connect(socket, SIGNAL(readyRead()), this, toBind);
  }
};
