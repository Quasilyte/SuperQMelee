#pragma once

class Constexpr {
public:
  static constexpr int strlen(const char* s) {
    return *s ? strlen(s + 1) + 1 : 0;
  }
};
