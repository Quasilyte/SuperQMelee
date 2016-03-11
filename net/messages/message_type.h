#pragma once

#include <QtGlobal>

enum class MessageType: qint8 {
  NIL_TYPE,
  AUTH_DATA_REQUEST = 2,
  AUTH_DATA = 3,
  AUTH_CONFIRM = 4,
  PLAYER_LIST_REQUEST = 5,
  PLAYER_LIST = 6,
  NEW_PLAYER = 7,
  PRIVATE_TEXT = 8,
  PUBLIC_TEXT = 9,
  UPPER_BOUND,
};
