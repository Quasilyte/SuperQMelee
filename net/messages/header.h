#pragma once

#include "net/config.h"
#include "net/clients_iter.h"
#include "message_type.h"
#include "utils/constexpr.h"
#include "net/socket.h"

#include <QtGlobal>
#include <QByteArray>
#include <limits>
#include <QString>

namespace msg {
  typedef quint8 Size;
  typedef qint8 Id;

  class Header;
}

class Player;

class msg::Header {
public:
  class Exception{};
  class SizeOverflow: public Exception{};
  class UnknownType: public Exception{};
  class InvalidId: public Exception{};

  static const qint8 META_DATA_SIZE =
      sizeof(Size) + sizeof(MessageType) + sizeof(Id);
  static const quint8 MAX_SIZE =
      std::numeric_limits<quint8>::max() - META_DATA_SIZE;

  Header(MessageType type, Id id, int size):
  size{static_cast<Size>(size)}, type{type}, id{id} {}

  Header(Socket *socket) {
    socket->read(reinterpret_cast<char*>(this), META_DATA_SIZE);
  }

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
  }

  static_assert(
    META_DATA_SIZE == sizeof(char) * 3 && MAX_SIZE > 128,
    "platform data type size expectations"
  );
};

class Message {
public:
  Message(MessageType type, msg::Id id, int size = 0):
  header{type, id, size} {
    bytes.reserve(size + msg::Header::META_DATA_SIZE);
    bytes.append(reinterpret_cast<char*>(&header), msg::Header::META_DATA_SIZE);
  }

  qint64 getDataSize() const noexcept {
    return bytes.length();
  }

  const char* getData() const noexcept {
    return bytes.data();
  }

protected:
  msg::Header header;
  QByteArray bytes;
};

struct PublicText: public Message {
  PublicText(msg::Id id, QString author, QString text):
  Message{TYPE, id, author.length() + Constexpr::strlen(": ") + text.length()} {
    bytes.append(author);
    bytes.append(": ");
    bytes.append(text);
  }

  PublicText(msg::Id id, QString text):
  Message{TYPE, id, text.length()} {
    bytes.append(text);
  }

  static const auto TYPE = MessageType::PUBLIC_TEXT;
};

struct AuthDataRequest: public Message {
  AuthDataRequest(msg::Id id, qint8 team):
  Message{TYPE, id, 1} {
    bytes.append(team);
  }

  static const auto TYPE = MessageType::AUTH_DATA_REQUEST;
};

struct AuthConfirm: public Message {
  AuthConfirm(msg::Id id):
  Message{TYPE, id} {}

  static const auto TYPE = MessageType::AUTH_CONFIRM;
};

struct PlayerList: public Message {
  PlayerList(msg::Id id, ClientsIter clients);

  static const auto TYPE = MessageType::PLAYER_LIST;
};

struct PrivateText: public Message {
  PrivateText(msg::Id id, QString author, QString text):
  Message{TYPE, id, author.length() + Constexpr::strlen(": ") + text.length()} {
    bytes.append(author);
    bytes.append(": ");
    bytes.append(text);
  }

  PrivateText(msg::Id id, qint8 team, QString text):
  Message{TYPE, id, 1 + text.length()} {
    bytes.append(team);
    bytes.append(text);
  }

  static const auto TYPE = MessageType::PRIVATE_TEXT;
};

struct AuthData: public Message {
  AuthData(msg::Id id, QString name):
  Message{TYPE, id, name.length()} {
    bytes.append(name);
  }

  static const auto TYPE = MessageType::AUTH_DATA;
};

struct PlayerListRequest: public Message {
  PlayerListRequest(msg::Id id):
  Message{TYPE, id} {}

  static const auto TYPE = MessageType::PLAYER_LIST_REQUEST;
};

struct NewPlayer: public Message {
  NewPlayer(msg::Id id, Player *player);

  static const auto TYPE = MessageType::NEW_PLAYER;
};
