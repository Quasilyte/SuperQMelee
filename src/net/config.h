#pragma once

#include <QtGlobal>

#include "utils/constexpr.h"

namespace cfg {
  const qint8 ID_MIN = 0;
  const qint8 ID_MAX = 3;
  const qint8 PLAYERS_MAX = ID_MAX - ID_MIN + 1;

  const int NICKNAME_MAX_LEN = 12;
  const int IP_MAX_LEN = Constexpr::strlen("255.255.255.255");
  const unsigned PORT = 8080;
}
