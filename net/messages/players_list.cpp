#include "header.h"

#include "net/client.h"

PlayerList::PlayerList(msg::Id id, ClientsIter clients):
Message{TYPE, id, msg::Header::MAX_SIZE} {
  while (auto client = clients.next()) {
    Player::Codec::intoBytes(client->getPlayer(), &bytes);
  }
}

NewPlayer::NewPlayer(msg::Id id, Player *player):
Message{TYPE, id, Player::Codec::sizeOf(player)} {
  Player::Codec::intoBytes(player, &bytes);
}
