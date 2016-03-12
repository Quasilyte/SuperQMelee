#include "header.h"

#include "net/client.h"

msg::PlayerList::PlayerList(Id id, ClientsIter clients):
Base{Type::PLAYER_LIST, id, batchSize(clients)} {
  while (auto client = clients.next()) {
    Player::Codec::intoBytes(client->getPlayer(), &bytes);
  }
}

int msg::PlayerList::batchSize(ClientsIter clients) {
  int size = 0;

  while (auto client = clients.next()) {
    size += Player::Codec::sizeOf(client->getPlayer());
  }

  return size;
}

msg::NewPlayer::NewPlayer(Id id, Player *player):
Base{Type::NEW_PLAYER, id, Player::Codec::sizeOf(player)} {
  Player::Codec::intoBytes(player, &bytes);
}
