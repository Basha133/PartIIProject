#include <deque>
#include <mutex>
#include "variable.h"

#ifndef MONOTONIC_VARIABLE_H
#define MONOTONIC_VARIABLE_H

class MonotonicVariable : public Variable {
 private:
  bool inc;
  bool strict;
  int last_value;
  int before_last_value;
  bool initialised;
  std::mutex last_value_mutex;
  bool ok;
 public:
  void newValue(const std::string& formatted_string, long long value);
  bool isOk();
  std::string getStatusMessage();
  MonotonicVariable(bool inc, bool strict);
};

#endif
