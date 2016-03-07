#pragma once

#include "route.h"
#include "client.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QStringList>

// #FIXME: disconnection not handled
class Server: public QObject {
  Q_OBJECT

private slots:
  void gotBytes() {
    auto socket = static_cast<QTcpSocket*>(sender());

    auto message = QString{socket->readAll()};
    qDebug() << message;
    QStringList idAndText = message.split(":");
    if (idAndText.length() != 2) {
      return;
    }
    auto id = idAndText[0].toInt();
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
      qDebug() << "reg client " + text;
      client->auth(id, text);
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
    }
  }

  void acceptConnection() {
    if (connections < MAX_PLAYERS) {
      auto socket = server->nextPendingConnection();
      qDebug() << socket->socketDescriptor() << " got connection";

      clients[connections] = new Client{socket};
      sockets[connections] = socket;

      connect(socket, SIGNAL(readyRead()), this, SLOT(gotBytes()));

      socket->write((QString::number(connections) + ":" + socket->peerAddress().toString()).toUtf8());
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
  QTcpSocket *sockets[MAX_PLAYERS];
  int connections = 0;
  Route route;
};

