#pragma once

#include <QString>
#include <QTcpSocket>

#include "utils.h"
#include "config.h"

class Player {
public:
  class Codec;

  typedef qint8 Team;

  Player() {}

  Player(QString name): name{name} {}

  Player(QString ip, Team team):
  ip{ip}, team{team} {}

  Player(QString name, QString ip, Team team):
  name{name}, ip{ip}, team{team} {}

  const QString& getName() const {
    return name;
  }

  const QString& getIp() const {
    return ip;
  }

  Team getTeam() const {
    return team;
  }

  void setName(QString name) {
    this->name = name;
  }

  void setTeam(Team team) {
    this->team = team;
  }

private:
  QString name;
  QString ip;
  Team team;
};

class Player::Codec {
public:
  static quint8 sizeOf(Player *player) {
    quint8 name = 1 + player->getName().length();
    quint8 ip = 1 + player->getIp().length();
    quint8 team = 1;

    return name + ip + team;
  }

  static Player fromSocket(QTcpSocket *socket) {
    static char nameBuf[Config::NICKNAME_MAX_LEN];
    static char ipBuf[Config::IP_MAX_LEN];

    auto nameLen = readByte(socket);
    socket->read(nameBuf, nameLen);

    auto ipLen = readByte(socket);
    socket->read(ipBuf, ipLen);

    auto team = readByte(socket);

    return Player{
      QString::fromAscii(nameBuf, nameLen),
      QString::fromAscii(ipBuf, ipLen),
      team
    };
  }
};
