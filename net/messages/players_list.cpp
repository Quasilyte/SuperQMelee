#include "header.h"

#include "net/client.h"

msg::PlayerList::PlayerList(Id id, ClientsIter clients):
Base{Type::PLAYER_LIST, id, MAX_SIZE} {
  while (auto client = clients.next()) {
    Player::Codec::intoBytes(client->getPlayer(), &bytes);
  }
}

msg::NewPlayer::NewPlayer(Id id, Player *player):
Base{Type::NEW_PLAYER, id, Player::Codec::sizeOf(player)} {
  Player::Codec::intoBytes(player, &bytes);
}
