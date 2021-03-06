#include <cstdio>
#include <string>
#include <mutex>
#include "monotonic_variable.h"

using namespace std;

MonotonicVariable::MonotonicVariable(bool inc, bool strict) : Variable(dist_monotonic) {
  this->inc = inc;
  this->strict = strict;
  this->initialised = false;
  this->ok = true;
}

bool MonotonicVariable::isOk() {
  return ok;
}

string MonotonicVariable::getStatusMessage() {
  if (ok) {
    return "OK";
  }
  last_value_mutex.lock();
  string res = string("Should be monotonically ") + (strict?"strictly ":"") + (inc?"increasing":"decreasing") + " but last two values were " + to_string(before_last_value) + " and " + to_string(last_value);
  last_value_mutex.unlock();
  return res;
}

void MonotonicVariable::newValue(const string& formatted_string, long long x) {
  last_value_mutex.lock();
  if (!initialised) {
    initialised = true;
    last_value = x;
    return;
  }

  if (strict) {
    if (x == last_value) {
      ok = false;
    }
  }
  if (inc) {
    if (x < last_value) {
      ok = false;
    }
  } else {
    if (x > last_value) {
      ok = false;
    }
  }
  before_last_value = last_value;
  last_value = x;
  last_value_mutex.unlock();
}
