#include "clients_iter.h"

#include "client.h"

void ClientsIter::seekNext() {
  while (pos != count && !clients[pos]->hasAuth()) {
    pos += 1;
  }
}
