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

  void handlePlayerListRequest(Socket *socket, const Message& in) {
    socket->write(PlayerList{in.getId(), ClientsIter{clients, connections}});
  }

  void handlePublicText(QTcpSocket *socket, const Message& in) {
    auto sender = clients[in.getId()]->getPlayer();
    auto messageBody = socket->readAll();

    ClientsIter iter{clients, connections};
    while (auto client = iter.next()) {
      client->getSocket()->write(
        PublicText{client->getId(), sender->getName(), messageBody}
      );
    }
  }

  void handlePrivateText(Socket *socket, const Message& in) {
    auto sender = clients[in.getId()]->getPlayer();
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

  void handleChangeTeam(Socket *socket, const Message& in) {
    auto sender = clients[in.getId()];
    sender->setTeam(readByte(socket));
  }

  void handleAuth(Socket *socket, const Message& in) {
    auto newClient = clients[in.getId()];
    auto nameBytes = socket->readAll();

    if (!newClient->hasAuth()) {
      auto newPlayer = newClient->getPlayer();
      newPlayer->setName(QString{nameBytes});

      socket->write(AuthConfirm{in.getId()});

      qDebug() << "given auth to" << newPlayer->getName();

      ClientsIter iter{clients, connections};
      while (auto client = iter.next()) {
        client->getSocket()->write(NewPlayer{client->getId(), newPlayer});
      }

      newClient->auth(in.getId());
    } else {
      qDebug() << "double auth from" << QString{nameBytes};
    }
  }

private slots:
  void gotBytes() {
    auto socket = static_cast<Socket*>(sender());

    Message in{socket};

    if (in.getId() > connections) {
      qDebug() << "Server: bad id" << in.getId();
      return;
    }

    auto client = clients[in.getId()];
    if (client->hasAuth()) {
      qDebug() << "request from" << client->getPlayer()->getName() << socket->socketDescriptor();
    }

    if (in.getSize() == socket->bytesAvailable()) {
      switch (in.getType()) {
      case in.AUTH_DATA:
        handleAuth(socket, in); break;
      case in.PLAYER_LIST_REQUEST:
        handlePlayerListRequest(socket, in); break;
      case in.PUBLIC_TEXT:
        handlePublicText(socket, in); break;
      case in.PRIVATE_TEXT:
        handlePrivateText(socket, in); break;
      case in.CHANGE_TEAM:
        handleChangeTeam(socket, in); break;
      default:
        qDebug() << "Server: unknown message type" << in.getType();
      }
    } else {
      // We are ignoring corrupted/partial packets
      qDebug() << "expected" << in.getSize()
               << "bytes got" << socket->bytesAvailable();
    }

    // socket->read(reinterpret_cast<char*>(&pack), pack.META_DATA_SIZE);
    //pack.dump();
    //quint8 size;
    //qint8 type;
    //qint8 id;
    /*
    qDebug() << "got" << socket->bytesAvailable() << "bytes";

    socket->getChar((char*)&size);
    socket->getChar((char*)&type);
    socket->getChar((char*)&id);
    qDebug() << type << size << id << socket->bytesAvailable();
    */

    /*
    auto message = socket->readAll();
    qDebug() << "server got" << message;

    auto idAndText = message.split(':');
    if (idAndText.length() != 2) {
      return;
    }
    auto id = idAndText[0][0];
    auto text = idAndText[1];
    if (id > MAX_PLAYERS) {
      return;
    }

    auto client = clients[id];
    auto name = client->getPlayer()->getName();
    if (client->hasAuth()) {
      auto msg = ("1|" + name + + ": " + text).toUtf8();
      for (int i = 0; i < connections; ++i) {
        if (clients[i]->hasAuth()) {

          clients[i]->getSocket()->write(msg);
        }
      }
    } else {
      qDebug() << "reg client " + text << "with id " << id;
      client->auth(id);
      client->getPlayer()->setName(text);
      auto msg = ("0|" + text + "|" + socket->peerAddress().toString()).toUtf8();

      for (int i = 0; i < connections; ++i) {
        if (clients[i]->hasAuth()) {
          clients[i]->getSocket()->write(msg);
        }
      }

      for (int i = 0; i < connections; ++i) {
        if (clients[i] != client) {
          auto msg2 = ";0|" + clients[i]->getPlayer()->getName() + "|" + clients[i]->getSocket()->peerAddress().toString();
          client->getSocket()->write(msg2.toUtf8());
        }
      }
    }*/
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

