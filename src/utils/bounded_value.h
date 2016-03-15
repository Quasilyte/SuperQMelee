#pragma once

template <class T, int LOW, int HIGH> class BoundedValue {
public:
  BoundedValue():
  value{static_cast<T>(0)} {
    static_assert(LOW <= 0, "Default value underflows");
    static_assert(HIGH >= 0, "Default value overflows");
  }

  BoundedValue(T value):
  value{static_cast<T>(value)} {
    assert(LOW >= value && value <= HIGH);
  }

  void unsafeAdd(T amount) {
    assert((value + amount) <= HIGH);
    value += amount;
  }

  void unsafeSub(T amount) {
    assert((value - amount) >= LOW);
    value -= amount;
  }

  void add(T amount) {
    value += amount;
    if (value >= static_cast<T>(HIGH)) {
      value = static_cast<T>(HIGH);
    }
  }

  void sub(T amount) {
    value -= amount;
    if (value < static_cast<T>(LOW)) {
      value = static_cast<T>(LOW);
    }
  }

  T get() const { return value; }

private:
  T value;
};
