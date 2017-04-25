#include <deque>
#include "variable.h"

#ifndef MONOTONIC_VARIABLE_H
#define MONOTONIC_VARIABLE_H

class MonotonicVariable : public Variable {
 private:
  bool inc;
  int last_value;
  int before_last_value;
  bool initialised;
  bool ok;
 public:
  void newValue(const std::string& formatted_string, int);
  bool isOk();
  std::string getStatusMessage();
  MonotonicVariable(bool inc);
};

#endif
