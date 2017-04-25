#include <cstdio>
#include <string>
#include "monotonic_variable.h"

using namespace std;

MonotonicVariable::MonotonicVariable(bool inc) : Variable(dist_monotonic) {
  dist = dist_monotonic;
  this->inc = inc;
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
  string res = string("Should be monotonically ") + (inc?"increasing":"decreasing") + " but last two values were " + to_string(before_last_value) + " and " + to_string(last_value);
  return res;
}

void MonotonicVariable::newValue(const string& formatted_string, int x) {
  if (!initialised) {
    initialised = true;
    last_value = x;
    return;
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
}
