#include <deque>
#include "variable.h"

#ifndef MONOTONIC_VARIABLE_H
#define MONOTONIC_VARIABLE_H

class MonotonicVariable : public Variable {
 private:
  bool inc;
  int last_value;
  bool initialised;
  bool ok;
 public:
  void newValue(int);
  bool isOk();
  MonotonicVariable(bool inc);
};

#endif
