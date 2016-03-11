#pragma once

/*
#include "config.h"

#include <QByteArray>
#include <QTcpSocket>

#include "socket.h"

#include <limits>

class Message {
public:
  class Exception{};
  class SizeOverflow: public Exception{};
  class UnknownType: public Exception{};
  class InvalidId: public Exception{};

  typedef quint8 Size;
  typedef qint8 Id;
  enum Type: quint8 {
    NIL_TYPE,
    HOST_IS_FULL, // S->C
    AUTH_DATA_REQUEST, // S->C
    AUTH_DATA, // C->S
    AUTH_CONFIRM, // S->C
    PLAYER_LIST_REQUEST, // C->S
    PLAYER_LIST, // S->C
    NEW_PLAYER, // S->C
    PRIVATE_TEXT, // S<->C
    PUBLIC_TEXT, // S<->C
    CHANGE_TEAM, // S<->C
    UPPER_BOUND
  };

  static const qint8 META_DATA_SIZE =
      sizeof(Size) + sizeof(Type) + sizeof(Id);
  static const quint8 MAX_SIZE =
      std::numeric_limits<quint8>::max() - META_DATA_SIZE;

  Message() {}

  Message(Socket *socket) {
    initFromSocket(socket);
  }

  Message(Type type, Id id, int size = 0):
  size{static_cast<Size>(size)}, type{type}, id{id} {
    initFromValues();
  }

  auto append(char c) {
    bytea.append(c);
    return this;
  }

  auto append(const QString& source) {
    bytea.append(source);
    return this;
  }

  auto append(const QByteArray& source) {
    bytea.append(source);
    return this;
  }

  void embed(const QString& source) {
    bytea.append(static_cast<char>(source.length()));
    bytea.append(source);
  }

  void embed(qint8 byte) {
    bytea.append(static_cast<char>(byte));
  }

  bool isBroken(Socket *socket) {
    return socket->bytesAvailable() != size;
  }

  bool is(Type type) const noexcept {
    return this->type == type;
  }

  const char* getData() const noexcept {
    return bytea.data();
  }

  qint64 getTotalSize() const noexcept {
    return bytea.length();
  }

  Size getSize() const noexcept {
    return size;
  }

  Id getId() const noexcept {
    return id;
  }

  Type getType() const noexcept {
    return type;
  }

private:
  Size size;
  Type type = NIL_TYPE;
  Id id;
  QByteArray bytea;

  void validate() {    
    if (size > MAX_SIZE) {
      throw SizeOverflow{};
    }

    if (!(type > NIL_TYPE && type < UPPER_BOUND)) {
      throw UnknownType{};
    }

    if (id < Config::ID_MIN || id > Config::ID_MAX) {
      throw InvalidId{};
    }
  }

  void readMetaData(Socket *socket) {
    socket->read(reinterpret_cast<char*>(this), META_DATA_SIZE);
  }

  void writeMetaData() {
    bytea.append(reinterpret_cast<char*>(this), META_DATA_SIZE);
  }

  void initFromSocket(Socket *socket) {
    if (socket->bytesAvailable() >= META_DATA_SIZE) {
      readMetaData(socket);
      validate();
    }
  }

  void initFromValues() {
    validate();
    bytea.reserve(size + META_DATA_SIZE);
    writeMetaData();
  }

  static_assert(
    META_DATA_SIZE == sizeof(char) * 3 && MAX_SIZE > 128,
    "platform data type size expectations"
  );
};
*/
