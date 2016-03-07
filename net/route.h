#pragma once

#include <QString>

struct Route {
  QString address;
  unsigned port;

  Route(): address{"localhost"}, port{DEFAULT_PORT} {}
  Route(QString address, unsigned port = DEFAULT_PORT): address{address}, port{port} {}

  static const unsigned DEFAULT_PORT = 8080;
};
