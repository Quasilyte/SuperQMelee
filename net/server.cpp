#include "server_packet.h"

const char StatusResponse::AUTH_CONFIRM_BODY[StatusResponse::META_DATA_SIZE] = {
  static_cast<char>(2),
  static_cast<char>(StatusResponse::AUTH_CONFIRM),
};
