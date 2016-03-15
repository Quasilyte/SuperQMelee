#pragma once

template <class T, int LOW, int HIGH> class CyclicValue {
public:
  CyclicValue():
  value{static_cast<T>(0)} {}

  CyclicValue(T value):
  value{value} {}
  
  void add(T amount) {
    value += amount;
    if (value >= static_cast<T>(HIGH)) {
      value -= static_cast<T>(HIGH);
    }
  }

  void sub(T amount) {
    value -= amount;
    if (value < static_cast<T>(LOW)) {
      value = static_cast<T>(HIGH) + value;
    }
  }

  T get() const { return value; }

private:
  T value;
};
