#pragma once

#include "net/config.h"
#include <QtNetwork/QTcpServer>
#include <QString>
#include "net/socket.h"
#include "net/client.h"

class Server: public QObject {
  Q_OBJECT

signals:
  void incomingMessage(msg::Header, Socket*, Client*);

private slots:
  void recvMessage() {
    auto socket = static_cast<Socket*>(sender());

    msg::Header info{socket};
    auto id = info.getId();

    if (id > connections) {
      qDebug() << "Server: decline id" << id;
    } else if (info.getSize() == socket->bytesAvailable()) {
      emit incomingMessage(info, socket, clients[id]);
    } else {
      // We are ignoring corrupted/partial packets
      qDebug() << "Server: expected" << info.getSize()
               << "bytes got" << socket->bytesAvailable();
    }
  }

  void handleNewConnection() {
    auto socket = static_cast<Socket*>(server->nextPendingConnection());

    if (connections < cfg::PLAYERS_MAX) {
      acceptConnection(socket);
    } else {
      rejectConnection(socket);
    }
  }

  void acceptConnection(Socket *socket) {
    auto id = connections++;
    auto team = id; // Default team = client id

    auto player = new Player{
      socket->peerAddress().toString(), team
    };

    clients[id] = new Client{id, socket, player};
    connect(socket, SIGNAL(readyRead()), this, SLOT(recvMessage()));

    socket->write(msg::AuthDataRequest{id, team});
  }

  void rejectConnection(Socket *) {
    // #FIXME: send message to user!
    qDebug() << "Server: reject connection, server is full";
  }

public:
  Server() {
    server = new QTcpServer{this};
  }

  void start(const QString& address) {
    assert(!isRunning);

    if (!server->listen(QHostAddress{address}, cfg::PORT)) {
      qDebug() << "Server: failed to start because"
               << server->errorString();
    } else {
      qDebug() << "Server: listening on " << address << ":" << cfg::PORT;
    }
  }

  void stop() {
    assert(isRunning);

    qDebug() << "Server: shutdown server";
    server->close();
    isRunning = false;
  }

  void startAcceptingClients() {
    server->setMaxPendingConnections(cfg::PLAYERS_MAX);
    connect(server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
  }

  void stopAcceptingClients() {
    server->setMaxPendingConnections(0);
    disconnect(server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
  }

  ~Server() {
    if (isRunning) {
      stop();
    }
  }

  ClientsIter getClientsIter() {
    return ClientsIter{clients, connections};
  }

private:
  QTcpServer *server;
  bool isRunning = false;
  Client *clients[cfg::PLAYERS_MAX];
  qint8 connections = 0;
};
