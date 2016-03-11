#pragma once

#include "route.h"
#include "client.h"
#include "clients_iter.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QStringList>

#include "utils.h"

// #FIXME: disconnection not handled
class Server: public QObject {
  Q_OBJECT

  void handlePlayerListRequest(Socket *socket, MessageHeader::Id id) {
    socket->write(PlayerList{id, ClientsIter{clients, connections}});
  }

  void handlePublicText(QTcpSocket *socket, MessageHeader::Id id) {
    auto sender = clients[id]->getPlayer();
    auto messageBody = socket->readAll();

    ClientsIter iter{clients, connections};
    while (auto client = iter.next()) {
      client->getSocket()->write(
        PublicText{client->getId(), sender->getName(), messageBody}
      );
    }
  }

  void handlePrivateText(Socket *socket, MessageHeader::Id id) {
    auto sender = clients[id]->getPlayer();
    auto team = readByte(socket);
    auto messageBody = socket->readAll();

    ClientsIter iter{clients, connections};
    while (auto client = iter.next()) {
      if (client->getPlayer()->getTeam() == team) {
        client->getSocket()->write(
          PrivateText{client->getId(), sender->getName(), messageBody}
        );
      }
    }
  }

  /*
  void handleChangeTeam(Socket *socket, const Message& in) {
    auto sender = clients[in.getId()];
    sender->setTeam(readByte(socket));
  }
  */

  void handleAuth(Socket *socket, MessageHeader::Id id) {
    auto newClient = clients[id];
    auto nameBytes = socket->readAll();

    if (!newClient->hasAuth()) {
      auto newPlayer = newClient->getPlayer();
      newPlayer->setName(QString{nameBytes});

      socket->write(AuthConfirm{id});

      qDebug() << "given auth to" << newPlayer->getName();

      ClientsIter iter{clients, connections};
      while (auto client = iter.next()) {
        client->getSocket()->write(NewPlayer{client->getId(), newPlayer});
      }

      newClient->auth(id);
    } else {
      qDebug() << "double auth from" << QString{nameBytes};
    }
  }

private slots:
  void gotBytes() {
    auto socket = static_cast<Socket*>(sender());

    MessageHeader in{socket};
    auto id = in.getId();

    if (id > connections) {
      qDebug() << "Server: bad id" << id;
      return;
    }

    auto client = clients[id];
    if (client->hasAuth()) {
      qDebug() << "request from" << client->getPlayer()->getName() << socket->socketDescriptor();
    }

    if (in.getSize() == socket->bytesAvailable()) {
      switch (in.getType()) {
      case AuthData::TYPE:
        handleAuth(socket, id); break;
      case PlayerListRequest::TYPE:
        handlePlayerListRequest(socket, id); break;
      case PublicText::TYPE:
        handlePublicText(socket, id); break;
      case PrivateText::TYPE:
        handlePrivateText(socket, id); break;
      /*case in.CHANGE_TEAM:
        handleChangeTeam(socket, in); break;*/
      default:
        qDebug() << "Server: unknown message type" << static_cast<int>(in.getType());
      }
    } else {
      // We are ignoring corrupted/partial packets
      qDebug() << "expected" << in.getSize()
               << "bytes got" << socket->bytesAvailable();
    }
  }

  void acceptConnection() {
    if (connections < MAX_PLAYERS) {
      auto socket = static_cast<Socket*>(server->nextPendingConnection());

      auto player = new Player{
        socket->peerAddress().toString(),
        static_cast<Player::Team>(connections)
      };

      clients[connections] = new Client{socket, player};
      connect(socket, SIGNAL(readyRead()), this, SLOT(gotBytes()));

      auto id = connections;
      auto team = connections;
      socket->write(AuthDataRequest{id, team});
      connections += 1;
    } else {
      qDebug() << "rejected connection due to limits";
    }
  }

public:
  Server(Route route):
  route{route} {
    server = new QTcpServer{this};
    server->setMaxPendingConnections(MAX_PLAYERS);

    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    if (!server->listen(QHostAddress::Any, route.DEFAULT_PORT)) {
      qDebug() << server->errorString();
    } else {
      qDebug() << "listening on " << route.address << ":" << route.port;
    }
  }

  void stop() {
    qDebug() << "shutdown server";
    server->close();
    isRunning = false;
  }

  ~Server() {
    /*
    for (unsigned i = 0; i < MAX_PLAYERS; ++i) {
      if (clients[i]) {
        auto socket = clients[i]->getSocket();

        if (socket->isOpen()) {
          socket->disconnect();
        }

        delete clients[i];
      }
    }
    */

    if (isRunning) {
      stop();
    }

    delete server;
  }

private:

  static const int MAX_PLAYERS = 4;

  QTcpServer *server;
  bool isRunning = true;
  Client *clients[MAX_PLAYERS];
  qint8 connections = 0;
  Route route;
};

