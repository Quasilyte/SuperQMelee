#pragma once

#include <QtGlobal>

enum class MessageType: qint8 {
  NIL_TYPE,
  AUTH_DATA_REQUEST,
  AUTH_DATA,
  AUTH_CONFIRM,
  PLAYER_LIST_REQUEST,
  PLAYER_LIST,
  NEW_PLAYER,
  PRIVATE_TEXT,
  PUBLIC_TEXT,
  UPPER_BOUND,
};
