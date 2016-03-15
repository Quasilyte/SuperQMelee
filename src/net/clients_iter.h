#pragma once

class Client;

class ClientsIter {
public:
  ClientsIter(Client **clients, int count):
  clients{clients}, count{count} {}

  Client* next() {
    seekNext();
    return pos == count ? nullptr : clients[pos++];
  }

private:
  void seekNext();

  Client **clients;
  int count;
  int pos = 0;
};
