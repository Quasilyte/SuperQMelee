#pragma once

#include "net/config.h"
#include "net/clients_iter.h"
#include "utils/constexpr.h"
#include "net/socket.h"

#include <QtGlobal>
#include <QByteArray>
#include <limits>
#include <QString>

class Player;

namespace msg {
  typedef quint16 Size;
  typedef qint8 Id;

  class Header;
  class Base;
  class PublicText;
  class PrivateText;
  class AuthDataRequest;
  class AuthData;
  class AuthConfirm;
  class PlayerListRequest;
  class PlayerList;
  class NewPlayer;
  class Command;

  enum class Type: qint8 {
    NIL_TYPE,
    AUTH_DATA_REQUEST,
    AUTH_DATA,
    AUTH_CONFIRM,
    PLAYER_LIST_REQUEST,
    PLAYER_LIST,
    NEW_PLAYER,
    PRIVATE_TEXT,
    PUBLIC_TEXT,
    COMMAND,
    UPPER_BOUND,
  };

  const int META_DATA_SIZE =
      sizeof(Size) + sizeof(Type) + sizeof(Id);
  const Size MAX_SIZE =
      std::numeric_limits<quint16>::max() - META_DATA_SIZE;

  static_assert(
    META_DATA_SIZE == 4 && MAX_SIZE > 32767, // ((2 >> 14) - 1)
    "platform data type size expectations"
  );
}

class msg::Header {
public:
  class Exception{};
  class SizeOverflow: public Exception{};
  class UnknownType: public Exception{};
  class InvalidId: public Exception{};

  Header(Type type, Id id, int size):
  size{static_cast<Size>(size)}, type{type}, id{id} {
    validate();
  }

  Header(Socket *socket) {
    socket->read(reinterpret_cast<char*>(this), META_DATA_SIZE);
    validate();
  }

  Size getSize() const noexcept { return size; }
  Id getId() const noexcept { return id; }
  Type getType() const noexcept { return type; }

private:
  Size size;
  Type type = Type::NIL_TYPE;
  Id id;

  void validate() {
    if (size > MAX_SIZE) {
      throw SizeOverflow{};
    }

    if (!(type > Type::NIL_TYPE && type < Type::UPPER_BOUND)) {
      throw UnknownType{};
    }
  }
};

class msg::Base {
public:
  Base(Type type, Id id, int size = 0):
  header{type, id, size} {
    bytes.reserve(size + msg::META_DATA_SIZE);
    bytes.append(reinterpret_cast<char*>(&header), msg::META_DATA_SIZE);
  }

  qint64 getDataSize() const noexcept {
    return bytes.length();
  }

  const char* getData() const noexcept { return bytes.data(); }

protected:
  Header header;
  QByteArray bytes;
};

struct msg::PublicText: public Base {
  PublicText(Id id, QString author, QString text):
  Base{Type::PUBLIC_TEXT, id, author.length() + Constexpr::strlen(": ") + text.length()} {
    bytes.append(author).append(": ").append(text);
  }

  PublicText(Id id, QString text): Base{Type::PUBLIC_TEXT, id, text.length()} {
    bytes.append(text);
  }
};

struct msg::AuthDataRequest: public Base {
  AuthDataRequest(Id id, qint8 team): Base{Type::AUTH_DATA_REQUEST, id, 1} {
    bytes.append(team);
  }
};

struct msg::AuthConfirm: public Base {
  AuthConfirm(Id id): Base{Type::AUTH_CONFIRM, id} {}
};

struct msg::PlayerList: public Base {
  PlayerList(Id id, ClientsIter clients);

  static int batchSize(ClientsIter clients);
};

struct msg::PrivateText: public Base {
  PrivateText(Id id, QString author, QString text):
  Base{Type::PRIVATE_TEXT, id, author.length() + Constexpr::strlen(": ") + text.length()} {
    bytes.append(author).append(": ").append(text);
  }

  PrivateText(Id id, qint8 team, QString text):
  Base{Type::PRIVATE_TEXT, id, 1 + text.length()} {
    bytes.append(team).append(text);
  }
};

struct msg::AuthData: public Base {
  AuthData(Id id, QString name): Base{Type::AUTH_DATA, id, name.length()} {
    bytes.append(name);
  }
};

struct msg::PlayerListRequest: public Base {
  PlayerListRequest(Id id): Base{Type::PLAYER_LIST_REQUEST, id} {}
};

struct msg::NewPlayer: public Base {
  NewPlayer(Id id, Player *player);
};

struct msg::Command: public Base {
  Command(Id id, qint32 bits): Base{Type::COMMAND, id, sizeof(qint32)} {
    bytes.append(reinterpret_cast<char*>(&bits), sizeof(qint32));
  }
};
