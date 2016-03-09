#pragma once

#include <QtGlobal>

#include "utils/constexpr.h"

struct Config {
  static const qint8 ID_MIN = 0;
  static const qint8 ID_MAX = 3;
  static const qint8 PLAYERS_MAX = ID_MAX - ID_MIN + 1;

  static const int NICKNAME_MAX_LEN = 12;
  static const int IP_MAX_LEN = Constexpr::strlen("255.255.255.255");
};
