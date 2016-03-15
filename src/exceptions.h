#pragma once

#include <stdexcept>

class UnexpectedInvocation: public std::logic_error {
  using std::logic_error::logic_error;
};
