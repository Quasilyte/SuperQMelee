#include "header.h"

#include "net/client.h"

PlayerList::PlayerList(MessageHeader::Id id, ClientsIter clients):
XMessage{TYPE, id, MessageHeader::MAX_SIZE} {
  while (auto client = clients.next()) {
    auto player = client->getPlayer();

    bytes.append(static_cast<char>(player->getName().length()));
    bytes.append(player->getName());
    bytes.append(static_cast<char>(player->getIp().length()));
    bytes.append(player->getIp());
    bytes.append(player->getTeam());
  }
}
