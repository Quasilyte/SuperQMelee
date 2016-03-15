#pragma once

#include <QString>
#include "net/socket.h"
#include "net/client.h"

#include "server.h"
#include "utils/defs.h"

#include <cassert>

// #FIXME: rename to LobbyController
class ChatController: public QObject {
  Q_OBJECT

private slots:
  void handleIncomingMessage(msg::Header info, Socket *socket, Client *client) {
    switch (info.getType()) {
    case msg::Type::AUTH_DATA:
      handleAuth(socket, client); break;
    case msg::Type::PLAYER_LIST_REQUEST:
      handlePlayerListRequest(socket, client); break;
    case msg::Type::PUBLIC_TEXT:
      handlePublicText(socket, client); break;
    case msg::Type::PRIVATE_TEXT:
      handlePrivateText(socket, client); break;
    /*case in.CHANGE_TEAM:
      handleChangeTeam(socket, in); break;*/
    default:
      qDebug() << "ChatController: unknown message type"
               << static_cast<int>(info.getType());
    }
  }

public:
  ChatController(Server *server): server{server} {
    from(server, SIGNAL(incomingMessage(msg::Header, Socket*, Client*)))
    to(this, SLOT(handleIncomingMessage(msg::Header,Socket*,Client*)));
  }

private:
  Server *server;

  void handleAuth(Socket *socket, Client *newClient) {
    auto nameBytes = socket->readAll();

    auto newPlayer = newClient->getPlayer();
    newPlayer->setName(QString{nameBytes});

    socket->write(msg::AuthConfirm{newClient->getId()});

    qDebug() << "ChatServer: given auth to"
             << newPlayer->getName();

    auto iter = server->getClientsIter();
    while (auto client = iter.next()) {
      if (client->getId() != newClient->getId()) {
        client->getSocket()->write(msg::NewPlayer{client->getId(), newPlayer});
      }
    }
  }

  void handlePublicText(QTcpSocket *socket, Client *client) {
    auto sender = client->getPlayer();
    auto messageBody = socket->readAll();

    auto iter = server->getClientsIter();
    while (auto client = iter.next()) {
      client->getSocket()->write(
        msg::PublicText{client->getId(), sender->getName(), messageBody}
      );
    }
  }

  void handlePlayerListRequest(Socket *socket, Client *client) {
    socket->write(msg::PlayerList{client->getId(), server->getClientsIter()});
  }

  void handlePrivateText(Socket *socket, Client *client) {
    auto sender = client->getPlayer();
    auto team = readByte(socket);
    auto messageBody = socket->readAll();

    auto iter = server->getClientsIter();
    while (auto client = iter.next()) {
      if (client->getPlayer()->getTeam() == team) {
        client->getSocket()->write(
          msg::PrivateText{client->getId(), sender->getName(), messageBody}
        );
      }
    }
  }
};


