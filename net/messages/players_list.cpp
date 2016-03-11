#include "header.h"

#include "net/client.h"

PlayerList::PlayerList(MessageHeader::Id id, ClientsIter clients):
XMessage{TYPE, id, MessageHeader::MAX_SIZE} {
  while (auto client = clients.next()) {
    Player::Codec::intoBytes(client->getPlayer(), &bytes);
  }
}

NewPlayer::NewPlayer(MessageHeader::Id id, Player *player):
XMessage{TYPE, id, Player::Codec::sizeOf(player)} {
  Player::Codec::intoBytes(player, &bytes);
}
