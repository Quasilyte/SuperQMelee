#pragma once

#include <QString>

class Player {
public:
  Player() {}

  Player(QString name): name{name} {}

  QString getName() const {
    return name;
  }

  void setName(QString name) {
    this->name = name;
  }

private:
  QString name;
};
