#pragma once

#include "net/config.h"
#include "net/clients_iter.h"
#include "message_type.h"
#include "utils/constexpr.h"

#include <QtGlobal>
#include <QByteArray>
#include <limits>
#include <QString>

class Player;

class MessageHeader {
public:
  class Exception{};
  class SizeOverflow: public Exception{};
  class UnknownType: public Exception{};
  class InvalidId: public Exception{};

  typedef quint8 Size;
  typedef qint8 Id;

  static const qint8 META_DATA_SIZE =
      sizeof(Size) + sizeof(MessageType) + sizeof(Id);
  static const quint8 MAX_SIZE =
      std::numeric_limits<quint8>::max() - META_DATA_SIZE;

  MessageHeader(MessageType type, Id id, int size):
  size{static_cast<Size>(size)}, type{type}, id{id} {}

  Size getSize() const noexcept {
    return size;
  }

  Id getId() const noexcept {
    return id;
  }

  MessageType getType() const noexcept {
    return type;
  }

private:
  Size size;
  MessageType type = MessageType::NIL_TYPE;
  Id id;

  void validate() {
    if (size > MAX_SIZE) {
      throw SizeOverflow{};
    }

    if (!(type > MessageType::NIL_TYPE && type < MessageType::UPPER_BOUND)) {
      throw UnknownType{};
    }

    if (id < Config::ID_MIN || id > Config::ID_MAX) {
      throw InvalidId{};
    }
  }
};

class XMessage {
public:
  XMessage(MessageType type, MessageHeader::Id id, int size = 0):
  header{type, id, size} {
    bytes.reserve(size + MessageHeader::META_DATA_SIZE);
    bytes.append(reinterpret_cast<char*>(&header), MessageHeader::META_DATA_SIZE);
  }

  qint64 getDataSize() const noexcept {
    return bytes.length();
  }

  const char* getData() const noexcept {
    return bytes.data();
  }

protected:
  MessageHeader header;
  QByteArray bytes;
};

struct PublicText: public XMessage {
  PublicText(MessageHeader::Id id, QString author, QString text):
  XMessage{TYPE, id, author.length() + Constexpr::strlen(": ") + text.length()} {
    bytes.append(author);
    bytes.append(": ");
    bytes.append(text);
  }

  PublicText(MessageHeader::Id id, QString text):
  XMessage{TYPE, id, text.length()} {
    bytes.append(text);
  }

  static const auto TYPE = MessageType::PUBLIC_TEXT;
};

struct AuthDataRequest: public XMessage {
  AuthDataRequest(MessageHeader::Id id, qint8 team):
  XMessage{TYPE, id, 1} {
    bytes.append(team);
  }

  static const auto TYPE = MessageType::AUTH_DATA_REQUEST;
};

struct AuthConfirm: public XMessage {
  AuthConfirm(MessageHeader::Id id):
  XMessage{TYPE, id} {}

  static const auto TYPE = MessageType::AUTH_CONFIRM;
};

struct PlayerList: public XMessage {
  PlayerList(MessageHeader::Id id, ClientsIter clients);

  static const auto TYPE = MessageType::PLAYER_LIST;
};

struct PrivateText: public XMessage {
  PrivateText(MessageHeader::Id id, QString author, QString text):
  XMessage{TYPE, id, author.length() + Constexpr::strlen(": ") + text.length()} {
    bytes.append(author);
    bytes.append(": ");
    bytes.append(text);
  }

  PrivateText(MessageHeader::Id id, qint8 team, QString text):
  XMessage{TYPE, id, 1 + text.length()} {
    bytes.append(team);
    bytes.append(text);
  }

  static const auto TYPE = MessageType::PRIVATE_TEXT;
};

struct AuthData: public XMessage {
  AuthData(MessageHeader::Id id, QString name):
  XMessage{TYPE, id, name.length()} {
    bytes.append(name);
  }

  static const auto TYPE = MessageType::AUTH_DATA;
};

struct PlayerListRequest: public XMessage {
  PlayerListRequest(MessageHeader::Id id):
  XMessage{TYPE, id} {}

  static const auto TYPE = MessageType::PLAYER_LIST_REQUEST;
};

struct NewPlayer: public XMessage {
  NewPlayer(MessageHeader::Id id, Player *player);

  static const auto TYPE = MessageType::NEW_PLAYER;
};
